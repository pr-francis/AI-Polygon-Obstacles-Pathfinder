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
public:
	float euclideanDistance(int idx1, int idx2);
	bool checkNeighbor(int idx1, int idx2);
	std::vector<int> bfs(int start, int end);

	Algorithms(std::map<int, std::vector<int>> adjacencyList, const std::vector<Vertex>& vertices) : adjacencyList(std::move(adjacencyList)), vertices(vertices) {};
};


#endif //SRC_ALGORITHMS_H