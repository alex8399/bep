#include "stdthread_boruvka_mst_solver.hpp"

#include "graph.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>
#include "experiment_setup.hpp"

constexpr int NULL_EDGE = -1;

static inline int findPivot(std::vector<int> &pivots, int vertex)
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

static inline int findPivotSafely(const std::vector<int> &pivots, int vertex)
{
    int root = vertex;

    while (pivots[root] != root)
    {
        root = pivots[root];
    }

    return root;
}

static void uniteRoots(std::vector<int> &pivots, std::vector<int> &sizes, int root1, int root2)
{
    if (sizes[root1] < sizes[root2])
    {
        std::swap(root1, root2);
    }

    pivots[root2] = root1;
    sizes[root1] += sizes[root2];
}

void StdThreadBoruvkaMSTSolver::calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup) const
{
    const int verticesNum = graph.verticesNum;
    const std::size_t edgesNum = graph.edges.size();

    const unsigned hw = std::thread::hardware_concurrency();
    const int threadsNum = static_cast<int>(hw == 0 ? 1 : hw);

    mst.verticesNum = verticesNum;
    mst.edges.reserve(verticesNum > 0 ? verticesNum - 1 : 0);

    std::vector<int> pivots(verticesNum);
    std::iota(pivots.begin(), pivots.end(), 0);
    std::vector<int> sizes(verticesNum, 1);

    std::vector<std::vector<int>> threadLocal(threadsNum, std::vector<int>(verticesNum, NULL_EDGE));
    std::vector<int> cheapestEdge(verticesNum, NULL_EDGE);
    
    std::vector<std::thread> workers;
    workers.reserve(threadsNum);

    int treesNum = verticesNum;

    while (treesNum > 1)
    {
        std::fill(cheapestEdge.begin(), cheapestEdge.end(), NULL_EDGE);

        experimentSetup.edgePhaseTimer.start();

        for (std::vector<int> &vec: threadLocal) { std::fill(vec.begin(), vec.end(), NULL_EDGE); }

        auto scanEdgesWorker = [&](int tid)
        {
            int begin = edgesNum * tid / threadsNum;
            int end = edgesNum * (tid + 1) / threadsNum;

            std::vector<int> &cheapestEdgeLocal = threadLocal[tid];

            for (int ind = begin; ind < end; ++ind)
            {
                const Edge &edge = graph.edges[ind];
                int pivot1 = findPivotSafely(pivots, edge.source);
                int pivot2 = findPivotSafely(pivots, edge.target);

                if (pivot1 != pivot2)
                {
                    if (cheapestEdgeLocal[pivot1] == NULL_EDGE ||
                        edge.weight < graph.edges[cheapestEdgeLocal[pivot1]].weight)
                    {
                        cheapestEdgeLocal[pivot1] = ind;
                    }

                    if (cheapestEdgeLocal[pivot2] == NULL_EDGE ||
                        edge.weight < graph.edges[cheapestEdgeLocal[pivot2]].weight)
                    {
                        cheapestEdgeLocal[pivot2] = ind;
                    }
                }
            }
        };

        for (int tid = 0; tid < threadsNum; ++tid)
        {
            workers.emplace_back(scanEdgesWorker, tid);
        }

        for (std::thread &worker : workers)
        {
            worker.join();
        }

        workers.clear();

        auto reduceWorker = [&](int tid)
        {
            int begin = verticesNum * tid / threadsNum;
            int end = verticesNum * (tid + 1) / threadsNum;

            for (int pivot = begin; pivot < end; ++pivot)
            {
                for (int thread = 0; thread < threadsNum; ++thread)
                {
                    int localEdge = threadLocal[thread][pivot];

                    if (localEdge != NULL_EDGE &&
                        (cheapestEdge[pivot] == NULL_EDGE ||
                         graph.edges[localEdge].weight < graph.edges[cheapestEdge[pivot]].weight))
                    {
                        cheapestEdge[pivot] = localEdge;
                    }
                }
            }
        };

        for (int tid = 0; tid < threadsNum; ++tid)
        {
            workers.emplace_back(reduceWorker, tid);
        }

        for (std::thread &worker : workers)
        {
            worker.join();
        }

        workers.clear();

        experimentSetup.edgePhaseTimer.stop();

        bool merged = false;

        for (int vertex = 0; vertex < verticesNum; ++vertex)
        {
            int edgeInd = cheapestEdge[vertex];
            if (edgeInd != NULL_EDGE)
            {
                const Edge &edge = graph.edges[edgeInd];
                int pivot1 = findPivot(pivots, edge.source);
                int pivot2 = findPivot(pivots, edge.target);

                if (pivot1 != pivot2)
                {
                    uniteRoots(pivots, sizes, pivot1, pivot2);
                    mst.edges.push_back(edge);
                    --treesNum;
                    merged = true;
                }
            }
        }

        if (!merged)
        {
            throw std::runtime_error("MST can not be built because graph is not connected");
        }
    }
}