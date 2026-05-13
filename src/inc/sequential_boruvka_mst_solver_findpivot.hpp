#ifndef __SEQUENTIAL_BORUVKA_MST_SOLVER_FINDPIVOT_HPP__
#define __SEQUENTIAL_BORUVKA_MST_SOLVER_FINDPIVOT_HPP__

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"

class SequentialBoruvkaMSTSolverWithFindPivot : public BaseBoruvkaMSTSolver
{
public:
    void calculateMST(const Graph& graph, Graph& mst, ExperimentSetup &experimentSetup) override;
};

#endif