#include "opencl_boruvka_mst_solver.hpp"

#include "graph.hpp"
#include "experiment_setup.hpp"

#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS


#include <CL/opencl.hpp>

#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>
#include <cstdint>

namespace
{
    constexpr cl_ulong PACKED_SENTINEL = 0xFFFFFFFFFFFFFFFFull;

    // Embedded OpenCL kernel source.
    const char *kBoruvkaKernelSource = R"CLSRC(
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable

inline ulong packEdge(int weight, int edgeIndex)
{
    uint biasedWeight = ((uint)weight) ^ 0x80000000u;
    uint idx = (uint)edgeIndex;
    return ((ulong)biasedWeight << 32) | (ulong)idx;
}

__kernel void findCheapestEdges(
    __global const int *restrict edgeSources,
    __global const int *restrict edgeTargets,
    __global const int *restrict edgeWeights,
    const int edgesNum,
    __global const int *restrict pivots,
    __global ulong *cheapestPacked)
{
    int tid = get_global_id(0);
    if (tid >= edgesNum)
    {
        return;
    }

    int src = edgeSources[tid];
    int dst = edgeTargets[tid];

    int root1 = src;
    while (pivots[root1] != root1)
    {
        root1 = pivots[root1];
    }

    int root2 = dst;
    while (pivots[root2] != root2)
    {
        root2 = pivots[root2];
    }

    if (root1 == root2)
    {
        return;
    }

    int weight = edgeWeights[tid];
    ulong packed = packEdge(weight, tid);

    atom_min(&cheapestPacked[root1], packed);
    atom_min(&cheapestPacked[root2], packed);
}
)CLSRC";

    inline int unpackEdgeIndex(cl_ulong packed)
    {
        return static_cast<int>(packed & 0xFFFFFFFFull);
    }

    inline int findPivotHost(std::vector<int> &pivots, int vertex)
    {
        int root = vertex;
        while (pivots[root] != root)
        {
            root = pivots[root];
        }

        while (pivots[vertex] != vertex)
        {
            int next = pivots[vertex];
            pivots[vertex] = root;
            vertex = next;
        }
        return root;
    }

    inline void uniteRootsHost(std::vector<int> &pivots, std::vector<int> &sizes,
                               int root1, int root2)
    {
        if (sizes[root1] < sizes[root2])
        {
            std::swap(root1, root2);
        }

        pivots[root2] = root1;
        sizes[root1] += sizes[root2];
    }
}

