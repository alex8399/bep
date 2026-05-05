#include "file_facilities.hpp"
#include <gtest/gtest.h>
#include "graph.hpp"
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "timer.hpp"
#include <vector>

static void expectEqualTextFiles(const std::string &file1, const std::string &file2);
static void expectEqualGraphs(const Graph &graph1, const Graph &graph2);

static void expectEqualTextFiles(const std::string &file1Path, const std::string &file2Path)
{
    std::ifstream file1(file1Path);
    std::ifstream file2(file2Path);

    std::string line1, line2;
    bool run = true;

    while (run)
    {
        bool eof1 = !std::getline(file1, line1);
        bool eof2 = !std::getline(file2, line2);

        if (!eof1 && !eof2)
        {
            EXPECT_EQ(line1, line2);
        }
        else
        {
            EXPECT_TRUE(eof1 && eof2);
            run = false;
        }
    }

    file1.close();
    file2.close();
}

static void expectEqualGraphs(const Graph &graph1, const Graph &graph2)
{
    EXPECT_EQ(graph1.verticesNum, graph2.verticesNum);
    EXPECT_EQ(graph1.edges.size(), graph2.edges.size());

    for (int ind = 0; ind < graph1.edges.size(); ++ind)
    {
        EXPECT_EQ(graph1.edges[ind].source, graph2.edges[ind].source);
        EXPECT_EQ(graph1.edges[ind].target, graph2.edges[ind].target);
        EXPECT_EQ(graph1.edges[ind].weight, graph2.edges[ind].weight);
    }
}

TEST(ReadGraphFileTests, SuccessfulReading01)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_01.txt";
    Graph graph;

    const Graph expectedGraph = {
        .verticesNum = 5,
        .edges = {{0, 1, 4}, {1, 3, 10}, {3, 2, 5}, {3, 4, 5}}
    };

    EXPECT_NO_THROW(readGraphFile(graphFilePath, graph));

    expectEqualGraphs(graph, expectedGraph);
}

TEST(ReadGraphFileTests, SuccessfulReading02)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_02.txt";
    Graph graph;

    const Graph expectedGraph = {
        .verticesNum = 6,
        .edges = {{0, 1, 2}, {0, 2, 3}, {1, 2, 1}, {1, 3, 4}, {2, 3, 2}, {2, 4, 5}, {3, 4, 3}, {3, 5, 6}, {4, 5, 4}}
    };

    EXPECT_NO_THROW(readGraphFile(graphFilePath, graph));

    expectEqualGraphs(graph, expectedGraph);
}

TEST(ReadGraphFileTests, SuccessfulReading03)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_03.txt";
    Graph graph;

    const Graph expectedGraph = {
        .verticesNum = 10,
        .edges = {{0, 1, 5}, {0, 3, 8}, {0, 4, 12}, {1, 2, 6}, {1, 5, 9}, {2, 3, 7}, {2, 6, 11},
                  {3, 4, 10}, {3, 7, 13}, {4, 5, 14}, {4, 8, 15}, {5, 6, 16}, {5, 9, 17}, {6, 7, 18},
                  {6, 9, 19}, {7, 8, 20}, {8, 9, 21}}
    };

    EXPECT_NO_THROW(readGraphFile(graphFilePath, graph));

    expectEqualGraphs(graph, expectedGraph);
}

TEST(ReadGraphFileTests, SuccessfulReading04)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_04.txt";
    Graph graph;

    const Graph expectedGraph = {
        .verticesNum = 7,
        .edges = {{0, 1, 1}, {1, 2, 2}, {3, 4, 3}, {4, 5, 4}, {5, 6, 5}}
    };

    EXPECT_NO_THROW(readGraphFile(graphFilePath, graph));

    expectEqualGraphs(graph, expectedGraph);
}

TEST(ReadGraphFileTests, IncorrectVerticeNumber)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_05.txt";
    Graph graph;

    EXPECT_THROW(readGraphFile(graphFilePath, graph), std::runtime_error);
}

TEST(ReadGraphFileTests, IncorrectWeight)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_06.txt";
    Graph graph;

    EXPECT_THROW(readGraphFile(graphFilePath, graph), std::runtime_error);
}

TEST(ReadGraphFileTests, NoTotalVerticesNumber)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_07.txt";
    Graph graph;

    EXPECT_THROW(readGraphFile(graphFilePath, graph), std::runtime_error);
}

TEST(ReadGraphFileTests, IncorrectEdgeLine)
{
    const std::string graphFilePath = "tst/data/file_facilities/graph_08.txt";
    Graph graph;

    EXPECT_THROW(readGraphFile(graphFilePath, graph), std::runtime_error);
}

TEST(ReadGraphFileTests, FileNotExist)
{
    const std::string graphFilePath = "tst/data/file_facilities/file_not_exist.txt";
    Graph graph;

    EXPECT_THROW(readGraphFile(graphFilePath, graph), std::runtime_error);
}

TEST(DumpResultsTests, SuccessfulDumping01)
{
    const std::string resultsFilePath = "tst/tmp/results1.tmp";
    const Measurements results = {{4}, {534}, {46}, {347}};

    const std::string expectedResultsFilePath = "tst/data/file_facilities/results_01.txt";

    EXPECT_NO_THROW(dumpResults(results, resultsFilePath));

    expectEqualTextFiles(resultsFilePath, expectedResultsFilePath);
}

TEST(DumpResultsTests, SuccessfulDumping02)
{
    const std::string resultsFilePath = "tst/tmp/results2.tmp";
    const Measurements results = {};

    const std::string expectedResultsFilePath = "tst/data/file_facilities/results_02.txt";

    EXPECT_NO_THROW(dumpResults(results, resultsFilePath));

    expectEqualTextFiles(resultsFilePath, expectedResultsFilePath);
}

TEST(DumpResultsTests, SuccessfulDumping03)
{
    const std::string resultsFilePath = "tst/tmp/results3.tmp";
    const Measurements results = {{123456789012345}};

    const std::string expectedResultsFilePath = "tst/data/file_facilities/results_03.txt";

    EXPECT_NO_THROW(dumpResults(results, resultsFilePath));

    expectEqualTextFiles(resultsFilePath, expectedResultsFilePath);
}

TEST(DumpResultsTests, SuccessfulDumping04)
{
    const std::string resultsFilePath = "tst/tmp/results4.tmp";
    const Measurements results = {{1, 2, 3, 4, 5, 6}, {7, 8, 9, 10}};

    const std::string expectedResultsFilePath = "tst/data/file_facilities/results_04.txt";

    EXPECT_NO_THROW(dumpResults(results, resultsFilePath));

    expectEqualTextFiles(resultsFilePath, expectedResultsFilePath);
}

TEST(DumpResultsTests, SuccessfulDumping05)
{
    const std::string resultsFilePath = "tst/tmp/results5.tmp";
    const Measurements results = {{0, 0, 0, 0, 0}};

    const std::string expectedResultsFilePath = "tst/data/file_facilities/results_05.txt";

    EXPECT_NO_THROW(dumpResults(results, resultsFilePath));

    expectEqualTextFiles(resultsFilePath, expectedResultsFilePath);
}
