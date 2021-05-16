#include "treasureMap.h"
#include "queue.h"
using namespace std;
namespace cv {}
using namespace cv;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

/* YOUR CODE HERE */
    start = s;
    base = baseim;
    maze = mazeim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);

    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);


}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

/* YOUR CODE HERE */
    string binaryD = bitset<6>(d%64).to_string();
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);

    pixel->r = pixel->r & 0xFC;
    pixel->g = pixel->g & 0xFC;
    pixel->b = pixel->b & 0xFC;

    unsigned char dr = bitset<2>(binaryD.substr(0, 2)).to_ulong();

    unsigned char dg = bitset<2>(binaryD.substr(2, 2)).to_ulong();

    unsigned char db = bitset<2>(binaryD.substr(4, 2)).to_ulong();

    pixel->r = pixel->r | dr;
    pixel->g = pixel->g | dg;
    pixel->b = pixel->b | db;

}

PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    PNG baseCopy = base;
    vector<vector<bool>> visitedMap(base.width() , vector<bool>(base.height(), false)); 
    vector<vector<int>> distanceMap(base.width() , vector<int>(base.height(), 0)); 

    Queue<pair<int,int>> traversalQueue;

    visitedMap[start.first][start.second] = true;
    distanceMap[start.first][start.second] = 0;
    setLOB(baseCopy, start, distanceMap[start.first][start.second]);
    traversalQueue.enqueue(start);

    while (!traversalQueue.isEmpty()) {
        pair<int, int> curr = traversalQueue.dequeue();

        vector<pair<int, int>> foundNeighbors = neighbors(curr);
        
        for (int i = 0; i  < foundNeighbors.size(); i++) {
            if (good(visitedMap, curr, foundNeighbors[i])) {
                visitedMap[foundNeighbors[i].first][foundNeighbors[i].second] = true;
                distanceMap[foundNeighbors[i].first][foundNeighbors[i].second] = distanceMap[curr.first][curr.second]+1;
                setLOB(baseCopy, foundNeighbors[i], distanceMap[foundNeighbors[i].first][foundNeighbors[i].second]);
                traversalQueue.enqueue(foundNeighbors[i]);
            }
        }
    }

    return baseCopy;
}


PNG treasureMap::renderMaze(){

/* YOUR CODE HERE */
    PNG baseCopy = base;
    vector<vector<bool>> visitedMap(base.width() , vector<bool>(base.height(), false)); 

    Queue<pair<int,int>> traversalQueue;

    visitedMap[start.first][start.second] = true;   
    traversalQueue.enqueue(start);

    while (!traversalQueue.isEmpty()) {
        pair<int, int> curr = traversalQueue.dequeue();

        vector<pair<int, int>> foundNeighbors = neighbors(curr);

        for (int i = 0; i  < foundNeighbors.size(); i++) {
            if (good(visitedMap, curr, foundNeighbors[i])) {
                visitedMap[foundNeighbors[i].first][foundNeighbors[i].second] = true;
                setGrey(baseCopy, foundNeighbors[i]);
                traversalQueue.enqueue(foundNeighbors[i]);
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            pair<int, int> curr(start.first-3+i, start.second-3+j);

            if (curr.first >= 0 && curr.first < visitedMap.size()) {
                if (curr.second >= 0 && curr.second < visitedMap[0].size()) {
                    *baseCopy.getPixel(curr.first, curr.second) = RGBAPixel(255, 0, 0);
                }
            }
        }
    }

    return baseCopy;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
    if (next.first >= 0 && next.first < v.size()) {
        if (next.second >= 0 && next.second < v[0].size()) {
            if (!v[next.first][next.second]) {
                if (*maze.getPixel(curr.first, curr.second) == *maze.getPixel(next.first, next.second)) {
                    return true;
                }
            }
        }
    }

    return false;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
    pair<int, int> left(curr.first-1, curr.second);
    pair<int, int> below(curr.first, curr.second+1);
    pair<int, int> right(curr.first+1, curr.second);
    pair<int, int> above(curr.first, curr.second-1);

    vector<pair<int, int>> neighbors;
    neighbors.push_back(left);
    neighbors.push_back(below);
    neighbors.push_back(right);
    neighbors.push_back(above);

    return neighbors;

}

