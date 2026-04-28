#ifndef SRC_ALGORITHMS_H
#define SRC_ALGORITHMS_H

#include "Vertex.h"
#include <queue>
#include "VisibilityGraph.h"
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <algorithm>


class Algorithms{
private:
	std::vector<Vertex> vertices;
public:
	float euclideanDistance(int idx1, int idx2);
	bool checkNeighbor(int idx1, int idx2);
	std::vector<int> bfs(std::map<int, std::vector<int>> adjacency_list, int start, int end);
};


#endif //SRC_ALGORITHMS_H