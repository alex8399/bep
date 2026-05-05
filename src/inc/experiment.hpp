#ifndef __EXPERIMENT_HPP__
#define __EXPERIMENT_HPP__

#include "base_boruvka_mst_solver.hpp"
#include <string>
#include <memory>

class Experiment
{
private:
    const std::unique_ptr<BaseBoruvkaMSTSolver> m_boruvkaMstSolver;
    const int m_runNumber;

public:
    Experiment(std::unique_ptr<BaseBoruvkaMSTSolver> &boruvkaMstSolver, const int runNumber) :
        m_boruvkaMstSolver(std::move(boruvkaMstSolver)),
        m_runNumber(runNumber) {};

    void run(const std::string &graphFilePath, const std::string &resultsFilePath);
};

#endif