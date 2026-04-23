#include "graph.hpp"
#include "csr.hpp"
#include "triangle_count.hpp"
#include <iostream>

/* Triangle counting driver program.
   Usage: ./triangle_count <graph_file>

   Loads an edge list, converts it to CSR
   format, and runs both CPU and GPU triangle 
   counting implementations.*/
int main(int argc, char** argv) {

  // Ensures a graph file was provided
  if (argc < 2) {
    std::cerr << "Usage: ./triangle_count <graph_file>\n";
    return 1;
  }

  std::string path = argv[1];

  try {
    // Load graph as an edge list
    auto edge_list = load_edge_list_from_file(path);

    // Convert to CSR
    auto csr = build_csr_from_edge_list(edge_list);

    std::cout << "Vertices: " << csr.num_vertices << ", Undirected Edges: " << edge_list.num_edges << "\n";

    // CPU triangle count
    long long cpu_tri = triangle_count_cpu(csr);
    std::cout << "CPU triagles: " << cpu_tri << "\n";

    // GPU triangle count
    long long gpu_tri = triangle_count_gpu(csr, edge_list);
    std::cout << "GPU triangles: " << gpu_tri << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

  return 0;

}
