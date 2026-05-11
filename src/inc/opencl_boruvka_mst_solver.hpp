#ifndef OPENCL_BORUVKA_MST_SOLVER_HPP
#define OPENCL_BORUVKA_MST_SOLVER_HPP

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"

class OpenCLBoruvkaMSTSolver : public BaseBoruvkaMSTSolver
{
public:
    explicit OpenCLBoruvkaMSTSolver(int blockSize = 256)
        : m_blockSize(blockSize) {}

    void calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup) const;

private:
    int m_blockSize;
};

#endif // OPENCL_BORUVKA_MST_SOLVER_HPP
