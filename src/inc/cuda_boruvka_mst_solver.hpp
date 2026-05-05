#ifndef __CUDA_BORUVKA_MST_SOLVER_HPP__
#define __CUDA_BORUVKA_MST_SOLVER_HPP__

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"

class CudaBoruvkaMSTSolver : public BaseBoruvkaMSTSolver
{
public:
    CudaBoruvkaMSTSolver() = default;
    explicit CudaBoruvkaMSTSolver(int blockSize) : m_blockSize(blockSize) {}

    void calculateMST(const Graph& graph, Graph& mst, ExperimentSetup &experimentSetup) const override;
private:
    int m_blockSize = 256;
};

#endif