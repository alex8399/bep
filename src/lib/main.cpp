#if !defined(GCC) && !defined(NVCC)
    #error "Unknown compiler"
#endif

#include <stdexcept>
#include <string>
#include "base_boruvka_mst_solver.hpp"
#include <memory>
#include "experiment.hpp"
#include <iostream>

#ifdef GCC
    #include "sequential_boruvka_mst_solver_findpivot.hpp"
    #include "sequential_boruvka_mst_solver_findpivotsafely.hpp"
    #include "openmp_boruvka_mst_solver_mapreduce.hpp"
    #include "openmp_boruvka_mst_solver_mutex.hpp"
    #include "stdthread_boruvka_mst_solver.hpp"
    #include "opencl_boruvka_mst_solver.hpp"
#elif NVCC
    #include "cuda_boruvka_mst_solver.hpp"
#endif

constexpr int OK = 0;
constexpr int ERROR = 1;

#ifdef GCC
    const std::string SEQUENTIAL_BORUVKA_TYPE_FINDPIVOT = "SEQ-FINDPIVOT";
    const std::string SEQUENTIAL_BORUVKA_TYPE_FINDPIVOTSAFELY = "SEQ-FINDPIVOTSAFELY";
    const std::string OPENMP_BORUVKA_TYPE_MAPREDUCE = "OPENMP-MAPREDUCE";
    const std::string OPENMP_BORUVKA_TYPE_MUTEX = "OPENMP-MUTEX";
    const std::string STDTHREAD_BORUVKA_TYPE = "STDTHREAD";
    const std::string OPENCL_BORUVKA_TYPE = "OPENCL";
#elif NVCC
    const std::string CUDA_BORUVKA_TYPE = "CUDA";
#endif

static std::unique_ptr<BaseBoruvkaMSTSolver> createBoruvkaMSTSolver(const std::string &arg);

static std::unique_ptr<BaseBoruvkaMSTSolver> createBoruvkaMSTSolver(const std::string &arg)
{
    if (false)
    {
    }
    #ifdef GCC
    else if (arg == SEQUENTIAL_BORUVKA_TYPE_FINDPIVOT)
    {
        return std::make_unique<SequentialBoruvkaMSTSolverWithFindPivot>();
    }
    else if (arg == SEQUENTIAL_BORUVKA_TYPE_FINDPIVOTSAFELY)
    {
        return std::make_unique<SequentialBoruvkaMSTSolverWithFindPivotSafely>();
    }
    else if (arg == OPENMP_BORUVKA_TYPE_MAPREDUCE)
    {
        return std::make_unique<OpenMPBoruvkaMSTSolverMapReduce>();
    }
    else if (arg == OPENMP_BORUVKA_TYPE_MUTEX)
    {
        return std::make_unique<OpenMPBoruvkaMSTSolverMutex>();
    }
    else if (arg == OPENCL_BORUVKA_TYPE)
    {
        return std::make_unique<OpenCLBoruvkaMSTSolver>();
    }
    else if (arg == STDTHREAD_BORUVKA_TYPE)
    {
        return std::make_unique<StdThreadBoruvkaMSTSolver>();
    }
    #elif NVCC
    else if (arg == CUDA_BORUVKA_TYPE)
    {
        return std::make_unique<CudaBoruvkaMSTSolver>();
    }
    #endif
    else
    {
        throw std::invalid_argument(
            "Algorithm type " + arg + " does not exist\n");
    }
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 5)
        {
            throw std::invalid_argument(
                "Incorrect arguments number\n"
                "Correct usage: ./experiment.exe algorithm_type run_number path_to_graph_file path_to_results_file");
        }

        auto solver = createBoruvkaMSTSolver(argv[1]);
        const int runNumber = std::stoi(argv[2]);

        Experiment experiment(solver, runNumber);
        experiment.run(argv[3], argv[4]);

        return OK;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return ERROR;
    }
}