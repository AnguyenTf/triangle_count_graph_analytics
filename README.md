# Triangle Counting in CUDA & C++

* Insert Triangle Count archiecture *

## Introducing Triangle Counting for Graph Analytics in CUDA & C++

In this project, I designed and implemented a fully high performance GPU implementation of triangle counting using CUDA and C++, built from the ground up with a custom graph loader, CSR converter, and GPU kernel. This project demonstrates how to design a complete graph anlytics pipeline optimized for parallel execution on NVIDIA GPUs.

---

### Why this project matters

I built this project because I wanted to understand what really goes into GPU-accelerated graph analytics, and not just writing a CUDA kernel, but everything around it. I thought this project turned out to be the perfect challenge: it's simple to describe, but it forces me to think carefully about data layout, memory access patterns, and how GPUs behave when the work isn't perfectly regular. Working through this from scratch taught me a lot about how high performnace systems are build. I had to design a graph loader that doesn't fall apart on a messy input, build a CSR converter that produces clean and sorted adjacency lists, and then write a kernel that can handle thousands of edges in parallel. 

---

### Highlights

- Full custom graph ingestion pipeline
- Memory coalesced structure of arrays layout for edges
- Safe CUDA error handing with CUDA_CHECK
- Per-edge parallelism for high GPU throughput

---

### Project reflection

Key things I learned were
- CUDA memory management
- Kernel design, thread indexing, grid/block configuration
- CSR graph representation and adjacency traversal
- Debugging GPU kernel and validating against CPU output

### Helpful References
- https://www.nvidia.com/en-us/glossary/graph-analytics/
 
 ---
 
### Architecture

1) Takes a files that contains ordered/unordered edges
2) Normalize each edge so that u < v
3) 
