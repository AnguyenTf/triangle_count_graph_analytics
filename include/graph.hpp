#pragma once
#include <vector>
#include <string>

/* Edge list graph representation
   Each undirected edge is stored once with src < dst*/
struct Edge{
    int src;
    int dst;
};

struct  EdgeListGraph{
    int num_vertices{};           // number of vertices
    int num_edges{};              // number of undirected edges
    std::vector<Edge> edges;      // edge list (src < dst)
};

// Load an edge list from a text file.
EdgeListGraph load_edge_list_from_file(const std::string& path);
