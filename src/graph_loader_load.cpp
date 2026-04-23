#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

EdgeListGraph load_edge_list_from_file(const std::string& path) {

    EdgeListGraph g;

    // Open the input file
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open graph file: " + path);
    }

    std::vector<Edge> raw_edges;

    int u, v;
    int max_vertex = -1;

    // Read edges from file
    while (in >> u >> v) {

        // Detect negative vertex IDs
        if (u < 0 || v < 0) {
            throw std::runtime_error("Error: negative vertex ID detected in input graph.");
        }

        // Ignore self-loops
        if (u == v) continue;

        // Normalize edge so src < dst
        if (u > v) std::swap(u, v);

        raw_edges.push_back({u, v});

        max_vertex = std::max(max_vertex, std::max(u, v));
    }

    // Empty graph case
    if (max_vertex < 0) {
        g.num_vertices = 0;
        g.num_edges = 0;
        return g;
    }

    // Sort edges by (src, dst)
    std::sort(raw_edges.begin(), raw_edges.end(),
              [](const Edge& a, const Edge& b) {
                  if (a.src != b.src) return a.src < b.src;
                  return a.dst < b.dst;
              });

    // Remove duplicates
    raw_edges.erase(
        std::unique(raw_edges.begin(), raw_edges.end(),
                    [](const Edge& a, const Edge& b) {
                        return a.src == b.src && a.dst == b.dst;
                    }),
        raw_edges.end()
    );

    // Number of vertices (0-based indexing)
    g.num_vertices = max_vertex + 1;

    // Number of undirected edges
    g.num_edges = static_cast<int>(raw_edges.size());

    // Move edge list into graph
    g.edges = std::move(raw_edges);

    return g;
}
