# Memory-Page-Simulator
a C++ program built for my Operating System course at UNT that simulates core memory management concepts. 

# What it covers:
- Memory Allocation — First-fit, Best-fit, and Worst-fit algorithms across partitioned memory
- Virtual Memory — Page and offset address translation, logical vs physical address space
- Page Replacement — FIFO, LRU, and Optimal algorithms compared across 1–7 frames on a 20-reference string

# How to run:
g++ simulator.cpp -o simulator
./simulator

# What I learned
Implementing these algorithms from scratch gave me a much clearer picture of how an OS actually manages memroy under the hood, especially seeing how page fault counts change depending on the algorithm and number of frames available.

