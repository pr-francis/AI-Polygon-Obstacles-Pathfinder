#include "Application.h"

#include <cmath>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

#include <imgui.h>
#include <Vertex.h>
#include <Polygon.h>

#include "Algorithms.h"
#include "VisibilityGraph.h"

namespace App {
    static std::vector<Vertex> points_to_draw;
    static std::vector<Vertex> points;
    static std::vector<Vertex> path_points;
    static VisibilityGraph visibility_graph;

    static Vertex start_node = Vertex{};
    static Vertex goal_node = Vertex{};
    static std::vector<Polygon> polys;
    static uint8_t next_idx = 0;
    static uint8_t next_polygon_idx = 0;
    ImDrawList* draw_list;
    bool created_shape = false;
    bool placing_start_node = false;
    bool placing_goal_node = false;
    bool start_node_placed = false;
    bool goal_node_placed = false;

    void RenderUI(){
        ImGui::Begin("UI");
        {
            ImGui::BeginChild("Grid", ImVec2(ImGui::GetContentRegionAvail().x * 0.75f, ImGui::GetContentRegionAvail().y));
            RenderGrid();
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("Sidebar");
            RenderSidebar();
            ImGui::EndChild();
        }

        //ImGui::ShowDemoWindow();

        ImGui::End();
    }

    void RenderSidebar() {
        ImGui::Text("Algorithm:");
        static int algorithm = 0;
        ImGui::RadioButton("BFS", &algorithm, 0);
        ImGui::RadioButton("A*", &algorithm, 1);
        ImGui::RadioButton("Dijkstra", &algorithm, 2);

        if(ImGui::Button("Create Shape From Points") && points_to_draw.size() > 2) {
            created_shape = true;
        }

        if(ImGui::Button("Place start node")){
            placing_goal_node = false;
            placing_start_node = true;
            //points_to_draw.clear();
        }

        if(ImGui::Button("Place goal node")) {
            placing_goal_node = true;
            placing_start_node = false;
            //points_to_draw.clear();
        }

        if (ImGui::Button("Generate path")) {
            std::vector<Vertex> all_vertices = points;
            start_node.idx = all_vertices.size();
            all_vertices.push_back(start_node);
            goal_node.idx = all_vertices.size();
            all_vertices.push_back(goal_node);

            const auto adjacency_list = visibility_graph.build_graph(polys, start_node, goal_node);
            Algorithms algorithms(adjacency_list, all_vertices);
            std::vector<int> path;

            switch (algorithm){
                case 0:
                    //bfs
                    path = algorithms.bfs(start_node.idx, goal_node.idx);
                    break;
                case 1:
                    //A*
                     path = algorithms.AStar(start_node.idx, goal_node.idx);
                    break;
                case 2:
                    //Dijkstra
                    path = algorithms.dijkstra(start_node.idx, goal_node.idx);
                    break;
                default:
                    break;
            }
            path_points.clear();
            for (const int &idx : path) {
                path_points.push_back(all_vertices[idx]);
            }
        }

        if (ImGui::Button("Clear all")){
            points_to_draw.clear();
            points.clear();
            path_points.clear();
            polys.clear();
            next_idx = 0;
            next_polygon_idx = 0;
            start_node_placed = false;
            goal_node_placed = false;
        }
    }

    void RenderGrid() {
        static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

        // Draw the canvas background and border
        ImGuiIO& io = ImGui::GetIO();
        draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

        draw_list->PushClipRect(canvas_p0, canvas_p1, true);
        const ImVec2 origin(canvas_p0.x, canvas_p0.y);
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

        // Adding points
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && !placing_start_node && !placing_goal_node){
            Vertex point(next_idx, next_polygon_idx, mouse_pos_in_canvas, 0.0f);
            points_to_draw.push_back(point);
            points.push_back(point);
            next_idx++;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && placing_start_node && ImGui::IsWindowHovered()){
            Vertex point(mouse_pos_in_canvas, 0.0f);
            start_node = point;
            start_node_placed = true;
            placing_start_node = false;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && placing_goal_node && ImGui::IsWindowHovered()){
            Vertex point(mouse_pos_in_canvas, 0.0f);
            goal_node = point;
            goal_node_placed = true;
            placing_goal_node = false;
        }

        // Draw the grid lines, shapes & points
        const float GRID_STEP = 16.0f;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));

        for(const auto& point : points_to_draw){
            draw_list->AddCircleFilled(ImVec2(origin.x + point.pos.x, origin.y + point.pos.y), 2.0f, IM_COL32(255, 255, 255, 255));
        }
        if(goal_node_placed){
            draw_list->AddCircleFilled(ImVec2(origin.x + goal_node.pos.x, origin.y + goal_node.pos.y), 4.0f, IM_COL32(255, 0, 0, 255));
        }
        if(start_node_placed){
            draw_list->AddCircleFilled(ImVec2(origin.x + start_node.pos.x, origin.y + start_node.pos.y), 4.0f, IM_COL32(0, 255, 0, 255));
        }
        for(const auto& poly : polys){
            std::vector<ImVec2> pts;
            pts.reserve(poly.vertices.size());

            for (const auto& v : poly.vertices)
                pts.emplace_back(origin.x + v.pos.x, origin.y + v.pos.y);
            draw_list->AddConvexPolyFilled(pts.data(), pts.size(), IM_COL32(255, 255, 255, 255));
        }
        if (!path_points.empty()) {
            for (int i = 0; i < path_points.size() - 1; i++) {
                draw_list->AddLine(
                    ImVec2(origin.x + path_points[i].pos.x, origin.y + path_points[i].pos.y),
                    ImVec2(origin.x + path_points[i + 1].pos.x, origin.y + path_points[i + 1].pos.y),
                        IM_COL32(0, 150, 255, 255), 4.0f);
            }
        }
        if(created_shape){
            // Calculate centroid
            ImVec2 centroid(0.0f, 0.0f);
            for (const auto& p : points_to_draw) {
                centroid.x += p.pos.x;
                centroid.y += p.pos.y;
            }
            centroid.x /= static_cast<float>(points_to_draw.size());
            centroid.y /= static_cast<float>(points_to_draw.size());

            // Sort vertices clockwise around centroid
            std::sort(points_to_draw.begin(), points_to_draw.end(), [&centroid](const Vertex& a, const Vertex& b) {
                float angleA = atan2f(a.pos.y - centroid.y, a.pos.x - centroid.x);
                float angleB = atan2f(b.pos.y - centroid.y, b.pos.x - centroid.x);
                return angleA < angleB;
            });

            Polygon poly(points_to_draw);
            polys.push_back(poly);
            points_to_draw.clear();
            next_polygon_idx++;
            created_shape = false;
        }
        draw_list->PopClipRect();
    }
} // namespace App
