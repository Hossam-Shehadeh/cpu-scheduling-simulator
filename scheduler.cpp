#include "scheduler.h"

using namespace std;

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <stdexcept>
#include <sstream>

namespace {

struct Segment {
    string label;
    int start_time;
    int end_time;
};

struct SimulationResult {
    vector<Process> processes;
    vector<string> trace_lines;
    vector<Segment> segments;
};

struct AverageMetrics {
    double turnaround;
    double waiting;
};

void printHeader(const string& name) {
    cout << "========================\n";
    cout << "--- " << name << " ---\n";
    cout << "========================\n\n";
}

string makeTraceLine(int time, const string& label, const string& action) {
    return "[Time " + to_string(time) + "] " + label + " " + action;
}

string processLabel(int pid) {
    return "Process " + to_string(pid);
}

void printGanttChart(const vector<Segment>& segments) {
    if (segments.empty()) {
        return;
    }

    cout << "\nGantt Chart:\n";

    auto centerText = [](const string& text, int width) -> string {
        if (static_cast<int>(text.size()) >= width) {
            return text;
        }
        const int total_padding = width - static_cast<int>(text.size());
        const int left_padding = total_padding / 2;
        const int right_padding = total_padding - left_padding;
        return string(static_cast<size_t>(left_padding), ' ') +
               text +
               string(static_cast<size_t>(right_padding), ' ');
    };

    vector<int> cell_widths;
    cell_widths.reserve(segments.size());

    string top_border = "+";
    string process_row = "|";
    string duration_row = "|";

    for (const Segment& segment : segments) {
        const int duration = max(1, segment.end_time - segment.start_time);
        const string duration_text = "d=" + to_string(duration);
        const bool is_idle = (segment.label == "Idle");
        const string display_label = is_idle ? "[IDLE]" : segment.label;

        // Proportional width based on duration, with a floor for readability.
        int width = duration * 3;
        width = max(width, static_cast<int>(display_label.size()) + 4);
        width = max(width, static_cast<int>(duration_text.size()) + 4);
        width = max(width, 9);
        width = min(width, 28);

        cell_widths.push_back(width);
        top_border += string(static_cast<size_t>(width), '-') + "+";
        process_row += centerText(display_label, width) + "|";
        duration_row += centerText(duration_text, width) + "|";
    }

    cout << top_border << '\n';
    cout << process_row << '\n';
    cout << duration_row << '\n';
    cout << top_border << '\n';

    // Build an ASCII ruler aligned with segment boundaries.
    const int chart_width = static_cast<int>(top_border.size());
    string ruler(static_cast<size_t>(chart_width), ' ');
    string marks(static_cast<size_t>(chart_width), ' ');

    int cursor = 0;
    for (size_t i = 0; i < segments.size(); ++i) {
        ruler[static_cast<size_t>(cursor)] = '|';

        const string time_text = to_string(segments[i].start_time);
        for (size_t j = 0; j < time_text.size() && cursor + static_cast<int>(j) < chart_width; ++j) {
            marks[static_cast<size_t>(cursor + static_cast<int>(j))] = time_text[j];
        }

        cursor += cell_widths[i] + 1;
        for (int fill = cursor - cell_widths[i]; fill < cursor; ++fill) {
            if (fill > 0 && fill < chart_width && ruler[static_cast<size_t>(fill)] != '|') {
                ruler[static_cast<size_t>(fill)] = '-';
            }
        }
    }

    if (cursor < chart_width) {
        ruler[static_cast<size_t>(cursor)] = '|';
    }

    const string final_time_text = to_string(segments.back().end_time);
    int final_start = cursor;
    if (final_start + static_cast<int>(final_time_text.size()) > chart_width) {
        final_start = chart_width - static_cast<int>(final_time_text.size());
    }
    final_start = max(0, final_start);
    for (size_t j = 0; j < final_time_text.size() && final_start + static_cast<int>(j) < chart_width; ++j) {
        marks[static_cast<size_t>(final_start + static_cast<int>(j))] = final_time_text[j];
    }

    cout << ruler << '\n';
    cout << marks << '\n';
}

void printCompletionMetrics(const vector<Process>& processes) {
    cout << "\nMetrics Table:\n";
    cout << left
              << setw(6) << "PID"
              << setw(6) << "AT"
              << setw(6) << "BT"
              << setw(6) << "CT"
              << setw(6) << "TT"
              << setw(6) << "WT"
              << '\n';

    vector<Process> sorted = processes;
    sort(sorted.begin(), sorted.end(), [](const Process& left, const Process& right) {
        return left.pid < right.pid;
    });

    double total_turnaround = 0.0;
    double total_waiting = 0.0;

    for (const Process& process : sorted) {
        const int turnaround_time = process.completion_time - process.arrival_time;
        const int waiting_time = turnaround_time - process.burst_time;

        total_turnaround += static_cast<double>(turnaround_time);
        total_waiting += static_cast<double>(waiting_time);

        cout << left
                  << setw(6) << process.pid
                  << setw(6) << process.arrival_time
                  << setw(6) << process.burst_time
                  << setw(6) << process.completion_time
                  << setw(6) << turnaround_time
                  << setw(6) << waiting_time
                  << '\n';
    }

    const double count = static_cast<double>(sorted.size());
    cout << fixed << setprecision(2);
    cout << "\nFinal:\n";
    cout << "Average Turnaround Time: " << (total_turnaround / count) << '\n';
    cout << "Average Waiting Time: " << (total_waiting / count) << '\n';
    cout.unsetf(ios::floatfield);
}

AverageMetrics calculateAverageMetrics(const vector<Process>& processes) {
    double total_turnaround = 0.0;
    double total_waiting = 0.0;

    for (const Process& process : processes) {
        const int turnaround_time = process.completion_time - process.arrival_time;
        const int waiting_time = turnaround_time - process.burst_time;
        total_turnaround += static_cast<double>(turnaround_time);
        total_waiting += static_cast<double>(waiting_time);
    }

    const double count = static_cast<double>(processes.size());
    return {total_turnaround / count, total_waiting / count};
}

string escapeJsString(const string& value) {
    string escaped;
    escaped.reserve(value.size());
    for (char character : value) {
        if (character == '\\' || character == '"') {
            escaped.push_back('\\');
        }
        escaped.push_back(character);
    }
    return escaped;
}

string segmentsToJsRows(const vector<Segment>& segments) {
    ostringstream stream;
    for (size_t i = 0; i < segments.size(); ++i) {
        const Segment& segment = segments[i];
        stream << "[\"" << escapeJsString(segment.label) << "\", "
               << segment.start_time << ", " << segment.end_time << "]";
        if (i + 1 < segments.size()) {
            stream << ",\n        ";
        }
    }
    return stream.str();
}

void writeVisualizationHtml(
    const string& output_file,
    const SimulationResult& fcfs,
    const SimulationResult& sjf,
    const SimulationResult& rr
) {
    ofstream html(output_file);
    if (!html) {
        throw runtime_error("Unable to create HTML visualization file: " + output_file);
    }

    const AverageMetrics fcfs_avg = calculateAverageMetrics(fcfs.processes);
    const AverageMetrics sjf_avg = calculateAverageMetrics(sjf.processes);
    const AverageMetrics rr_avg = calculateAverageMetrics(rr.processes);

    html << "<!DOCTYPE html>\n"
         << "<html lang=\"en\">\n"
         << "<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
         << "  <title>CPU Scheduling Visualization</title>\n"
         << "  <script src=\"https://www.gstatic.com/charts/loader.js\"></script>\n"
         << "  <style>\n"
         << "    :root {\n"
         << "      --bg: #f3f6fb;\n"
         << "      --card: #ffffff;\n"
         << "      --ink: #1b2733;\n"
         << "      --muted: #607080;\n"
         << "      --accent: #1f7a8c;\n"
         << "      --border: #d8e1ea;\n"
         << "    }\n"
         << "    body {\n"
         << "      margin: 0;\n"
         << "      font-family: 'Trebuchet MS', 'Segoe UI', sans-serif;\n"
         << "      color: var(--ink);\n"
         << "      background: radial-gradient(circle at top left, #ffffff, var(--bg));\n"
         << "    }\n"
         << "    .wrap {\n"
         << "      max-width: 1100px;\n"
         << "      margin: 24px auto;\n"
         << "      padding: 0 16px 24px;\n"
         << "    }\n"
         << "    .hero {\n"
         << "      background: linear-gradient(135deg, #0d3b66, #1f7a8c);\n"
         << "      color: #fff;\n"
         << "      border-radius: 16px;\n"
         << "      padding: 18px 22px;\n"
         << "      margin-bottom: 16px;\n"
         << "      box-shadow: 0 8px 20px rgba(13, 59, 102, 0.18);\n"
         << "    }\n"
         << "    .hero h1 { margin: 0; font-size: 1.5rem; }\n"
         << "    .hero p { margin: 6px 0 0; color: #dce8f5; }\n"
         << "    .cards {\n"
         << "      display: grid;\n"
         << "      grid-template-columns: repeat(auto-fit, minmax(210px, 1fr));\n"
         << "      gap: 12px;\n"
         << "      margin-bottom: 14px;\n"
         << "    }\n"
         << "    .card {\n"
         << "      background: var(--card);\n"
         << "      border: 1px solid var(--border);\n"
         << "      border-radius: 12px;\n"
         << "      padding: 12px;\n"
         << "    }\n"
         << "    .card h3 { margin: 0 0 6px; color: var(--accent); }\n"
         << "    .metric { margin: 3px 0; color: var(--muted); }\n"
         << "    .chart-card {\n"
         << "      background: var(--card);\n"
         << "      border: 1px solid var(--border);\n"
         << "      border-radius: 12px;\n"
         << "      margin-top: 12px;\n"
         << "      padding: 10px;\n"
         << "    }\n"
         << "    .chart-title { margin: 4px 0 8px 2px; font-weight: 700; color: #0d3b66; }\n"
         << "    .chart { width: 100%; height: 230px; }\n"
         << "    .note {\n"
         << "      margin-top: 14px;\n"
         << "      color: var(--muted);\n"
         << "      font-size: 0.92rem;\n"
         << "    }\n"
         << "  </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "  <div class=\"wrap\">\n"
         << "    <div class=\"hero\">\n"
         << "      <h1>CPU Scheduling Timeline Dashboard</h1>\n"
         << "      <p>Visual Gantt charts generated from your simulator (Google Charts Timeline).</p>\n"
         << "    </div>\n"
         << "\n"
         << "    <div class=\"cards\">\n"
         << "      <div class=\"card\"><h3>FCFS</h3><div class=\"metric\">Avg TT: " << fixed << setprecision(2) << fcfs_avg.turnaround << "</div><div class=\"metric\">Avg WT: " << fcfs_avg.waiting << "</div></div>\n"
         << "      <div class=\"card\"><h3>SJF</h3><div class=\"metric\">Avg TT: " << sjf_avg.turnaround << "</div><div class=\"metric\">Avg WT: " << sjf_avg.waiting << "</div></div>\n"
         << "      <div class=\"card\"><h3>RR</h3><div class=\"metric\">Avg TT: " << rr_avg.turnaround << "</div><div class=\"metric\">Avg WT: " << rr_avg.waiting << "</div></div>\n"
         << "    </div>\n"
         << "\n"
         << "    <div class=\"chart-card\"><div class=\"chart-title\">FCFS</div><div id=\"chart_fcfs\" class=\"chart\"></div></div>\n"
         << "    <div class=\"chart-card\"><div class=\"chart-title\">SJF</div><div id=\"chart_sjf\" class=\"chart\"></div></div>\n"
         << "    <div class=\"chart-card\"><div class=\"chart-title\">Round Robin</div><div id=\"chart_rr\" class=\"chart\"></div></div>\n"
         << "\n"
         << "    <div class=\"note\">Tip: Idle periods are shown as the row label <b>Idle</b>. Time values are simulation units.</div>\n"
         << "  </div>\n"
         << "\n"
         << "  <script>\n"
         << "    const fcfsRows = [\n        " << segmentsToJsRows(fcfs.segments) << "\n    ];\n"
         << "    const sjfRows = [\n        " << segmentsToJsRows(sjf.segments) << "\n    ];\n"
         << "    const rrRows = [\n        " << segmentsToJsRows(rr.segments) << "\n    ];\n"
         << "\n"
         << "    google.charts.load('current', { packages: ['timeline'] });\n"
         << "    google.charts.setOnLoadCallback(drawAll);\n"
         << "\n"
         << "    function drawTimeline(containerId, rows) {\n"
         << "      const container = document.getElementById(containerId);\n"
         << "      const chart = new google.visualization.Timeline(container);\n"
         << "      const data = new google.visualization.DataTable();\n"
         << "      data.addColumn({ type: 'string', id: 'Process' });\n"
         << "      data.addColumn({ type: 'string', id: 'Label' });\n"
         << "      data.addColumn({ type: 'date', id: 'Start' });\n"
         << "      data.addColumn({ type: 'date', id: 'End' });\n"
         << "\n"
         << "      data.addRows(rows.map((item) => [\n"
         << "        item[0],\n"
         << "        item[0],\n"
         << "        new Date(item[1] * 1000),\n"
         << "        new Date(item[2] * 1000)\n"
         << "      ]));\n"
         << "\n"
         << "      chart.draw(data, {\n"
         << "        timeline: { showRowLabels: true, showBarLabels: true },\n"
         << "        avoidOverlappingGridLines: false\n"
         << "      });\n"
         << "    }\n"
         << "\n"
         << "    function drawAll() {\n"
         << "      drawTimeline('chart_fcfs', fcfsRows);\n"
         << "      drawTimeline('chart_sjf', sjfRows);\n"
         << "      drawTimeline('chart_rr', rrRows);\n"
         << "    }\n"
         << "\n"
         << "    window.addEventListener('resize', drawAll);\n"
         << "  </script>\n"
         << "</body>\n"
         << "</html>\n";
}

SimulationResult simulateFCFS(vector<Process> processes) {
    SimulationResult result;
    result.processes = move(processes);

    sort(result.processes.begin(), result.processes.end(), [](const Process& left, const Process& right) {
        if (left.arrival_time != right.arrival_time) {
            return left.arrival_time < right.arrival_time;
        }
        return left.pid < right.pid;
    });

    int current_time = 0;
    for (Process& process : result.processes) {
        if (current_time < process.arrival_time) {
            result.segments.push_back({"Idle", current_time, process.arrival_time});
            current_time = process.arrival_time;
        }

        result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "started"));
        result.segments.push_back({"P" + to_string(process.pid), current_time, current_time + process.burst_time});
        current_time += process.burst_time;
        process.completion_time = current_time;
        result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "finished"));
    }

    return result;
}

