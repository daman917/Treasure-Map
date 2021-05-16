#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

/* YOUR CODE HERE */
   visitedMap = vector<vector<bool>>(mapImg.width() , vector<bool>(mapImg.height(), false));
   vector<vector<int>> distanceMap(mapImg.width() , vector<int>(mapImg.height(), 0)); 
   vector<vector<pair<int, int>>> sourceMap(mapImg.width() , vector<pair<int, int>>(mapImg.height(), make_pair(0, 0)));

   Queue<pair<int,int>> traversalQueue;

   visitedMap[start.first][start.second] = true;
   distanceMap[start.first][start.second] = 0;
   sourceMap[start.first][start.second] = start;
   traversalQueue.enqueue(start);

   while (!traversalQueue.isEmpty()) {
      pair<int, int> curr = traversalQueue.dequeue();

      vector<pair<int, int>> foundNeighbors = neighbors(curr);
        
      for (int i = 0; i  < foundNeighbors.size(); i++) {
         if (good(visitedMap, distanceMap, curr, foundNeighbors[i])) {
            visitedMap[foundNeighbors[i].first][foundNeighbors[i].second] = true;
            distanceMap[foundNeighbors[i].first][foundNeighbors[i].second] = distanceMap[curr.first][curr.second]+1;
            sourceMap[foundNeighbors[i].first][foundNeighbors[i].second] = curr;
            traversalQueue.enqueue(foundNeighbors[i]);
         }
      }
   }

   pair<int, int> end(start.first, start.second);

   for (int i = 0; i < distanceMap.size(); i++) {
      for (int j = 0; j < distanceMap[0].size(); j++) {
         if (distanceMap[i][j] >= distanceMap[end.first][end.second]) {
            end = make_pair(i, j);
         }
      }
   }

   pathPts.push_back(end);
   pair<int, int> curr = end;

   while (sourceMap[curr.first][curr.second] != curr) {
      pathPts.push_back(sourceMap[curr.first][curr.second]);
      curr = sourceMap[curr.first][curr.second];
   }

   reverse(pathPts.begin(), pathPts.end());
    
}

PNG decoder::renderSolution(){

/* YOUR CODE HERE */
   PNG mapImgCopy = mapImg;

   for (int i = 0; i < pathLength(); i++) {
      RGBAPixel * pixel = mapImgCopy.getPixel(pathPts[i].first, pathPts[i].second);
      
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
   }

   return mapImgCopy;
}

PNG decoder::renderMaze(){

/* YOUR CODE HERE */
    PNG mapImgCopy = mapImg;

   for (int i = 0; i < visitedMap.size(); i++) {
      for (int j = 0; j < visitedMap[0].size(); j++) {
         if (visitedMap[i][j]) {
            setGrey(mapImgCopy, make_pair(i, j));
         }
      }
   }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            pair<int, int> curr(start.first-3+i, start.second-3+j);

            if (curr.first >= 0 && curr.first < visitedMap.size()) {
                if (curr.second >= 0 && curr.second < visitedMap[0].size()) {
                    *mapImgCopy.getPixel(curr.first, curr.second) = RGBAPixel(255, 0, 0);
                }
            }
        }
    }

    return mapImgCopy;

}

void decoder::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);

    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);

}

pair<int,int> decoder::findSpot(){

/* YOUR CODE HERE */
   return pathPts[pathLength()-1];

}

int decoder::pathLength(){

/* YOUR CODE HERE */
   return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
    if (next.first >= 0 && next.first < v.size()) {
        if (next.second >= 0 && next.second < v[0].size()) {
            if (!v[next.first][next.second]) {
                if (compare(*mapImg.getPixel(next.first, next.second), d[curr.first][curr.second])) {
                    return true;
                }
            }
        }
    }

    return false;

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

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


bool decoder::compare(RGBAPixel p, int d){

/* YOUR CODE HERE */
   int mazeValue = (p.r % 4) * 16 + (p.g % 4) * 4 + p.b % 4;
   return mazeValue == (d + 1) % 64;

}
