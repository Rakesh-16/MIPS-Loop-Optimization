# MIPS Loop Optimization: Performance Enhancement via Loop Unrolling

[cite_start]This repository implements a systematic model for optimizing MIPS assembly code to maximize **Instruction-Level Parallelism (ILP)** and minimize pipeline stalls[cite: 41, 42]. [cite_start]The project demonstrates advanced architectural techniques including **Instruction Rescheduling**, **Register Renaming**, and **Loop Unrolling**[cite: 46, 47, 48].

## 🛠 Project Objectives
The model transforms a standard MIPS loop into a high-performance instruction stream by addressing the following:
* [cite_start]**Hazard Mitigation**: Automated insertion of NOP instructions based on a hardware latency table[cite: 45, 52].
* [cite_start]**Latency Optimization**: Rescheduling instructions to fill stall slots and reduce total clock cycles[cite: 46, 57].
* [cite_start]**Loop Unrolling**: Replicating the loop body (Factor 2–6) to significantly decrease iteration overhead[cite: 47, 60].
* [cite_start]**Register Renaming**: Eliminating WAR (Write-After-Read) and WAW (Write-After-Write) hazards[cite: 48, 61, 62].

## 📊 Latency Model & Methodology
[cite_start]The optimization logic is grounded in the following MIPS floating-point latency constraints[cite: 55, 56]:

| Instruction Producing Result | Instruction Using Result | Latency (Clock Cycles) |
| :--- | :--- | :--- |
| FP ALU operation | Another FP ALU op | 3 |
| FP ALU operation | Store Double (S.D) | 2 |
| Load Double (L.D) | FP ALU operation | 1 |
| Load Double (L.D) | Store Double (S.D) | 0 |

### The Optimization Pipeline
1. [cite_start]**NOP Insertion**: Identifies data hazards and ensures the instruction stream is safe for a standard pipeline[cite: 52, 53].
2. [cite_start]**Instruction Rescheduling**: Rearranges independent instructions (like `DADDUI`) to execute during functional unit latencies[cite: 57, 58].
3. [cite_start]**Loop Unrolling**: Unrolls the loop to expand the basic block size, providing more opportunities for scheduling[cite: 59, 60].
4. [cite_start]**Final Pipeline Scheduling**: Performs a global re-order of the unrolled instructions to ensure optimal utilization of all pipeline stages[cite: 63, 64].

## 🚀 Performance Analysis
[cite_start]The effectiveness of these optimizations was measured using total clock cycles as the primary metric[cite: 84, 113]:

* [cite_start]**Original Loop**: 9 Cycles[cite: 113, 212].
* [cite_start]**After Rescheduling**: **7 Cycles** (22% reduction in execution time)[cite: 114, 216].
* [cite_start]**Unrolled (Factor 6)**: 20 Cycles total, drastically reducing the *cycles-per-element* ratio[cite: 217, 218].

## 💻 Technical Implementation
The project consists of core C-based utilities used for assembly manipulation:
* [cite_start]`ins_stall.c`: Hazard detection and NOP insertion logic[cite: 80, 81].
* [cite_start]`user_unroll_loop.c`: Dynamic loop replication based on user input[cite: 183].
* [cite_start]`pipeline_ins.c`: Advanced rescheduling and register renaming for pipelined execution[cite: 188, 189].

### Example Output: Optimized Stream
[cite_start]The final output demonstrates the grouping of Loads to hide latency, followed by Parallel ALU operations[cite: 190]:
```mips
L.D F0, 0(R1)
L.D F2, -8(R1)
L.D F4, -16(R1)
...
ADD.D F1, F0, F2
ADD.D F3, F2, F2
...
S.D F1, 0(R1)
DADDUI R1, R1, #-48
BNE R1, R2, Loop
