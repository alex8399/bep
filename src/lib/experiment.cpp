#include "experiment.hpp"

#include "base_boruvka_mst_solver.hpp"
#include "file_facilities.hpp"
#include "graph.hpp"
#include "timer.hpp"
#include <vector>
#include "experiment_setup.hpp"

void Experiment::run(const std::string &graphFilePath, const std::string &resultsFilePath)
{
    Measurements results;

    Graph graph;
    readGraphFile(graphFilePath, graph);

    for (int ind = 0; ind < m_runNumber; ++ind)
    {
        Graph mst;
        ExperimentSetup experimentSetup;

        experimentSetup.overallTimer.start();
        m_boruvkaMstSolver->calculateMST(graph, mst, experimentSetup);
        experimentSetup.overallTimer.stop();

        results.push_back({
            experimentSetup.overallTimer.getElapsedTime(),
            experimentSetup.edgePhaseTimer.getElapsedTime()
        });
    }

    dumpResults(results, resultsFilePath);
}