void OpenCLBoruvkaMSTSolver::calculateMST(const Graph &graph, Graph &mst,
                                          ExperimentSetup &experimentSetup) const
{
    mst.verticesNum = graph.verticesNum;
    mst.edges.reserve(graph.verticesNum > 0 ? graph.verticesNum - 1 : 0);

    if (graph.verticesNum <= 1)
    {
        return;
    }

    const int V = graph.verticesNum;
    const int E = static_cast<int>(graph.edges.size());

    std::vector<int> hSources(E);
    std::vector<int> hTargets(E);
    std::vector<int> hWeights(E);
    for (int i = 0; i < E; ++i)
    {
        hSources[i] = graph.edges[i].source;
        hTargets[i] = graph.edges[i].target;
        hWeights[i] = graph.edges[i].weight;
    }

    try
    {
        // OpenCL setup
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        cl::Device device;
        bool deviceFound = false;

        for (const auto &platform : platforms)
        {
            if (!m_platformName.has_value() || platform.getInfo<CL_PLATFORM_NAME>() == m_platformName.value())
            {
                std::vector<cl::Device> devices;
                try
                {
                    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
                }
                catch (const cl::Error &)
                {
                    continue;
                }
                if (!devices.empty())
                {
                    device = devices.front();
                    deviceFound = true;
                    break;
                }
            }
        }

        if (!deviceFound)
        {
            throw std::runtime_error("No OpenCL devices found");
        }

        // Verify 64-bit base atomics support (required by atom_min on ulong).
        std::string extensions = device.getInfo<CL_DEVICE_EXTENSIONS>();
        if (extensions.find("cl_khr_int64_base_atomics") == std::string::npos)
        {
            throw std::runtime_error(
                "OpenCL device does not support cl_khr_int64_base_atomics");
        }

        cl::Context context(device);
        cl::CommandQueue queue(context, device);

        // Build the program from the embedded source.
        cl::Program program(context, std::string(kBoruvkaKernelSource));
        try
        {
            program.build({device});
        }
        catch (const cl::Error &)
        {
            std::string log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
            throw std::runtime_error("OpenCL program build failed:\n" + log);
        }

        cl::Kernel kernel(program, "findCheapestEdges");

        // Device buffers
        cl::Buffer dSources(context, CL_MEM_READ_ONLY, sizeof(int) * E);
        cl::Buffer dTargets(context, CL_MEM_READ_ONLY, sizeof(int) * E);
        cl::Buffer dWeights(context, CL_MEM_READ_ONLY, sizeof(int) * E);
        cl::Buffer dPivots(context, CL_MEM_READ_ONLY, sizeof(int) * V);
        cl::Buffer dCheapest(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * V);

        queue.enqueueWriteBuffer(dSources, CL_FALSE, 0, sizeof(int) * E, hSources.data());
        queue.enqueueWriteBuffer(dTargets, CL_FALSE, 0, sizeof(int) * E, hTargets.data());
        queue.enqueueWriteBuffer(dWeights, CL_TRUE, 0, sizeof(int) * E, hWeights.data());

        // Host-side union-find state
        std::vector<int> pivots(V);
        std::iota(pivots.begin(), pivots.end(), 0);
        std::vector<int> sizes(V, 1);
        std::vector<cl_ulong> hCheapest(V);

        const std::size_t blockSize = static_cast<std::size_t>(m_blockSize);
        const std::size_t globalSize =
            ((static_cast<std::size_t>(E) + blockSize - 1) / blockSize) * blockSize;

        int treesNum = V;

        while (treesNum > 1)
        {
            experimentSetup.edgePhaseTimer.start();

            // Push current pivots to device.
            queue.enqueueWriteBuffer(dPivots, CL_FALSE, 0, sizeof(int) * V, pivots.data());

            // Reset cheapest buffer to all 0xFF bytes (== PACKED_SENTINEL).
            cl_uchar pattern = 0xFF;
            queue.enqueueFillBuffer(dCheapest, pattern, 0, sizeof(cl_ulong) * V);

            // Set kernel arguments and launch.
            kernel.setArg(0, dSources);
            kernel.setArg(1, dTargets);
            kernel.setArg(2, dWeights);
            kernel.setArg(3, E);
            kernel.setArg(4, dPivots);
            kernel.setArg(5, dCheapest);

            queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                                       cl::NDRange(globalSize),
                                       cl::NDRange(blockSize));

            // Read results back (blocking — also acts as a sync point).
            queue.enqueueReadBuffer(dCheapest, CL_TRUE, 0,
                                    sizeof(cl_ulong) * V, hCheapest.data());

            experimentSetup.edgePhaseTimer.stop();

            bool merged = false;

            for (int v = 0; v < V; ++v)
            {
                cl_ulong packed = hCheapest[v];

                if (packed != PACKED_SENTINEL)
                {
                    int edgeInd = unpackEdgeIndex(packed);

                    const Edge &edge = graph.edges[edgeInd];
                    int p1 = findPivotHost(pivots, edge.source);
                    int p2 = findPivotHost(pivots, edge.target);
                    if (p1 != p2)
                    {
                        uniteRootsHost(pivots, sizes, p1, p2);
                        mst.edges.push_back(edge);
                        --treesNum;
                        merged = true;
                    }
                }
            }

            if (!merged)
            {
                throw std::runtime_error(
                    "MST can not be built because graph is not connected");
            }
        }
    }
    catch (const cl::Error &e)
    {
        throw std::runtime_error(std::string("OpenCL error: ") + e.what() +
                                 " (code " + std::to_string(e.err()) + ")");
    }
}
