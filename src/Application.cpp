#include "Application.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <thread>
#include <vector>

#include <imgui.h>
#include <Vertex.h>
#include <Polygon.h>

namespace App {
    static std::vector<Vertex> points_to_draw;
    static std::vector<Vertex> points;
    static Vertex start_node = Vertex{};
    static Vertex goal_node = Vertex{};
    static std::vector<Polygon> polys;
    ImDrawList* draw_list;
    bool created_shape = false;
    bool placing_start_node = false;
    bool placing_goal_node = false;

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

        ImGui::ShowDemoWindow();

        ImGui::End();
    }

    void RenderSidebar() {
        ImGui::Text("Algorithm:");
        static int algorithm = 0;
        ImGui::RadioButton("A*", &algorithm, 0);
        ImGui::RadioButton("Dijkstra", &algorithm, 1);
        ImGui::RadioButton("BFS", &algorithm, 2);

        if(ImGui::Button("Create Shape From Points") && points_to_draw.size() > 2) {
            created_shape = true;
        }

        if(ImGui::Button("Place start node")){
            placing_goal_node = false;
            placing_start_node = true;
            points_to_draw.clear();
        }

        if(ImGui::Button("Place goal node")){
            placing_goal_node = true;
            placing_start_node = false;
            points_to_draw.clear();
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
            Vertex point(mouse_pos_in_canvas, 0.0f);
            points_to_draw.push_back(point);
            points.push_back(point);
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && placing_start_node){
            Vertex point(mouse_pos_in_canvas, 0.0f);
            start_node = point;
            placing_start_node = false;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && placing_goal_node){
            Vertex point(mouse_pos_in_canvas, 0.0f);
            goal_node = point;
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
        if(!goal_node.unassigned){
            draw_list->AddCircleFilled(ImVec2(origin.x + goal_node.pos.x, origin.y + goal_node.pos.y), 4.0f, IM_COL32(255, 0, 0, 255));
        }
        if(!start_node.unassigned){
            draw_list->AddCircleFilled(ImVec2(origin.x + start_node.pos.x, origin.y + start_node.pos.y), 4.0f, IM_COL32(0, 255, 0, 255));
        }
        for(const auto& poly : polys){
            //@FIXME: Polygons are slightly shifted from the points position upon creation
            std::vector<ImVec2> pts;
            pts.reserve(poly.vertices.size());

            for (const auto& v : poly.vertices)
                pts.push_back(ImVec2(origin.x + v.pos.x, origin.y + v.pos.y));
            draw_list->AddConvexPolyFilled(pts.data(), pts.size(), IM_COL32(255, 255, 255, 255));
        }
        if(created_shape){
            Polygon poly(points_to_draw);
            polys.push_back(poly);
            points_to_draw.clear();
            created_shape = false;
        }
        draw_list->PopClipRect();
    }
} // namespace App
