#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>

#include "process.h"

using namespace std;

vector<Process> readProcessesFromFile(const string& input_file);

void runAll(const vector<Process>& processes, int time_quantum, const string& html_output_path = "");

void runFCFS(const vector<Process>& processes);
void runSJF(const vector<Process>& processes);
void runRR(const vector<Process>& processes, int time_quantum);

void printMetrics(const vector<Process>& processes);

#endif