SimulationResult simulateSJF(vector<Process> processes) {
    SimulationResult result;
    result.processes = move(processes);

    sort(result.processes.begin(), result.processes.end(), [](const Process& left, const Process& right) {
        if (left.arrival_time != right.arrival_time) {
            return left.arrival_time < right.arrival_time;
        }
        return left.pid < right.pid;
    });

    int current_time = 0;
    size_t next_arrival_index = 0;
    vector<size_t> ready_indices;
    size_t completed = 0;

    auto pickBestIndex = [&]() -> size_t {
        return *min_element(ready_indices.begin(), ready_indices.end(), [&](size_t left_index, size_t right_index) {
            const Process& left = result.processes[left_index];
            const Process& right = result.processes[right_index];

            if (left.burst_time != right.burst_time) {
                return left.burst_time < right.burst_time;
            }
            if (left.arrival_time != right.arrival_time) {
                return left.arrival_time < right.arrival_time;
            }
            return left.pid < right.pid;
        });
    };

    while (completed < result.processes.size()) {
        while (next_arrival_index < result.processes.size() && result.processes[next_arrival_index].arrival_time <= current_time) {
            ready_indices.push_back(next_arrival_index);
            ++next_arrival_index;
        }

        if (ready_indices.empty()) {
            const int next_arrival_time = result.processes[next_arrival_index].arrival_time;
            result.segments.push_back({"Idle", current_time, next_arrival_time});
            current_time = next_arrival_time;
            continue;
        }

        const size_t best_index = pickBestIndex();
        const auto erase_it = find(ready_indices.begin(), ready_indices.end(), best_index);
        if (erase_it != ready_indices.end()) {
            ready_indices.erase(erase_it);
        }

        Process& process = result.processes[best_index];
        result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "started"));
        result.segments.push_back({"P" + to_string(process.pid), current_time, current_time + process.burst_time});

        current_time += process.burst_time;
        process.completion_time = current_time;
        ++completed;

        result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "finished"));
    }

    return result;
}

