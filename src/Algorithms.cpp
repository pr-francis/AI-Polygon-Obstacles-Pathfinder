#include "Algorithms.h"
#include "VisibilityGraph.h"

#include <vector>
#include <map>
#include <queue>
#include <unordered_set>

// lookupTable = map<idx, <vector<pair<idx,cost>>>
// std::map<int, std::vector<std::pair<int, float>>> lookupTable= { // 0 is the start index, 5 is the goal index
//     {0, { {1, euclideanDistance(0,1)},{4, euclideanDistance(0,4)} }},
//     {1, { {0, euclideanDistance(1,0)},{2, euclideanDistance(1,2)},{4, euclideanDistance(1,4)} }},
//     {2, { {1, euclideanDistance(2,1)},{3, euclideanDistance(2,3)},{5, euclideanDistance(2,5)} }},
//     {3, { {2, euclideanDistance(3,2)},{4, euclideanDistance(3,4)},{5, euclideanDistance(3,5)} }},
//     {4, { {0, euclideanDistance(4,0)},{1, euclideanDistance(4,1)},{3, euclideanDistance(4,3)} }},
//     {5, { {2, euclideanDistance(5,2)},{3, euclideanDistance(5,3)} }}
// };

float Algorithms::euclideanDistance(int idx1, int idx2) {
    if (checkNeighbor(idx1,idx2)) {
        ImVec2 pos1 = vertices[idx1].pos;
        ImVec2 pos2 = vertices[idx2].pos;

        // used std::hypot to calculate the euclidean dist
        float dist = std::hypot(pos1.x - pos2.x, pos1.y - pos2.y);
        return dist;
    }
    else {
        return -1;
    }
}

bool Algorithms::checkNeighbor(int idx1, int idx2) { // check if two Vertices are connected in the lookup table
    const auto& idx1Neighbours = adjacencyList[idx1]; // this would return a vector from the lookupTable
    for (int p : idx1Neighbours) { // iterate over the vector pairs to check if the 2 indices are neighbors
        if (p==idx2) {
            return true;
        }
    }
    return false;
}

std::vector<int> Algorithms::bfs(int start, int end) {
	if (start == end) {return {};}

	std::queue<int> frontier;
	std::unordered_set<int> explored;
	std::map<int, int> parentMap; // map<child, parent>
	std::vector<int> neighbours;
	std::vector<int> path;
	int firstElement;
	frontier.push(start);

	while (!frontier.empty()) {
		firstElement = frontier.front();
		neighbours = adjacencyList[firstElement];
		// explored.insert(firstElement); [[wrong place to insert]]
		frontier.pop();
		if (neighbours.size()!=0) {
			for (int p : neighbours) {
				if(explored.count(p)==0) { // check if this node is already explored
					explored.insert(p);
					parentMap[p] = firstElement;	// map each child in the parentMap to its parent
					frontier.push(p);

					if (p==end){ // building the path
						for(int i = end; i != start; i = parentMap[i]){
							path.push_back(i);
						}

						path.push_back(start);
						std::reverse(path.begin(), path.end()); // reversing the whole path
						return path;
					}
				}
			}
		}
	}
	return {};
}
