#pragma once

#include <imgui.h>

struct Vertex {
    ImVec2 pos;
    float heuristic;
    bool unassigned = false;

    Vertex() : pos(ImVec2(0.0f, 0.0f)), heuristic(0.0f), unassigned(true) {}
    Vertex(ImVec2 p, float h) : pos(p), heuristic(h) {}
};
