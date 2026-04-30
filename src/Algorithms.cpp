#include "Algorithms.h"
#include "VisibilityGraph.h"

#include <limits>
#include <vector>
#include <map>
#include <queue>
#include <unordered_set>
#include <functional>
#include <algorithm>

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
	ImVec2 pos1 = vertices[idx1].pos;
    ImVec2 pos2 = vertices[idx2].pos;

    // used std::hypot to calculate the Euclidean dist
    float dist = std::hypot(pos1.x - pos2.x, pos1.y - pos2.y);
    return dist;
}

std::vector<int> Algorithms::bfs(int start, int end) {
	parentMap.clear();
	if (start == end) {return {};}

	std::queue<int> frontier;
	std::unordered_set<int> explored;
	std::vector<int> neighbours;
	std::vector<int> path;
	int firstElement;
	frontier.push(start);

	while (!frontier.empty()) {
		firstElement = frontier.front();
		neighbours = adjacencyList[firstElement];
		frontier.pop(); // remove the first element (least cost)
		if (!neighbours.empty()) {
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

std::vector<int> Algorithms::AStar(int start, int end) {
	parentMap.clear();

	std::vector<int> path;
	// inverting the behavior of the priority queue
	std::priority_queue<std::pair<float, int>, std::vector<std::pair<float,int>>, std::greater<> > frontier;
	// priority_queue<pair<f_cost,idx>> should sort now by least cost first

	std::unordered_set<int> explored;

	// will update both if we found a cheaper path to a node
	std::map<int, float> gCost; // used to store the least cost for each node.
	std::vector<int> neighbours; // used to store immediate neighbors for each node

	frontier.emplace(0,start);
	gCost[start] = 0;

	int firstElement;

	if (start==end){return {};}

	while (!frontier.empty()) {
		firstElement = frontier.top().second; // accessing the idx of the first element in the frontier
		if (firstElement == end) {
			for (int i=end; i != start; i=parentMap[i]) { // build the path
				path.push_back(i);
			}

			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}
		neighbours = adjacencyList[firstElement];
		frontier.pop();
		if (explored.count(firstElement)!=0) {
			continue;
		}
		explored.insert(firstElement);
		if (!neighbours.empty()){
			for (int p : neighbours) {
				if (explored.count(p)!=0) {
					continue;
				}
				// we can't just update the parents of a certain node without checking if it will lead to a less costly path
				// now we need to check if this parent can achieve less gCost for the current node
				float nodeFCost = fCost(firstElement,p,end,gCost);
				frontier.emplace(nodeFCost,p);
			}
		}
	}
	return {};
}

float Algorithms::fCost(int currentParent, int current, int end, std::map<int, float> &gCost) {
	float distPrev = euclideanDistance(current,currentParent);
	float nodeGCost = gCost[currentParent] + distPrev;

	if (gCost.count(current)==0) {  // check if the node G Cost exists in the gCost map
		gCost[current] = nodeGCost;
		parentMap[current] = currentParent;
		// since the cost wasn't calculated before, then it's our first instance with this node, so we insert the current parent.
	}

	if (nodeGCost<gCost[current]) { // check if the currently calculated gCost is less than then one saved in the gCost map
		gCost[current] = nodeGCost; // overwrite the old gCost with the new, lower one
		parentMap[current] = currentParent; // update the old parent of the node with the one used here
	}
	// ========================PS: KEPT BOTH IF STATEMENTS SEPARATED FOR CLARITY========================

	float nodeHCost = euclideanDistance(current, end);
	return nodeGCost + nodeHCost; // fCost = gCost + hCost
}

std::vector<int> Algorithms::dijkstra(int start, int end) {
	parentMap.clear();

	std::vector<int> path;
	// inverting the behavior of the priority queue
	std::priority_queue<std::pair<float, int>, std::vector<std::pair<float,int>>, std::greater<> > frontier;
	// priority_queue<pair<g_cost,idx>> should sort now by least cost first

	std::unordered_set<int> explored;

	// will update both if we found a cheaper path to a node
	std::map<int, float> gCost; // used to store the least cost for each node.
	std::vector<int> neighbours; // used to store immediate neighbors for each node

	frontier.emplace(0,start);
	gCost[start] = 0;

	int firstElement;

	if (start==end){return {};}

	while (!frontier.empty()) {
		firstElement = frontier.top().second; // accessing the idx of the first element in the frontier
		if (firstElement == end) {
			for (int i=end; i != start; i=parentMap[i]) { // build the path
				path.push_back(i);
			}

			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}
		neighbours = adjacencyList[firstElement];
		frontier.pop();
		if (explored.count(firstElement)!=0) {
			continue;
		}
		explored.insert(firstElement);
		if (!neighbours.empty()){
			for (int p : neighbours) {
				if (explored.count(p)!=0) {
					continue;
				}
				// we can't just update the parents of a certain node without checking if it will lead to a less costly path
				// now we need to check if this parent can achieve less gCost for the current node
				float nodeGCost = gCost[firstElement] + euclideanDistance(p, firstElement);
				if (gCost.find(p) == gCost.end()) {gCost[p] = std::numeric_limits<float>::infinity();}
				if (nodeGCost < gCost[p]) {
					frontier.emplace(nodeGCost,p);
					parentMap[p] = firstElement;
				}
			}
		}
	}
	return {};
}