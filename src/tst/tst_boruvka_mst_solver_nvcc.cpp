#if !defined(NVCC)
    #error "Only compiler NVCC can be used"
#endif

#include "graph.hpp"
#include <stdexcept>
#include "experiment_setup.hpp"
#include <iostream>
#include <cassert>

#include "cuda_boruvka_mst_solver.hpp"

#define EXPECT_EQ(a, b) do { \
    assert(a == b);    \
} while(0)

#define EXPECT_THROW_ANY(statement)        \
    do {                                   \
        bool caught = false;               \
        try {                              \
            statement;                     \
        } catch (...) {                    \
            caught = true;                 \
        }                                  \
        assert(caught);  \
    } while(0)

const Graph GRAPH_01{
    .verticesNum = 4,
    .edges = {{0, 1, 10}, {0, 2, 6}, {0, 3, 5}, {1, 3, 15}, {2, 3, 4}}};

const Graph EXPECTED_MST_01{
    .verticesNum = 4,
    .edges = {{0, 3, 5}, {0, 1, 10}, {2, 3, 4}}};

const Graph GRAPH_02{.verticesNum = 1, .edges = {}};

const Graph EXPECTED_MST_02{.verticesNum = 1, .edges = {}};

const Graph GRAPH_03{.verticesNum = 2, .edges = {{0, 1, 42}}};

const Graph EXPECTED_MST_03{.verticesNum = 2, .edges = {{0, 1, 42}}};

const Graph GRAPH_04{
    .verticesNum = 3,
    .edges = {{0, 1, 5}, {0, 1, 2}, {1, 2, 1}, {0, 2, 4}}};

const Graph EXPECTED_MST_04{
    .verticesNum = 3,
    .edges = {{1, 2, 1}, {0, 1, 2}}};

const Graph GRAPH_05{
    .verticesNum = 5,
    .edges = {{0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8}, {1, 4, 5}, {2, 4, 7}, {3, 4, 9}}};

const Graph EXPECTED_MST_05{
    .verticesNum = 5,
    .edges = {{0, 1, 2}, {1, 2, 3}, {1, 4, 5}, {0, 3, 6}}};

const Graph GRAPH_06{
    .verticesNum = 15,
    .edges = {
        {0, 2, 83}, {0, 3, 44}, {0, 4, 15}, {0, 8, 38}, {0, 10, 56}, {0, 11, 21}, {0, 13, 59}, {0, 14, 1}, {1, 4, 93}, {1, 7, 93}, {1, 11, 34}, {1, 14, 65}, {2, 3, 98}, {3, 6, 23}, {3, 7, 65}, {3, 8, 14}, {3, 9, 81}, {3, 10, 39}, {3, 11, 82}, {3, 12, 65}, {4, 6, 78}, {4, 7, 26}, {4, 11, 20}, {4, 13, 48}, {5, 12, 98}, {5, 13, 21}, {6, 7, 70}, {6, 8, 100}, {7, 9, 68}, {7, 10, 1}, {7, 12, 77}, {8, 10, 42}, {8, 13, 63}, {8, 14, 3}, {9, 10, 15}, {9, 14, 47}, {11, 13, 40}, {13, 14, 31}}};

const Graph EXPECTED_MST_06{
    .verticesNum = 15,
    .edges = {
        {0, 14, 1}, {7, 10, 1}, {8, 14, 3}, {3, 8, 14}, {0, 4, 15}, {9, 10, 15}, {4, 11, 20}, {5, 13, 21}, {3, 6, 23}, {4, 7, 26}, {13, 14, 31}, {1, 11, 34}, {3, 12, 65}, {0, 2, 83}}};

