#include <exception>
#include <iostream>
#include <string>

#include "scheduler.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
        cerr << "Usage: " << argv[0] << " <input_file> <time_quantum> [html_output_file]\n";
        return 1;
    }

    const string input_file = argv[1];
    const string html_output_path = (argc == 4) ? argv[3] : "";

    int time_quantum = 0;
    try {
        time_quantum = stoi(argv[2]);
    } catch (const exception&) {
        cerr << "Error: time_quantum must be a valid integer.\n";
        return 1;
    }

    if (time_quantum <= 0) {
        cerr << "Error: time_quantum must be greater than zero.\n";
        return 1;
    }

    try {
        const vector<Process> processes = readProcessesFromFile(input_file);
        runAll(processes, time_quantum, html_output_path);
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}