<div align="center">

```
 ██████╗██████╗ ██╗   ██╗    ███████╗ ██████╗██╗  ██╗███████╗██████╗
██╔════╝██╔══██╗██║   ██║    ██╔════╝██╔════╝██║  ██║██╔════╝██╔══██╗
██║     ██████╔╝██║   ██║    ███████╗██║     ███████║█████╗  ██║  ██║
██║     ██╔═══╝ ██║   ██║    ╚════██║██║     ██╔══██║██╔══╝  ██║  ██║
╚██████╗██║     ╚██████╔╝    ███████║╚██████╗██║  ██║███████╗██████╔╝
 ╚═════╝╚═╝      ╚═════╝     ╚══════╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚═════╝
```

# ⚡ CPU Scheduling Simulator

**Bringing Operating Systems theory to life — one process at a time.**

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/Build-Makefile%20%7C%20CMake-4CAF50?style=for-the-badge&logo=cmake&logoColor=white)](Makefile)
[![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey?style=for-the-badge&logo=linux&logoColor=white)]()
[![Standard](https://img.shields.io/badge/Standard-C%2B%2B17-blue?style=for-the-badge)]()
[![CI](https://github.com/Hossam-Shehadeh/cpu-scheduling-simulator/actions/workflows/build.yml/badge.svg)](https://github.com/Hossam-Shehadeh/cpu-scheduling-simulator/actions/workflows/build.yml)

<br/>

> A terminal-powered **CPU scheduling simulator** written in C++ that runs three classic OS algorithms side-by-side,
> prints full Gantt charts, and spits out every metric you need — all from a single command.

<br/>

[📖 About](#-about) • [🚀 Features](#-features) • [🧠 Algorithms](#-algorithms) • [⚙️ Build](#%EF%B8%8F-build--run) • [📋 Usage](#-usage) • [📊 Comparison](#-how-do-they-compare) • [📁 Structure](#-project-structure)

</div>

---

## 📖 About

I built this during my **Operating Systems** course to really *understand* scheduling — not just read about it. There's a big difference between memorizing "Round Robin gives fair CPU time" and watching P1 get preempted every 4 ticks while P2 and P3 slip through.

This simulator:
- Reads a process list from a plain text file
- Runs **FCFS**, **SJF**, and **Round Robin** on the same input
- Prints a proper **Gantt chart** for each algorithm
- Shows per-process **completion, turnaround, and waiting times**
- Optionally exports an **interactive HTML Gantt chart**

---

## 🚀 Features

| | Feature | What it does |
|---|---|---|
| 🔁 | **3 Algorithms** | FCFS, SJF (non-preemptive), Round Robin (preemptive) |
| 📊 | **ASCII Gantt Charts** | Visual timeline printed right in your terminal |
| 📈 | **Full Metrics** | Per-process WT, TT, CT — plus averages |
| 🌐 | **HTML Export** | Save an interactive Gantt chart as `.html` |
| 🛠️ | **Dual Build System** | Works with `make` or `cmake` — your choice |
| 💻 | **Cross-Platform** | macOS, Linux, Windows (MSVC / MinGW) |
| 📄 | **Simple Input Format** | One line per process: `PID ArrivalTime BurstTime` |

---

## 🧠 Algorithms

### 1️⃣ FCFS — First Come, First Served

The simplest one. Whoever shows up first, runs first. No interruptions.

```
Timeline:  [ ──────── P1 (24) ──────── ][ P2 (3) ][ P3 (3) ]
           0                            24         27        30
```

| ✅ Pros | ❌ Cons |
|---|---|
| Dead simple to implement | **Convoy Effect** — short jobs get stuck behind long ones |
| Fair in arrival order | High average waiting time when burst times vary a lot |

---

### 2️⃣ SJF — Shortest Job First

Among all processes that have arrived, run the one with the **shortest burst time** next.

```
Timeline:  [ ──────── P1 (24) ──────── ][ P2 (3) ][ P3 (3) ]
           0                            24         27        30
```

> **Note:** With this input, P1 arrives first and is already running when P2/P3 show up —
> so SJF and FCFS produce the same schedule. Try a different input where a short job arrives before a long one starts!

| ✅ Pros | ❌ Cons |
|---|---|
| Optimal average waiting time (theoretically) | Can **starve** long processes indefinitely |
| Works well when burst times are predictable | Requires knowing burst time in advance |

---

### 3️⃣ RR — Round Robin

Every process gets a fixed **time quantum** on the CPU. When time is up, it goes to the back of the queue.

```
Quantum = 4

 Slice:   1    2    3    4    5    6    7    8
         ┌────┬────┬────┬────┬────┬────┬────┬────┐
  CPU:   │ P1 │ P2 │ P3 │ P1 │ P1 │ P1 │ P1 │ P1 │
         └────┴────┴────┴────┴────┴────┴────┴────┘
 Time:   0    4    7   10   14   18   22   26   30
         ↑         ↑    ↑
      P1 first   P2   P3
      preempted  done done

  P1 remaining:  24 → 20 → 20 → 20 → 16 → 12 → 8 → 4 → 0  ✓
  P2 remaining:   3 →  3 →  0  ✓
  P3 remaining:   3 →  3 →  3 →  0  ✓
```

| ✅ Pros | ❌ Cons |
|---|---|
| **Fair** — every process gets CPU time regularly | More context switches = more overhead |
| Best **response time** for interactive workloads | Avg turnaround can be worse than SJF |
| Quantum size lets you tune the trade-off | Choosing the right quantum is non-trivial |

---

## ⚙️ Build & Run

### Prerequisites

- A C++17 compiler — `g++`, `clang++`, or MSVC
- `make` **or** `cmake` (either works)

---

### 🍎 macOS / 🐧 Linux

```bash
# Clone
git clone https://github.com/Hossam-Shehadeh/cpu-scheduling-simulator.git
cd cpu-scheduling-simulator

# Build with make
make

# Run the basic example
./scheduler examples/basic.txt 4

# Or: build + run in one command
make run
```

Or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/scheduler sample_input.txt 4
```

One-liner helper script:

```bash
chmod +x run_mac.sh
./run_mac.sh sample_input.txt 4
```

---

### 🪟 Windows

```bat
cmake -S . -B build
cmake --build build --config Release
build\Release\scheduler.exe sample_input.txt 4
```

Or use the helper:

```bat
run_windows.bat sample_input.txt 4
```

---

### 🧹 Clean up

```bash
make clean
```

---

## 📋 Usage

```
./scheduler <input_file> <time_quantum> [html_output_file]
```

| Argument | Required | Description |
|---|---|---|
| `input_file` | ✅ | Path to your process list |
| `time_quantum` | ✅ | Time slice for Round Robin (must be > 0) |
| `html_output_file` | ❌ | If provided, saves an interactive HTML Gantt chart here |

**Examples:**

```bash
./scheduler sample_input.txt 4
./scheduler sample_input.txt 2 output.html
```

---

## 📄 Input Format

One process per line:

```
<PID>  <ArrivalTime>  <BurstTime>
```

**`sample_input.txt`:**

```
1  0  24
2  1   3
3  2   3
```

Blank lines and `#` comments are skipped.

---

## 📟 Sample Terminal Output

<details>
<summary><b>▶ Click to expand — FCFS output</b></summary>

```
========================
--- FCFS ---
========================

Execution Trace:
[Time  0] Process 1 started
[Time 24] Process 1 finished
[Time 24] Process 2 started
[Time 27] Process 2 finished
[Time 27] Process 3 started
[Time 30] Process 3 finished

Gantt Chart:
+----------------------------+---------+---------+
|             P1             |   P2    |   P3    |
|            d=24            |   d=3   |   d=3   |
+----------------------------+---------+---------+
|----------------------------|---------|---------|----
0                            24        27        30

Metrics Table:
PID   AT    BT    CT    TT    WT
1     0     24    24    24    0
2     1      3    27    26    23
3     2      3    30    28    25

Average Turnaround Time: 26.00
Average Waiting Time:    16.00
```

</details>

<details>
<summary><b>▶ Click to expand — SJF output</b></summary>

```
========================
--- SJF ---
========================

Execution Trace:
[Time  0] Process 1 started        ← P1 is the only one here at t=0
[Time 24] Process 1 finished
[Time 24] Process 2 started        ← P2 burst=3, chosen over P3 burst=3 (same, picks lower PID)
[Time 27] Process 2 finished
[Time 27] Process 3 started
[Time 30] Process 3 finished

Gantt Chart:
+----------------------------+---------+---------+
|             P1             |   P2    |   P3    |
|            d=24            |   d=3   |   d=3   |
+----------------------------+---------+---------+
|----------------------------|---------|---------|----
0                            24        27        30

Metrics Table:
PID   AT    BT    CT    TT    WT
1     0     24    24    24    0
2     1      3    27    26    23
3     2      3    30    28    25

Average Turnaround Time: 26.00
Average Waiting Time:    16.00

  ⚠ Same result as FCFS here because P1 is the only
    process available at t=0 — SJF has no choice.
```

</details>

<details>
<summary><b>▶ Click to expand — Round Robin output (q=4)</b></summary>

```
========================
--- RR (quantum=4) ---
========================

Execution Trace:
[Time  0] Process 1 started
[Time  4] Process 1 preempted      ← used its full quantum, back to queue
[Time  4] Process 2 started
[Time  7] Process 2 finished       ← only needed 3 ticks, done!
[Time  7] Process 3 started
[Time 10] Process 3 finished       ← only needed 3 ticks, done!
[Time 10] Process 1 resumed
[Time 14] Process 1 preempted
[Time 14] Process 1 resumed
[Time 18] Process 1 preempted
[Time 18] Process 1 resumed
[Time 22] Process 1 preempted
[Time 22] Process 1 resumed
[Time 26] Process 1 preempted
[Time 26] Process 1 resumed
[Time 30] Process 1 finished       ← finally done after 6 turns

Gantt Chart:
+------+-----+-----+------+------+------+------+------+
|  P1  |  P2 |  P3 |  P1  |  P1  |  P1  |  P1  |  P1  |
| d=4  | d=3 | d=3 | d=4  | d=4  | d=4  | d=4  | d=4  |
+------+-----+-----+------+------+------+------+------+
0      4     7    10     14     18     22     26     30

Metrics Table:
PID   AT    BT    CT    TT    WT
1     0     24    30    30     6     ← waited only 6 units total
2     1      3     7     6     3     ← finished super early!
3     2      3    10     8     5     ← finished super early!

Average Turnaround Time: 14.67     ← 11 units better than FCFS/SJF
Average Waiting Time:     4.67     ← 11 units better than FCFS/SJF
```

</details>

---

## 📊 How Do They Compare?

Using `sample_input.txt` with `quantum = 4`:

```
                    FCFS     SJF      Round Robin (q=4)
                 ┌──────────────────────────────────────────┐
Avg Wait  (WT)   │  16.00   ████████████████████            │
                 │  16.00   ████████████████████            │
                 │   4.67   ██████                ← winner  │
                 └──────────────────────────────────────────┘

                    FCFS     SJF      Round Robin (q=4)
                 ┌──────────────────────────────────────────┐
Avg Turn  (TT)   │  26.00   ██████████████████████████      │
                 │  26.00   ██████████████████████████      │
                 │  14.67   ███████████████       ← winner  │
                 └──────────────────────────────────────────┘
```

| Metric | 🐢 FCFS | 🎯 SJF | 🔄 Round Robin (q=4) |
|---|:---:|:---:|:---:|
| **Avg Waiting Time** | 16.00 | 16.00 | **4.67** ✅ |
| **Avg Turnaround Time** | 26.00 | 26.00 | **14.67** ✅ |
| Preemptive? | ❌ | ❌ | ✅ |
| Starvation Risk | Low | ⚠️ High | None |
| Best for | Batch jobs | Short predictable tasks | Interactive / time-sharing |

> **Why does RR win here?** P1 has a massive burst (24 units). FCFS and SJF both let it hog the CPU before P2 and P3 get a turn. RR breaks P1 into 4-unit slices, letting P2 finish at t=7 and P3 at t=10 — dramatically cutting their waiting time.

---

## 🧩 How It Works Inside

```
main.cpp
   │
   ├── readProcessesFromFile()      parse input file → vector<Process>
   │
   └── runAll()
         ├── runFCFS()              sort by arrival_time → run in order
         ├── runSJF()               at each step, pick lowest burst_time among arrived
         └── runRR()                ready queue + countdown per quantum
               │
               └── printMetrics()  TT = CT - AT,  WT = TT - BT,  print table + avg
```

Each `Process` carries:
- **Input:** `pid`, `arrival_time`, `burst_time`
- **Runtime:** `remaining_time` (decremented each tick in RR)
- **Output:** `completion_time` (stamped when process finishes)

Metric formulas:

```
Turnaround Time  =  Completion Time  −  Arrival Time
Waiting Time     =  Turnaround Time  −  Burst Time
```

---

## 📁 Project Structure

```
cpu-scheduling-simulator/
│
├── src/                       source files
│   ├── main.cpp               entry point, argument parsing, error handling
│   ├── scheduler.cpp          FCFS, SJF, RR algorithms + Gantt chart printer
│   └── process.cpp            file reader → parses lines into Process objects
│
├── include/                   headers
│   ├── scheduler.h            function declarations for all three algorithms
│   └── process.h              Process struct definition
│
├── examples/                  ready-to-use input files
│   ├── basic.txt              classic 3-process textbook example
│   ├── convoy_effect.txt      one long job blocks many short ones (FCFS worst case)
│   ├── starvation_demo.txt    short jobs keep starving a long one under SJF
│   └── balanced.txt           varied burst times — all three algorithms differ
│
├── docs/                      in-depth references
│   ├── algorithms.md          pseudocode, edge cases, and trade-off analysis
│   └── metrics.md             how CT, TT, WT, RT are calculated
│
├── tests/
│   └── run_tests.sh           automated test runner (10 tests, colour output)
│
├── .github/
│   ├── workflows/build.yml    CI: builds on Ubuntu, macOS, and Windows on every push
│   ├── ISSUE_TEMPLATE/        bug report + feature request forms
│   └── pull_request_template.md
│
├── Makefile                   quick build: make / make run / make clean
├── CMakeLists.txt             cross-platform build
├── run_mac.sh                 one-command script for macOS (auto-detects cmake/make/g++)
├── run_windows.bat            one-command script for Windows
├── gantt_visual.html          pre-generated HTML Gantt chart example
└── LICENSE
```

---

## 🧭 Running the Test Suite

```bash
bash tests/run_tests.sh
```

Runs 10 automated checks against all four example inputs and validates error handling:

```
▶ Running tests...

  ✓ PASS  basic.txt — FCFS avg waiting time
  ✓ PASS  basic.txt — RR avg waiting time
  ✓ PASS  basic.txt — FCFS section header present
  ✓ PASS  basic.txt — SJF section header present
  ✓ PASS  basic.txt — RR section header present
  ✓ PASS  convoy_effect.txt — P1 completes at 50
  ✓ PASS  balanced.txt — runs without error
  ✓ PASS  starvation_demo.txt — runs without error
  ✓ PASS  quantum=0 correctly rejected
  ✓ PASS  quantum=-1 correctly rejected

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Results: 10 passed / 0 failed / 10 total
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

<div align="center">

*"An OS without a scheduler is just a very expensive paperweight."*

⭐ **If this helped you understand scheduling, drop a star!** ⭐

</div>