SimulationResult simulateRR(vector<Process> processes, int time_quantum) {
    SimulationResult result;
    result.processes = move(processes);

    sort(result.processes.begin(), result.processes.end(), [](const Process& left, const Process& right) {
        if (left.arrival_time != right.arrival_time) {
            return left.arrival_time < right.arrival_time;
        }
        return left.pid < right.pid;
    });

    queue<size_t> ready_queue;
    size_t next_arrival_index = 0;
    size_t completed = 0;
    int current_time = 0;

    auto enqueueArrivalsUpTo = [&](int time_point) {
        while (next_arrival_index < result.processes.size() && result.processes[next_arrival_index].arrival_time <= time_point) {
            ready_queue.push(next_arrival_index);
            ++next_arrival_index;
        }
    };

    enqueueArrivalsUpTo(current_time);

    while (completed < result.processes.size()) {
        if (ready_queue.empty()) {
            if (next_arrival_index >= result.processes.size()) {
                break;
            }

            const int next_arrival_time = result.processes[next_arrival_index].arrival_time;
            if (current_time < next_arrival_time) {
                result.segments.push_back({"Idle", current_time, next_arrival_time});
                current_time = next_arrival_time;
            }

            enqueueArrivalsUpTo(current_time);
            continue;
        }

        const size_t index = ready_queue.front();
        ready_queue.pop();

        Process& process = result.processes[index];
        if (process.remaining_time <= 0) {
            continue;
        }

        result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "started"));

        const int run_time = min(time_quantum, process.remaining_time);
        const int start_time = current_time;
        const int end_time = current_time + run_time;
        result.segments.push_back({"P" + to_string(process.pid), start_time, end_time});

        current_time = end_time;
        process.remaining_time -= run_time;

        enqueueArrivalsUpTo(current_time);

        if (process.remaining_time == 0) {
            process.completion_time = current_time;
            ++completed;
            result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "finished"));
        } else {
            result.trace_lines.push_back(makeTraceLine(current_time, processLabel(process.pid), "preempted"));
            ready_queue.push(index);
        }
    }

    return result;
}

