#ifndef OPENCL_BORUVKA_MST_SOLVER_HPP
#define OPENCL_BORUVKA_MST_SOLVER_HPP

#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/opencl.hpp>

#include "graph.hpp"
#include "base_boruvka_mst_solver.hpp"
#include "experiment_setup.hpp"
#include <optional>
#include <string>

class OpenCLBoruvkaMSTSolver : public BaseBoruvkaMSTSolver
{
public:
    OpenCLBoruvkaMSTSolver(std::optional<std::string> platformName);

    OpenCLBoruvkaMSTSolver(std::string platformName)
        : OpenCLBoruvkaMSTSolver(std::optional<std::string>{std::move(platformName)}) {}

    OpenCLBoruvkaMSTSolver() : OpenCLBoruvkaMSTSolver(std::nullopt) {}

    void calculateMST(const Graph &graph, Graph &mst, ExperimentSetup &experimentSetup) override;

private:
    int m_blockSize = 256;

    cl::Device m_device;
    cl::Context m_context;
    cl::CommandQueue m_queue;
    cl::Program m_program;
    cl::Kernel m_kernel;
};

#endif // OPENCL_BORUVKA_MST_SOLVER_HPP
