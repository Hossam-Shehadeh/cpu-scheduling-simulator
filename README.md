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
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

<br/>

> A blazing-fast, terminal-powered **CPU scheduling simulator** that implements  
> three classic OS scheduling algorithms — with Gantt chart output and performance metrics.

<br/>

[📖 About](#-about) • [🚀 Features](#-features) • [🧠 Algorithms](#-algorithms) • [⚙️ Build](#%EF%B8%8F-build--run) • [📋 Usage](#-usage) • [📁 Structure](#-project-structure) • [📜 License](#-license)

</div>

---

## 📖 About

This project was built as part of a university **Operating Systems** course assignment. It simulates how an OS kernel schedules processes onto the CPU using three fundamental scheduling strategies.

The simulator reads a list of processes from a plain text file, runs all three algorithms, and outputs:
- **Gantt chart** timeline for each algorithm
- **Per-process metrics**: waiting time, turnaround time, response time
- **Averages** for quick comparison across algorithms

---

## 🚀 Features

| Feature | Details |
|---|---|
| 🔁 **3 Algorithms** | FCFS, SJF (non-preemptive), Round Robin |
| 📊 **Gantt Charts** | ASCII timeline printed to terminal |
| 📈 **Metrics** | Waiting, turnaround, and completion times |
| 🌐 **HTML Output** | Optional interactive Gantt chart saved as `.html` |
| 🛠️ **Dual Build** | Supports both `make` and `cmake` |
| 💻 **Cross-Platform** | macOS, Linux, Windows (MSVC / MinGW) |
| 📄 **File Input** | Clean `PID ArrivalTime BurstTime` format |

---

## 🧠 Algorithms

### 1️⃣ FCFS — First Come, First Served

> Non-preemptive. Processes are executed in the order they arrive.

```
Timeline:  [ P1 (24) ][ P2 (3) ][ P3 (3) ]
           0          24        27        30
```

- ✅ Simple and fair in arrival order  
- ❌ Suffers from the **Convoy Effect** — short jobs wait behind long ones

---

### 2️⃣ SJF — Shortest Job First

> Non-preemptive. The process with the shortest burst time runs next.

```
Timeline:  [ P1 (24) ][ P3 (3) ][ P2 (3) ]
           0          24        27        30
```

- ✅ Minimizes average waiting time  
- ❌ Can cause **starvation** for long processes  
- ❌ Requires knowing burst times in advance

---

### 3️⃣ RR — Round Robin

> Preemptive. Each process gets a fixed time quantum before being preempted.

```
Quantum = 4
Timeline:  [P1][P2][P3][P1][P1][P1][P1][P1]
           0   4   7  10  14  18  22  26  30
```

- ✅ Fair for all processes  
- ✅ Good response time for interactive systems  
- ❌ Higher context-switch overhead with small quantum

---

## ⚙️ Build & Run

### Prerequisites

- C++17 compatible compiler (`g++`, `clang++`, or MSVC)
- `make` **or** `cmake`

---

### 🐧 macOS / Linux — Using `make`

```bash
# Clone the repo
git clone https://github.com/YOUR_USERNAME/cpu-scheduling-simulator.git
cd cpu-scheduling-simulator

# Build
make

# Run
./scheduler sample_input.txt 4
```

---

### 🐧 macOS / Linux — Using `cmake`

```bash
cmake -S . -B build
cmake --build build
./build/scheduler sample_input.txt 4
```

Or use the helper script:

```bash
chmod +x run_mac.sh
./run_mac.sh sample_input.txt 4
```

---

### 🪟 Windows — Using CMake

```bat
cmake -S . -B build
cmake --build build --config Release
build\Release\scheduler.exe sample_input.txt 4
```

Or use the helper script:

```bat
run_windows.bat sample_input.txt 4
```

---

### 🧹 Clean Build

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
| `input_file` | ✅ | Path to process list (see format below) |
| `time_quantum` | ✅ | Time slice for Round Robin (integer > 0) |
| `html_output_file` | ❌ | Path to save interactive HTML Gantt chart |

### Example

```bash
./scheduler sample_input.txt 4
./scheduler sample_input.txt 4 gantt.html
```

---

## 📄 Input File Format

Each line represents one process:

```
<PID>  <ArrivalTime>  <BurstTime>
```

### Example — `sample_input.txt`

```
1  0  24
2  1   3
3  2   3
```

| Column | Description |
|---|---|
| `PID` | Unique process identifier |
| `ArrivalTime` | When the process arrives in the ready queue |
| `BurstTime` | Total CPU time the process needs |

> Lines starting with `#` and blank lines are ignored.

---

## 📊 Sample Output

```
============================================================
 FCFS — First Come, First Served
============================================================
Gantt Chart:
| P1 | P2 | P3 |
0    24   27   30

PID   Arrival  Burst  Completion  Turnaround  Waiting
  1        0     24          24          24        0
  2        1      3          27          26       23
  3        2      3          30          28       25

Average Turnaround Time : 26.00
Average Waiting Time    :  16.00

============================================================
 SJF — Shortest Job First
============================================================
...

============================================================
 Round Robin (Quantum = 4)
============================================================
...
```

---

## 📁 Project Structure

```
cpu-scheduling-simulator/
│
├── 📄 main.cpp            ← Entry point, argument parsing
├── 📄 scheduler.h         ← Algorithm declarations
├── 📄 scheduler.cpp       ← FCFS, SJF, RR implementations + Gantt output
├── 📄 process.h           ← Process struct definition
├── 📄 process.cpp         ← Process file I/O utilities
│
├── 🛠️ Makefile            ← Simple make build
├── 🛠️ CMakeLists.txt      ← Cross-platform CMake build
│
├── 📋 sample_input.txt    ← Sample 3-process test case
├── 🌐 gantt_visual.html   ← Example HTML Gantt output
│
├── 🏃 run_mac.sh          ← macOS one-command build & run
└── 🏃 run_windows.bat     ← Windows one-command build & run
```

---

## 🔬 Algorithm Comparison (Sample Input)

Using `sample_input.txt` with time quantum = 4:

| Metric | FCFS | SJF | Round Robin (q=4) |
|---|---|---|---|
| Avg. Waiting Time | 16.00 | 3.00 | 17.67 |
| Avg. Turnaround Time | 26.00 | 13.00 | 27.67 |
| Avg. Response Time | 16.00 | 3.00 | 0.00 |

> **SJF wins** on average waiting time when burst times are known.  
> **Round Robin** shines on response time — great for interactive workloads.

---

## 🧩 How It Works — Internals

```
main.cpp
   │
   ├── readProcessesFromFile()   ← parse input → vector<Process>
   │
   └── runAll()
         ├── runFCFS()           ← sort by arrival → sequential execution
         ├── runSJF()            ← sort by burst_time among arrived processes
         └── runRR()             ← queue-based round-robin with time quantum
               │
               └── printMetrics()  ← calculate & display stats per algorithm
```

Each `Process` tracks:
- `arrival_time`, `burst_time` (input)
- `remaining_time` (used by RR)
- `completion_time` (computed during simulation)

Derived metrics are calculated as:
$$\text{Turnaround} = \text{Completion} - \text{Arrival}$$
$$\text{Waiting} = \text{Turnaround} - \text{Burst}$$

---

## 🤝 Contributing

Contributions and improvements are welcome!

1. Fork this repo
2. Create a branch: `git checkout -b feature/your-feature`
3. Commit: `git commit -m "feat: add your feature"`
4. Push: `git push origin feature/your-feature`
5. Open a Pull Request 🚀

---

## 📜 License

This project is licensed under the **MIT License** — free to use, modify, and distribute.

---

<div align="center">

Made with ❤️ and lots of ☕ for an Operating Systems course.

*"An OS without a scheduler is just a very expensive paperweight."*

⭐ **Star this repo if it helped you!** ⭐

</div>
