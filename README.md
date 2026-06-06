# multithreaded-diagonal-sums
Multithreaded C program that scans an N×N grid for diagonal sequences summing to a target value using POSIX threads, with O(n³) time complexity.

# Multithreaded Diagonal Sums

A systems programming project written in C that finds all diagonal sequences in an N×N grid of digits that sum to a target value. The program uses POSIX threads to parallelize the computation across up to 3 threads, demonstrating measurable performance improvements on large grids.

## How it works

Given an N×N grid of digits (1–9), the program scans every cell in four diagonal directions — down-left, down-right, up-left, up-right — and identifies sequences whose values sum to a target value `s`. Matching cells are written to an output grid.

Computation is split across `t` threads (1–3), where each thread handles a subset of rows. A mutex lock ensures safe writes to the shared output grid.

## Tech Stack

- Language: C
- Concurrency: POSIX Threads (pthreads)
- Memory: Dynamic allocation with full cleanup (valgrind verified)
- Build tool: Make
- Debugging: GDB, Valgrind

## Performance

For large grids, using more threads produces measurable speedups:

| Threads | Grid Size | Time (approx) |
|---------|-----------|----------------|
| 1 | 672×672 | ~0.006s |
| 2 | 2778×2778 | ~0.21s |
| 3 | 3567×3567 | ~11.5s |

Note: for small grids, thread overhead can make single-threaded execution faster due to the cost of thread creation and synchronization.

## Getting Started

**Requirements:** GCC and Make on a Unix/Linux system.

**Build:**
```bash
make
```

**Run:**
```bash
./proj4.out <input_file> <output_file> <target_sum> <num_threads>
```

**Example:**
```bash
./proj4.out in1.txt out1.txt 10 1
./proj4.out in2.txt out2.txt 19 2
./proj4.out in3.txt out3.txt 3 3
```

**Clean build files:**
```bash
make clean
```

## Project Structure
