# MIPS Loop Optimization: Performance Enhancement via Loop Unrolling

This repository implements a systematic model for optimizing MIPS assembly code to maximize **Instruction-Level Parallelism (ILP)** and minimize pipeline stalls. The project demonstrates advanced architectural techniques including **Instruction Rescheduling**, **Register Renaming**, and **Loop Unrolling**.

## 🛠 Project Objectives
The model transforms a standard MIPS loop into a high-performance instruction stream by addressing the following:
* **Hazard Mitigation**: Automated insertion of NOP instructions based on a hardware latency table.
* **Latency Optimization**: Rescheduling instructions to fill stall slots and reduce total clock cycles.
* **Loop Unrolling**: Replicating the loop body (Factor 2–6) to significantly decrease iteration overhead.
* **Register Renaming**: Eliminating WAR (Write-After-Read) and WAW (Write-After-Write) hazards.

## 📊 Latency Model & Methodology
The optimization logic is grounded in the following MIPS floating-point latency constraints:

| Instruction Producing Result | Instruction Using Result | Latency (Clock Cycles) |
| :--- | :--- | :--- |
| FP ALU operation | Another FP ALU op | 3 |
| FP ALU operation | Store Double (S.D) | 2 |
| Load Double (L.D) | FP ALU operation | 1 |
| Load Double (L.D) | Store Double (S.D) | 0 |

### The Optimization Pipeline
1. **NOP Insertion**: Identifies data hazards and ensures the instruction stream is safe for a standard pipeline.
2. **Instruction Rescheduling**: Rearranges independent instructions (like `DADDUI`) to execute during functional unit latencies.
3. **Loop Unrolling**: Unrolls the loop to expand the basic block size, providing more opportunities for scheduling.
4. **Final Pipeline Scheduling**: Performs a global re-order of the unrolled instructions to ensure optimal utilization of all pipeline stages.

## 🚀 Performance Analysis
The effectiveness of these optimizations was measured using total clock cycles as the primary metric:

* **Original Loop**: 9 Cycles.
* **After Rescheduling**: **7 Cycles** (22% reduction in execution time).
* **Unrolled (Factor 6)**: 20 Cycles total, drastically reducing the *cycles-per-element* ratio.

## 💻 Technical Implementation
The project consists of core C-based utilities used for assembly manipulation:
* `ins_stall.c`: Hazard detection and NOP insertion logic.
* `user_unroll_loop.c`: Dynamic loop replication based on user input.
* `pipeline_ins.c`: Advanced rescheduling and register renaming for pipelined execution.

### Example Output: Optimized Stream
The final output demonstrates the grouping of Loads to hide latency, followed by Parallel ALU operations:
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
