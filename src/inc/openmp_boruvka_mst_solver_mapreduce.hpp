#ifndef __OPENMP_BORUVKA_MST_SOLVER_MAPREDUCE_HPP__
#define __OPENMP_BORUVKA_MST_SOLVER_MAPREDUCE_HPP__

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"

class OpenMPBoruvkaMSTSolverMapReduce : public BaseBoruvkaMSTSolver
{
public:
    void calculateMST(const Graph& graph, Graph& mst, ExperimentSetup &experimentSetup) override;
};

#endif