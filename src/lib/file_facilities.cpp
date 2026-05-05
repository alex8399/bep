#include "file_facilities.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "timer.hpp"

const std::string NEW_FILE_SYMBOL = "\n";
const std::string NEW_MEASUREMENT_SYMBOL = ",";

void readGraphFile(const std::string &filePath, Graph &graph)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Can not open graph file " + filePath);
    }

    std::string line;
    int lineNumber = 1;
    int verticesNum, edgesNum;

    if (std::getline(file, line))
    {
        std::istringstream stringStream(line);

        stringStream >> verticesNum >> edgesNum;

        if (stringStream.fail() || verticesNum <= 0 || edgesNum < 0)
        {
            throw std::runtime_error("Incorrect summary on line " + std::to_string(lineNumber) + " in graph file " + filePath);
        }

        graph.verticesNum = verticesNum;
        graph.edges.reserve(edgesNum);
    }
    else
    {
        throw std::runtime_error("No summary in graph file " + filePath);
    }

    lineNumber++;

    while (std::getline(file, line))
    {
        std::istringstream stringStream(line);
        int source, target, weight;

        stringStream >> source >> target >> weight;

        if (stringStream.fail() || weight < 0 || (source < 0 || graph.verticesNum <= source) || (target < 0 || graph.verticesNum <= target))
        {
            throw std::runtime_error("Incorrect edge definition on line " + std::to_string(lineNumber) + " in graph file " + filePath);
        }

        graph.edges.emplace_back(source, target, weight);
        edgesNum--;

        lineNumber++;
    }

    if (edgesNum != 0)
    {
        throw std::runtime_error("Incorrect number of edges in summary");
    }

    file.close();
}

void dumpResults(const Measurements &results, const std::string &filePath)
{
    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Can not create result file " + filePath);
    }

    for (const auto &measurements : results)
    {
        for (const auto &measurement : measurements)
        {
            file << measurement << NEW_MEASUREMENT_SYMBOL;
        }

        file << NEW_FILE_SYMBOL;
    }

    file.close();
}