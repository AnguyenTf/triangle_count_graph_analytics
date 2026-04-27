# Triangle Counting in CUDA & C++: Implementation, Profiling, and Optimization

![CUDA Graph Pipeline](assets/triangle_count.png)

## Overview

This repository is my deep dive into GPU-accelerated graph analytics, built entirely form scratch using CUDA and C++. I didn't want to just write a kernel, I wanted to understand the whoe pipeline: from loading in messy data, converting it into a clean CSR layout, and then pushing it through a GPU kernel that can handle millions of edges in parallel. 

Along the way, I used Nsight Systems and Nsight Compute to study how my code actually behaves on the GPU, not in theory, but in real hardware. This project became a hands-on way for me to learn how high-performance GPU systems are designed, debugged, and optimized.

---

### Why this project matters 

I've always been fascinated by how GPUs go through massive workloads, but I realized I didn't truly understand what happens between "write a kernel" and "get fast results." This project turned out to be the perfect challenge for me:

- It's simple to explain
- It forces me to think about memory layout
- It exposes how GPU's behave under irregular workloads
- It rewards careful engineering

Working through this project taught me how much performance comes from everything around the kernel: the data structures, the memory access patterns, the launch configuration, and the profiling tools that reveal what's really happening under the hood.

---

### Highlights

- Custom graph ingestion pipeline
- Clean CSR construction with sorted adjacency lists
- Memory coalesced structure of arrays layout for edges
- Safe CUDA error handing with CUDA_CHECK
- Per-edge parallelism for high GPU throughput
- Full performance analysis using Nsight Systems and Nsight Compute

---

### Project Structure + How to build and run the project

To build the project with full support for profiling and source‑level analysis, compile it using:

```
accelerated_graph_analytics/
├── data/
│   ├── sample_graph.txt
├── include/
│   ├── csr.hpp
│   ├── graph.hpp
│   ├── triangle_count.hpp
├── src/
│   ├── csr_converter.cpp
│   ├── graph_loader.cpp
│   ├── main.cpp
│   └── triangle_count.cu
├── triangle_count.exe
```

```
nvcc -O3 -std=c++17 -lineinfo -g -Iinclude ^
  src\triangle_count.cu ^
  src\main.cpp ^
  src\graph_loader.cpp ^
  src\csr_converter.cpp ^
  -o triangle_count.exe
```

---
 
### Architecture

1) Read an edge list file (ordered or unordered)
2) Normalize each edge so that u < v
3) Sort edges by (u, v) to guarantee deterministic adjacency lists
4) Build CSR (row_ptr + col_idx)
5) For each edge (u, v), run a two-pointer intersection on neighbors(u) and neighbors(v)
6) Count all w such that u < v < w
7) Parallelize the entire process on the GPU

---

### Nsight System

<img width="2304" height="1221" alt="image" src="https://github.com/user-attachments/assets/520e9c61-a634-4bdc-b09b-b66eefd057fa" />

With Nsight System, I analyzed the behavior of my triangle counting program. The profiler showed that the majority of the runtime was spent in host-to-device memory transfer and repeated cudaMalloc calls. This clearly indicates that the program is memory-bound, not compute bound. 

The CUDA API Summary showed:
- cudaMalloc: accounted for ~78% of runtime (165 ms across 5 calls)
- cudaMemcpy: acccounted for ~2.6% of runtime (5.5ms)
- cudaFree: added another 1.3ms
- cudaLaunchKernel: added another .60ms
- cudaDeviceSynchronize: added another .76ms

Key items to look at:
- cudaMalloc (GPU memory allocation)
- cudaMemcpy (Host <-> device data transfer)
- cudaFree (Free GPU memory)
- cudaLaunchKernel (Launching the kernel)
- cudaDeviceSynchronize (CPU waits for GPU)

---

### Nsight Compute

<img width="2311" height="579" alt="image" src="https://github.com/user-attachments/assets/75ae9d73-6908-4de0-9ac9-d1cd46af2514" />

The initial profiling with Nsight Compute showed:
- Compute SOL: ~51%
- Memory SOL: ~45% 

This indicates the kernel was neither compute-bound nor memory-bound but instead limited by latency-bound due to memory access patterns and warp inefficiency. 

Night's recommendations highlighted uncoalesced global loads, low L1/TEX hit rates, and warp divergence caused by irregular neighbor list lengths.

Definitions:
- Uncoalesced global loads: Occurs when threads in the same warp access global memory addresses that are far apart, forcing the GPU to issue multiple memory transactions instead of one. This reduces memory throughput and increases latency.
- Low L1/TEX hit rate: Most memory accesses miss the L1 cache and must be served from slower memory (L2 or DRAM). This happens when threads do not reuse nearby data or access memory irregularly.
- Warp divergence: Occurs when threads in the same warp take different control-flow paths (different branches or loop iterations). The warp must serialize these paths, reducing parallel efficiency.

Key items to look at:                             
- Live Registers: How many  registers are in use at that line. 
- Attributed Stalls: Why the warp is stalled at that line. 
- Attributed Live Registers: Registers attributed to that instruction. 
- Warp Stall (Not-issued Samples): Warp couldn't issue an instruction. 
- Instruction Mix: What types of instructions run (memory, control, integer). 
- Instructions Executed: How many instructions executed at that line. 
- Avg Predicated-On Threads Executed: How many threads actually executed the instruction. 

---

### Helpful References

- https://www.nvidia.com/en-us/glossary/graph-analytics/
- https://www.youtube.com/watch?v=F_BazucyCMw&t=14s
- https://docs.nvidia.com/nsight-compute/NsightCompute/index.html
- https://docs.nvidia.com/nsight-systems/UserGuide/index.html#cuda-trace

