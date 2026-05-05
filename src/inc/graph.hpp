#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <vector>

struct Edge
{
    int source;
    int target;
    int weight;

    Edge() : source(0), target(0), weight(0) {}
    Edge(int source, int target, int weight) : source(source), target(target), weight(weight) {}
};

struct Graph
{
    int verticesNum;
    std::vector<Edge> edges;
};

#endif