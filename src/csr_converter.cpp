#include "csr.hpp"
#include <algorithm>

/*  Example input graph (edge list):
    EdgeListGraph g = {
      num_vertices = 3,
      num_edges    = 3,
      edges = [{0,1}, {0,2}, {1,2}]
    };  */

// Convert an edge list graph into CSR format
CSRGraph build_csr_from_edge_list(const EdgeListGraph& g) {

    // Create an empty CSR graph
    CSRGraph csr;
    csr.num_vertices = g.num_vertices;
    csr.num_edges    = g.num_edges;

    // Initialize row_ptr with zeros (size = num_vertices + 1)
    csr.row_ptr.assign(csr.num_vertices + 1, 0);

    // Allocate space for adjacency list (undirected → 2 entries per edge)
    csr.col_idx.resize(2 * csr.num_edges);

    // Count degrees: increment row_ptr[u+1] and row_ptr[v+1] for each edge (u,v)
    for (const auto& e : g.edges) {
        csr.row_ptr[e.src + 1]++;
        csr.row_ptr[e.dst + 1]++;
    }

    // Prefix sum: convert degree counts into starting offsets
    for (int i = 1; i <= csr.num_vertices; i++) {
        csr.row_ptr[i] += csr.row_ptr[i - 1];
    }

    // Copy row_ptr into offset (used for writing into col_idx)
    std::vector<int> offset = csr.row_ptr;

    // Fill adjacency lists
    for (const auto& e : g.edges) {
        int u = e.src;
        int v = e.dst;

        int pos_u = offset[u]++;
        int pos_v = offset[v]++;

        csr.col_idx[pos_u] = v;
        csr.col_idx[pos_v] = u;
    }

    // Sort each adjacency list
    for (int v = 0; v < csr.num_vertices; v++) {
        int start = csr.row_ptr[v];
        int end   = csr.row_ptr[v + 1];
        std::sort(csr.col_idx.begin() + start, csr.col_idx.begin() + end);
    }
  
/* Final CSR example:
    csr = {
      num_vertices = 3,
      num_edges    = 3,
      row_ptr = {0, 2, 4, 6},
      col_idx = {1, 2, 0, 2, 0, 1}
    };  */
    return csr;
}
