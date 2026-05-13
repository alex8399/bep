#ifndef __SEQUENTIAL_BORUVKA_MST_SOLVER_FINDPIVOTSAFELY_HPP__
#define __SEQUENTIAL_BORUVKA_MST_SOLVER_FINDPIVOTSAFELY_HPP__

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"

class SequentialBoruvkaMSTSolverWithFindPivotSafely : public BaseBoruvkaMSTSolver
{
public:
    void calculateMST(const Graph& graph, Graph& mst, ExperimentSetup &experimentSetup) override;
};

#endif