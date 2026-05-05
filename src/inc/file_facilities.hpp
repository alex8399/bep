#ifndef __FILE_FACILITIES_HPP__
#define __FILE_FACILITIES_HPP__

#include "graph.hpp"
#include <string>
#include "timer.hpp"

using Measurements = std::vector<std::vector<TimerTime>>;

void readGraphFile(const std::string &filePath, Graph &graph);
void dumpResults(const Measurements &results, const std::string &filePath);

#endif