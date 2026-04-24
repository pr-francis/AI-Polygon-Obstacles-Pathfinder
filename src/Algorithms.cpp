#include "Algorithms.h"
#include "Vertex.h"
#include<vector>
#include<map>
#include<iostream>
#include<cmath>

std::vector<Vertex> vertices; // we need this to lookup each index and its heuristic and position
// lookupTable = map<idx, <vector<pair<idx,cost>>>
std::map<int, std::vector<std::pair<int, float>>> lookupTable= { // 0 is the start index, 5 is the goal index
    {0, { {1, euclideanDistance(0,1)},{4, euclideanDistance(0,4)} }},
    {1, { {0, euclideanDistance(1,0)},{2, euclideanDistance(1,2)},{4, euclideanDistance(1,4)} } ,
    {2, { {1, euclideanDistance(2,1)},{3, euclideanDistance(2,3)},{5, euclideanDistance(2,5)} } ,
    {3, { {2, euclideanDistance(3,2)},{4, euclideanDistance(3,4)},{5, euclideanDistance(3,5)} } ,
    {4, { {0, euclideanDistance(4,0)},{1, euclideanDistance(4,1)},{3, euclideanDistance(4,3)}} ,
    {5, { {2, euclideanDistance(5,2)},{3, euclideanDistance(5,3)} }}
};

float euclideanDistance(int idx1, int idx2) {
    if (checkRelation(idx1,idx2)) {
        std::pair<float,float> pos1 = vertices[idx1].pos;
        std::pair<float,float> pos2 = vertices[idx2].pos;

        // used std::hypot to calculate the euclidean dist
        float dist = std::hypot(pos1.first - pos2.first,pos1.second - pos2.second);
        return dist
    }
    else {
        std::cout<<idx1<<" and "<<idx2<< " are not neighbors!"<<std::endl;
        return -1;
    }
}

bool checkNeighbor(int idx1, int idx2) { // check if two Vertices are connected in the lookup table
    std::vector<std::pair<int,float>> idx1Neighbours = lookupTable[idx1]; // this would return a vector from the lookupTable
    for (pair<int,float> p : idx1Neighbours) { // iterate over the vector pairs to check if the 2 indices are neighbors
        if (p.first==idx2) {
            return true;
        }
    }
    return false;
}

int main(){
vertices.push_back(Vertex((1,2),10)); // 0: start node
vertices.push_back(Vertex((2.5,3.5),7)); // 1
vertices.push_back(Vertex((4.5,3.5),4.3)); // 2
vertices.push_back(Vertex((4.5,0.5),3.2)); // 3
vertices.push_back(Vertex((2.5,0.5),8.3)); // 4
vertices.push_back(Vertex((5.5,2),0)); // 5


return 0;
}