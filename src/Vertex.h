#pragma once

#include <imgui.h>

struct Vertex {
    uint8_t idx;
    uint8_t polygonIdx;
    ImVec2 pos;
    float heuristic;
    bool unassigned = false;

    Vertex() : idx(0), pos(ImVec2(0.0f, 0.0f)), heuristic(0.0f), unassigned(true) {}
    Vertex(uint8_t i, uint8_t polygonIdx, ImVec2 p, float h) : idx(i), polygonIdx(polygonIdx), pos(p), heuristic(h) {}
    Vertex(ImVec2 p, float h) : pos(p), heuristic(h) {}

};
