# Scheduling Algorithms — Deep Dive

A detailed reference for the three algorithms implemented in this simulator.

---

## 1. FCFS — First Come, First Served

### How it works

Processes are sorted by **arrival time** and executed one after another, with no interruption.

```
ready queue: [ P1(t=0) → P2(t=1) → P3(t=2) ]

Step 1: CPU is free at t=0. Pick P1 (arrived first). Run for 24 ticks.
Step 2: CPU is free at t=24. Pick P2. Run for 3 ticks.
Step 3: CPU is free at t=27. Pick P3. Run for 3 ticks.
Done at t=30.
```

### Implementation (pseudocode)

```
sort processes by arrival_time
time = 0
for each process p in sorted order:
    if time < p.arrival_time:
        time = p.arrival_time      # CPU idle gap
    p.completion_time = time + p.burst_time
    time = p.completion_time
```

### When to use it

- Batch processing systems where all jobs have similar lengths
- Simple embedded systems where fairness-by-arrival is acceptable

### Worst case

The **Convoy Effect**: one long process (CPU-bound) blocks many short ones.  
Example: P1 (burst=50) arrives at t=0. P2–P5 (burst=3 each) arrive at t=1–4.  
P2–P5 all wait ~50 units despite only needing 3 each.

---

## 2. SJF — Shortest Job First

### How it works

At every point where the CPU becomes free, the scheduler looks at **all processes that have already arrived** and picks the one with the **smallest burst time**.

```
ready queue at t=0: [ P1(burst=24) ]  → only P1 available, must run it
ready queue at t=24: [ P2(burst=3), P3(burst=3) ]  → tie: pick lower PID → P2
ready queue at t=27: [ P3(burst=3) ]  → only P3 left
```

### Implementation (pseudocode)

```
time = 0
remaining = all processes (unsorted)
while remaining is not empty:
    arrived = { p in remaining : p.arrival_time <= time }
    if arrived is empty:
        time = min(remaining.arrival_time)   # skip idle time
        continue
    p = process in arrived with smallest burst_time
    p.completion_time = time + p.burst_time
    time = p.completion_time
    remove p from remaining
```

### When to use it

- Batch systems where burst times can be estimated (e.g., from historical data)
- Job scheduling in scientific computing

### Worst case

**Starvation**: if a constant stream of short jobs keeps arriving, a long process may never get scheduled.  
Mitigation: **Aging** — gradually increase a process's priority the longer it waits.

---

## 3. Round Robin

### How it works

Processes are placed in a **circular queue**. Each one runs for at most `quantum` time units. If it doesn't finish, it's preempted and put at the back of the queue.

```
quantum = 4, processes: P1(burst=24), P2(burst=3), P3(burst=3)

t= 0: P1 runs  [0..4]   remaining=20 → back of queue
t= 4: P2 runs  [4..7]   remaining=0  → done ✓
t= 7: P3 runs  [7..10]  remaining=0  → done ✓
t=10: P1 runs  [10..14] remaining=16 → back of queue
t=14: P1 runs  [14..18] remaining=12 → back
t=18: P1 runs  [18..22] remaining=8  → back
t=22: P1 runs  [22..26] remaining=4  → back
t=26: P1 runs  [26..30] remaining=0  → done ✓
```

### Implementation (pseudocode)

```
queue = empty
time = 0
sort processes by arrival_time
index = 0   # pointer into sorted arrival list

while queue is not empty OR index < n:
    # enqueue newly arrived processes
    while index < n AND processes[index].arrival_time <= time:
        queue.push(processes[index++])

    if queue is empty:
        time = processes[index].arrival_time
        continue

    p = queue.pop_front()
    run_time = min(p.remaining_time, quantum)
    time += run_time
    p.remaining_time -= run_time

    # enqueue any new arrivals during this slice
    while index < n AND processes[index].arrival_time <= time:
        queue.push(processes[index++])

    if p.remaining_time > 0:
        queue.push(p)     # not done, re-queue
    else:
        p.completion_time = time   # finished
```

### Choosing the quantum

| Quantum | Effect |
|---|---|
| Very small | Approaches SJF behavior, high context-switch overhead |
| Very large | Degenerates to FCFS |
| Balanced | Sweet spot — good response time, reasonable overhead |

A common rule of thumb: **80% of CPU bursts should be shorter than the quantum**.

---

## Comparison Summary

| Property | FCFS | SJF | Round Robin |
|---|---|---|---|
| Preemptive | No | No | Yes |
| Optimal avg waiting | No | Yes (non-preemptive) | No |
| Starvation possible | No | Yes | No |
| Response time | Poor | Fair | Best |
| Context switches | Minimal | Minimal | High |
| Implementation complexity | Simple | Moderate | Moderate |
