#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;

    Process()
        : pid(0), arrival_time(0), burst_time(0), remaining_time(0), completion_time(-1) {}

    Process(int process_id, int arrival, int burst)
        : pid(process_id), arrival_time(arrival), burst_time(burst),
          remaining_time(burst), completion_time(-1) {}

    void reset() {
        remaining_time = burst_time;
        completion_time = -1;
    }
};

#endif