void printAlgorithmResult(const string& name, const SimulationResult& result) {
    printHeader(name);
    cout << "Execution Trace:\n";
    for (const string& line : result.trace_lines) {
        cout << line << '\n';
    }
    printGanttChart(result.segments);
    printCompletionMetrics(result.processes);
    cout << '\n';
}

} // namespace

vector<Process> readProcessesFromFile(const string& input_file) {
    ifstream file(input_file);
    if (!file) {
        throw runtime_error("Unable to open input file: " + input_file);
    }

    vector<Process> processes;
    string line;

    while (getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        istringstream stream(line);
        int pid = 0;
        int arrival_time = 0;
        int burst_time = 0;

        if (!(stream >> pid >> arrival_time >> burst_time)) {
            throw runtime_error("Invalid line in input file: " + line);
        }

        processes.emplace_back(pid, arrival_time, burst_time);
    }

    if (processes.empty()) {
        throw runtime_error("Input file contains no processes.");
    }

    return processes;
}

void printMetrics(const vector<Process>& processes) {
    printCompletionMetrics(processes);
}

void runFCFS(const vector<Process>& processes) {
    const SimulationResult result = simulateFCFS(processes);
    printAlgorithmResult("FCFS", result);
}

void runSJF(const vector<Process>& processes) {
    const SimulationResult result = simulateSJF(processes);
    printAlgorithmResult("SJF", result);
}

void runRR(const vector<Process>& processes, int time_quantum) {
    const SimulationResult result = simulateRR(processes, time_quantum);
    printAlgorithmResult("RR", result);
}

void runAll(const vector<Process>& processes, int time_quantum, const string& html_output_path) {
    const SimulationResult fcfs_result = simulateFCFS(processes);
    const SimulationResult sjf_result = simulateSJF(processes);
    const SimulationResult rr_result = simulateRR(processes, time_quantum);

    printAlgorithmResult("FCFS", fcfs_result);
    printAlgorithmResult("SJF", sjf_result);
    printAlgorithmResult("RR", rr_result);

    if (!html_output_path.empty()) {
        writeVisualizationHtml(html_output_path, fcfs_result, sjf_result, rr_result);
        cout << "HTML visualization generated: " << html_output_path << "\n";
    }
}