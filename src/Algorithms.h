#ifndef SRC_ALGORITHMS_H
#define SRC_ALGORITHMS_H

#include "Vertex.h"
#include <queue>
#include "VisibilityGraph.h"
#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <algorithm>


class Algorithms{
private:
	std::map<int, std::vector<int>> adjacencyList;
	std::vector<Vertex> vertices;
	std::map<int, int> parentMap; // map<child, parent>
public:
	float euclideanDistance(int idx1, int idx2);
	std::vector<int> bfs(int start, int end);

	std::vector<int> AStar(int start, int end);
	float fCost(int currentParent, int current, int end, std::map<int, float> &gCost); // fCost for AStar algo

	Algorithms(std::map<int, std::vector<int>> adjacencyList, const std::vector<Vertex>& vertices) : adjacencyList(std::move(adjacencyList)), vertices(vertices) {};
};


#endif //SRC_ALGORITHMS_H