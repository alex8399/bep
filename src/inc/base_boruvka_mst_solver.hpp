#ifndef __BASE_BORUVKA_MST_SOLVER_HPP__
#define __BASE_BORUVKA_MST_SOLVER_HPP__

#include "graph.hpp"
#include "experiment_setup.hpp"

class BaseBoruvkaMSTSolver
{
public:
    virtual void calculateMST(const Graph& graph, Graph& mst, ExperimentSetup &experimentSetup) = 0;
    virtual ~BaseBoruvkaMSTSolver() = default;
};

#endif