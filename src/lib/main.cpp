#include <stdexcept>
#include <string>
#include "base_boruvka_mst_solver.hpp"
#include "sequential_boruvka_mst_solver.hpp"
#include "openmp_boruvka_mst_solver.hpp"
#include "stdthread_boruvka_mst_solver.hpp"
#include "cuda_boruvka_mst_solver.hpp"
#include <memory>
#include "experiment.hpp"
#include <iostream>

constexpr int OK = 0;
constexpr int ERROR = 1;

const std::string SEQUENTIAL_BORUVKA_TYPE = "SEQ";
const std::string OPENMP_BORUVKA_TYPE = "OPENMP";
const std::string STDTHREAD_BORUVKA_TYPE = "STDTHREAD";
const std::string CUDA_BORUVKA_TYPE = "CUDA";


static std::unique_ptr<BaseBoruvkaMSTSolver> createBoruvkaMSTSolver(const std::string &arg);


static std::unique_ptr<BaseBoruvkaMSTSolver> createBoruvkaMSTSolver(const std::string &arg)
{
    if (arg == SEQUENTIAL_BORUVKA_TYPE)
    {
        return std::make_unique<SequentialBoruvkaMSTSolver>();
    }
    else if (arg == OPENMP_BORUVKA_TYPE)
    {
        return std::make_unique<OpenMPBoruvkaMSTSolver>();
    }
    else if (arg == STDTHREAD_BORUVKA_TYPE)
    {
        return std::make_unique<StdThreadBoruvkaMSTSolver>();
    }
    else if (arg == CUDA_BORUVKA_TYPE)
    {
        return std::make_unique<CudaBoruvkaMSTSolver>();
    }
    else
    {
        throw std::invalid_argument(
            "Algorithm type " + arg + " does not exist\n"
            "Correct types: SEQ OPENMP STDTHREAD");
    }
}

int main(int argc, char* argv[])
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
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return ERROR;
    }
}