const Graph GRAPH_07{
    .verticesNum = 20,
    .edges = {
        {0, 1, 54}, {0, 2, 41}, {0, 4, 46}, {0, 6, 28}, {0, 10, 99}, {0, 13, 58}, {0, 14, 44}, {0, 16, 97}, {0, 18, 25}, {1, 4, 24}, {1, 5, 49}, {1, 7, 8}, {1, 15, 54}, {2, 13, 49}, {2, 15, 65}, {2, 19, 34}, {3, 6, 23}, {3, 10, 72}, {3, 11, 49}, {3, 13, 48}, {3, 15, 9}, {4, 8, 38}, {4, 10, 16}, {4, 14, 34}, {4, 15, 65}, {4, 16, 78}, {5, 8, 42}, {5, 9, 8}, {5, 11, 49}, {5, 13, 7}, {5, 15, 76}, {6, 18, 50}, {6, 19, 39}, {7, 9, 14}, {7, 12, 60}, {7, 17, 74}, {8, 12, 98}, {8, 13, 91}, {9, 12, 42}, {9, 16, 32}, {10, 15, 83}, {10, 19, 58}, {11, 12, 35}, {11, 13, 57}, {11, 16, 73}, {11, 18, 74}, {14, 16, 45}, {14, 18, 32}, {16, 18, 89}, {16, 19, 59}}};

const Graph EXPECTED_MST_07{
    .verticesNum = 20,
    .edges = {
        {0, 6, 28}, {0, 18, 25}, {1, 4, 24}, {1, 7, 8}, {2, 19, 34}, {3, 6, 23}, {3, 15, 9}, {4, 8, 38}, {4, 10, 16}, {4, 14, 34}, {5, 9, 8}, {5, 13, 7}, {6, 19, 39}, {7, 9, 14}, {7, 17, 74}, {9, 12, 42}, {9, 16, 32}, {11, 12, 35}, {14, 18, 32}}};

const Graph GRAPH_NOT_CONNECTED{
    .verticesNum = 5,
    .edges = {{1, 2, 3}, {1, 3, 8}, {1, 4, 5}, {2, 4, 7}, {3, 4, 9}}};

static bool areEqualEdges(const Edge &edge1, const Edge &edge2);
static void expectEqualGraphs(const Graph &graph1, const Graph &graph2);

static bool areEqualEdges(const Edge &edge1, const Edge &edge2)
{
    bool equalWeight = edge1.weight == edge2.weight;
    bool equalEndPoints =
        (edge1.source == edge2.source && edge1.target == edge2.target) || (edge1.source == edge2.target && edge1.target == edge2.source);

    return equalWeight && equalEndPoints;
}

static void expectEqualGraphs(const Graph &graph1, const Graph &graph2)
{
    EXPECT_EQ(graph1.verticesNum, graph2.verticesNum);
    EXPECT_EQ(graph1.edges.size(), graph2.edges.size());

    bool equal = true;

    for (int ind1 = 0; equal && ind1 < graph1.edges.size(); ++ind1)
    {
        bool found = false;

        for (int ind2 = 0; !found && ind2 < graph2.edges.size(); ++ind2)
        {
            found = areEqualEdges(graph1.edges[ind1], graph2.edges[ind2]);
        }

        equal = found ? equal : false;
    }

    EXPECT_EQ(equal, true);
}


void test01()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_01, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_01);
}

void test02()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_02, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_02);
}

void test03()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_03, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_03);
}

void test04()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_04, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_04);
}

void test05()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_05, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_05);
}

void test06()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_06, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_06);
}

void test07()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    solver.calculateMST(GRAPH_07, mst, experimentSetup);

    expectEqualGraphs(mst, EXPECTED_MST_07);
}

void test08()
{
    Graph mst;
    ExperimentSetup experimentSetup;
    CudaBoruvkaMSTSolver solver;

    EXPECT_THROW_ANY(solver.calculateMST(GRAPH_NOT_CONNECTED, mst, experimentSetup));
}

int main() {
    test01();
    test02();
    test03();
    test04();
    test05();
    test06();
    test07();
    test08();

    std::cout << "CUDA TESTS on NVCC: All tests are passed" << std::endl;
}