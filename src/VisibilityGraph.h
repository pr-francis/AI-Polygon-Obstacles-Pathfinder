#pragma once
#include <vector>
#include <map>
#include <Vertex.h>
#include <Polygon.h>

class VisibilityGraph {
private:
    std::vector<std::pair<Vertex, Vertex>> walls;
    std::vector<Vertex> all_nodes;
    std::map<int, std::vector<int>> adjacency_list;

    int cross_product(ImVec2 p, ImVec2 q, ImVec2 r);
    bool is_intersecting(ImVec2 A, ImVec2 B, ImVec2 C, ImVec2 D);
public:
    VisibilityGraph() = default;
    std::map<int, std::vector<int>> build_graph(std::vector<Polygon> polygons, Vertex start, Vertex goal);
};