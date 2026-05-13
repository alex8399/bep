#include "sequential_boruvka_mst_solver_findpivot.hpp"

#include "graph.hpp"
#include <numeric>
#include <vector>
#include <stdexcept>
#include "experiment_setup.hpp"

static inline int findPivot(std::vector<int> &pivots, int vertex);
static void uniteRoots(std::vector<int> &pivots, std::vector<int> &sizes, int root1, int root2);

constexpr int NULL_EDGE = -1;

static inline int findPivot(std::vector<int> &pivots, int vertex)
{
    int root = vertex;
    int next;

    while (pivots[root] != root)
    {
        root = pivots[root];
    }

    while (pivots[vertex] != vertex)
    {
        next = pivots[vertex];
        pivots[vertex] = root;
        vertex = next;
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

void SequentialBoruvkaMSTSolverWithFindPivot::calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup)
{
    mst.verticesNum = graph.verticesNum;
    mst.edges.reserve(graph.verticesNum > 0 ? graph.verticesNum - 1 : 0);

    std::vector<int> pivots(graph.verticesNum);
    std::iota(pivots.begin(), pivots.end(), 0);
    std::vector<int> sizes(graph.verticesNum, 1);
    std::vector<int> cheapestEdge(graph.verticesNum);

    int treesNum = graph.verticesNum;

    while (treesNum > 1)
    {
        experimentSetup.edgePhaseTimer.start();

        std::fill(cheapestEdge.begin(), cheapestEdge.end(), NULL_EDGE);

        for (int ind = 0; ind < graph.edges.size(); ++ind)
        {
            const Edge &edge = graph.edges[ind];
            int pivot1 = findPivot(pivots, edge.source);
            int pivot2 = findPivot(pivots, edge.target);

            if (pivot1 != pivot2)
            {
                if (cheapestEdge[pivot1] == NULL_EDGE || edge.weight < graph.edges[cheapestEdge[pivot1]].weight)
                {
                    cheapestEdge[pivot1] = ind;
                }
                if (cheapestEdge[pivot2] == NULL_EDGE || edge.weight < graph.edges[cheapestEdge[pivot2]].weight)
                {
                    cheapestEdge[pivot2] = ind;
                }
            }
        }

        experimentSetup.edgePhaseTimer.stop();

        bool merged = false;

        for (int vertex = 0; vertex < graph.verticesNum; ++vertex)
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