#ifndef OPENCL_BORUVKA_MST_SOLVER_HPP
#define OPENCL_BORUVKA_MST_SOLVER_HPP

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"
#include <optional>
#include <string>

class OpenCLBoruvkaMSTSolver : public BaseBoruvkaMSTSolver
{
public:
    // explicit OpenCLBoruvkaMSTSolver(int blockSize = 256)
    //     : m_blockSize(blockSize) {}

    OpenCLBoruvkaMSTSolver(): m_platformName(std::nullopt) {}
    OpenCLBoruvkaMSTSolver(std::string platformName): m_platformName(platformName) {}

    void calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup) const;

private:
    int m_blockSize = 256;
    std::optional<std::string> m_platformName = std::nullopt;
};

#endif // OPENCL_BORUVKA_MST_SOLVER_HPP
