# Metrics Reference

How the simulator calculates and reports performance metrics.

---

## Per-Process Metrics

### Completion Time (CT)

The clock time at which the process **finishes executing**.

```
CT = time when the last CPU burst of the process ends
```

### Turnaround Time (TT)

Total time the process spent in the system — from arrival to completion.

```
TT = CT − Arrival Time
```

**Example:** Process arrives at t=1, finishes at t=27  
→ TT = 27 − 1 = **26**

### Waiting Time (WT)

Time the process spent in the ready queue, **not** running on the CPU.

```
WT = TT − Burst Time
   = (CT − Arrival Time) − Burst Time
```

**Example:** TT=26, Burst=3  
→ WT = 26 − 3 = **23**

### Response Time (RT)

Time from arrival until the process **first** gets the CPU.  
Especially important for interactive systems.

```
RT = Time of first CPU slice − Arrival Time
```

> Round Robin tends to have the best response time because
> every process gets CPU access quickly, even if it isn't finished.

---

## Average Metrics

The simulator reports the **arithmetic mean** across all processes.

```
Avg TT = sum(TT for all processes) / number of processes
Avg WT = sum(WT for all processes) / number of processes
```

---

## Column Legend (Metrics Table)

| Column | Full Name | Formula |
|---|---|---|
| `PID` | Process ID | — |
| `AT` | Arrival Time | input |
| `BT` | Burst Time | input |
| `CT` | Completion Time | simulated |
| `TT` | Turnaround Time | CT − AT |
| `WT` | Waiting Time | TT − BT |

---

## Interpreting Results

- **Low WT** → processes spend less time waiting; CPU is used efficiently
- **Low TT** → processes finish quickly relative to when they arrived
- **Low RT** → system feels responsive to users

Algorithms can't optimize all three simultaneously — there are always trade-offs.
See [algorithms.md](algorithms.md) for a full comparison.
