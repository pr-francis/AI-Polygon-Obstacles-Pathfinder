#include <vector>
#include <map>
#include <cmath>
#include <cstdint>
#include "Vertex.h"
#include "Polygon.h"
#include "VisibilityGraph.h"

// 0 = Collinear, 1 = Clockwise, 2 = Counter-Clockwise
// cross product of pq and pr 
int VisibilityGraph::cross_product(ImVec2 p, ImVec2 q, ImVec2 r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    // handling floating point inaccuracies
    if (std::abs(val) < 0.001f) return 0;  
    return (val > 0.0f) ? 1 : 2;
}

bool VisibilityGraph::is_intersecting(ImVec2 A, ImVec2 B, ImVec2 C, ImVec2 D){
    int o1 = cross_product(A, B, C); //ABxAC
    int o2 = cross_product(A, B, D); //ABxAD
    int o3 = cross_product(C, D, A); //CDxCA
    int o4 = cross_product(C, D, B); //CDxCB

    if (o1 != o2 && o3 != o4)
        return true;
    return false;
}

std::map<int, std::vector<int>> VisibilityGraph::build_graph(std::vector<Polygon> polygons, Vertex start, Vertex goal){
    adjacency_list.clear();
    all_nodes.clear();
    walls.clear();
    
    // store all walls
    for (Polygon polygon : polygons){
        int num_vertices = polygon.vertices.size();
        for (int i = 0; i < num_vertices; i++){
            Vertex current = polygon.vertices[i];
            Vertex next = polygon.vertices[(i+1)%num_vertices];
            walls.push_back(std::make_pair(current, next));
        }
    }
    // add start and goal to the all nodes list
    all_nodes.push_back(start);
    all_nodes.push_back(goal);
    
    // add all polygon vertices to the all nodes list
    for (Polygon polygon : polygons){
        for (Vertex vertex : polygon.vertices){
            all_nodes.push_back(vertex);
        }
    }

    for (int i = 0; i < all_nodes.size(); i++){
        for (int j = i+1; j < all_nodes.size(); j++){
            Vertex nodeA = all_nodes[i];
            Vertex nodeB = all_nodes[j];

            // skip pair is they are a polygon diagonal
            if(!nodeA.unassigned && !nodeB.unassigned && (nodeA.polygonIdx == nodeB.polygonIdx)){
                bool is_side = false;
                for(std::pair<Vertex,Vertex> wall : walls){
                    if((wall.first.idx == nodeA.idx && wall.second.idx == nodeB.idx) ||
                        wall.first.idx == nodeB.idx && wall.second.idx == nodeA.idx){
                            is_side = true;
                            break;
                        }
                }
                if (!is_side) 
                    continue;
            }

            // check if there is a wall blocking the path between nodeA and nodeB
            bool is_blocked = false;
            for(std::pair<Vertex, Vertex> wall : walls){
                Vertex C = wall.first;
                Vertex D = wall.second;

                // skip if the path and wall are a corner
                if(nodeA.idx == C.idx || nodeB.idx == C.idx ||
                   nodeA.idx == D.idx || nodeB.idx == D.idx){
                    continue;
                }

                if(is_intersecting(nodeA.pos, nodeB.pos, C.pos, D.pos)){
                    is_blocked = true;
                    break;
                }
            }
            if (!is_blocked){
                adjacency_list[nodeA.idx].push_back(nodeB.idx);
                adjacency_list[nodeB.idx].push_back(nodeA.idx);
            }
        }
    }
    return adjacency_list;
}
