#include "cuda_boruvka_mst_solver.hpp"

#include "graph.hpp"
#include <cuda_runtime.h>
#include "experiment_setup.hpp"
#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <climits>

#define CUDA_CHECK(call)                                                  \
    do                                                                    \
    {                                                                     \
        cudaError_t err__ = (call);                                       \
        if (err__ != cudaSuccess)                                         \
        {                                                                 \
            throw std::runtime_error(std::string("CUDA error: ") +        \
                                     cudaGetErrorString(err__) + " at " + \
                                     __FILE__ + ":" +                     \
                                     std::to_string(__LINE__));           \
        }                                                                 \
    } while (0)

constexpr unsigned long long PACKED_SENTINEL = 0xFFFFFFFFFFFFFFFFull;

// Device-side packed value used for a single 64-bit atomicMin per component
// Layout: [ weight : 32 bits | edge_index : 32 bits ]
// Sentinel "no edge" value is the maximum unsigned long long
__host__ __device__ inline unsigned long long packEdge(int weight, int edgeIndex)
{
    unsigned int biasedWeight = static_cast<unsigned int>(weight) ^ 0x80000000u;
    unsigned int idx = static_cast<unsigned int>(edgeIndex);
    return (static_cast<unsigned long long>(biasedWeight) << 32) | static_cast<unsigned long long>(idx);
}

__host__ __device__ inline int unpackEdgeIndex(unsigned long long packed)
{
    return static_cast<int>(packed & 0xFFFFFFFFull);
}

__global__ void findCheapestEdgesKernel(
    const int *__restrict__ edgeSources,
    const int *__restrict__ edgeTargets,
    const int *__restrict__ edgeWeights,
    int edgesNum,
    const int *__restrict__ pivots,
    unsigned long long *cheapestPacked)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
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
    unsigned long long packed = packEdge(weight, tid);

    atomicMin(&cheapestPacked[root1], packed);
    atomicMin(&cheapestPacked[root2], packed);
}

static inline int findPivotHost(std::vector<int> &pivots, int vertex)
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

static inline void uniteRootsHost(std::vector<int> &pivots, std::vector<int> &sizes, int root1, int root2)
{
    if (sizes[root1] < sizes[root2])
    {
        std::swap(root1, root2);
    }

    pivots[root2] = root1;
    sizes[root1] += sizes[root2];
}

void CudaBoruvkaMSTSolver::calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup)
{
    mst.verticesNum = graph.verticesNum;
    mst.edges.reserve(graph.verticesNum > 0 ? graph.verticesNum - 1 : 0);

    if (graph.verticesNum <= 1)
    {
        return;
    }

    const int V = graph.verticesNum;
    const int E = static_cast<int>(graph.edges.size());

    // Build flat edge arrays
    std::vector<int> hSources(E);
    std::vector<int> hTargets(E);
    std::vector<int> hWeights(E);
    for (int i = 0; i < E; ++i)
    {
        hSources[i] = graph.edges[i].source;
        hTargets[i] = graph.edges[i].target;
        hWeights[i] = graph.edges[i].weight;
    }

    int *dSources = nullptr;
    int *dTargets = nullptr;
    int *dWeights = nullptr;
    int *dPivots = nullptr;
    unsigned long long *dCheapest = nullptr;

    auto cleanup = [&]()
    {
        if (dSources)
            cudaFree(dSources);
        if (dTargets)
            cudaFree(dTargets);
        if (dWeights)
            cudaFree(dWeights);
        if (dPivots)
            cudaFree(dPivots);
        if (dCheapest)
            cudaFree(dCheapest);
    };

    try
    {
        CUDA_CHECK(cudaMalloc(&dSources, sizeof(int) * E));
        CUDA_CHECK(cudaMalloc(&dTargets, sizeof(int) * E));
        CUDA_CHECK(cudaMalloc(&dWeights, sizeof(int) * E));
        CUDA_CHECK(cudaMalloc(&dPivots, sizeof(int) * V));
        CUDA_CHECK(cudaMalloc(&dCheapest, sizeof(unsigned long long) * V));

        CUDA_CHECK(cudaMemcpy(dSources, hSources.data(), sizeof(int) * E, cudaMemcpyHostToDevice));
        CUDA_CHECK(cudaMemcpy(dTargets, hTargets.data(), sizeof(int) * E, cudaMemcpyHostToDevice));
        CUDA_CHECK(cudaMemcpy(dWeights, hWeights.data(), sizeof(int) * E, cudaMemcpyHostToDevice));

        std::vector<int> pivots(V);
        std::iota(pivots.begin(), pivots.end(), 0);
        std::vector<int> sizes(V, 1);
        std::vector<unsigned long long> hCheapest(V);

        const int blockSize = m_blockSize;
        const int edgeBlocks = (E + blockSize - 1) / blockSize;

        int treesNum = V;

        while (treesNum > 1)
        {
            experimentSetup.edgePhaseTimer.start();

            // Push current array pivots to device.
            CUDA_CHECK(
                cudaMemcpy(dPivots, pivots.data(), sizeof(int) * V, cudaMemcpyHostToDevice));
            // Reset cheapest buffer
            CUDA_CHECK(
                cudaMemset(dCheapest, 0xFF, sizeof(unsigned long long) * V));

            findCheapestEdgesKernel<<<edgeBlocks, blockSize>>>(
                dSources, dTargets, dWeights, E, dPivots, dCheapest);
            CUDA_CHECK(cudaGetLastError());

            CUDA_CHECK(
                cudaMemcpy(hCheapest.data(), dCheapest, sizeof(unsigned long long) * V, cudaMemcpyDeviceToHost));

            experimentSetup.edgePhaseTimer.stop();

            bool merged = false;

            for (int v = 0; v < V; ++v)
            {
                unsigned long long packed = hCheapest[v];

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

        cleanup();
    }
    catch (...)
    {
        cleanup();
        throw;
    }
}
