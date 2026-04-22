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
    static std::vector<Polygon> polys;
    ImDrawList* draw_list;
    bool created_shape = false;
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
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()){
            Vertex point(mouse_pos_in_canvas, 0.0f);
            points_to_draw.push_back(point);
            points.push_back(point);
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
