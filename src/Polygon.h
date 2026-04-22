#pragma once

#include <vector>
#include <Vertex.h>
#include <imgui.h>

struct Polygon {
    std::vector<Vertex> vertices;
    
    Polygon() = default;
    Polygon(std::vector<Vertex> vertices) : vertices(std::move(vertices)) {}
};