#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

/* YOUR CODE HERE */
   mapImg = tm;
   start = s;

   //------------------------------------------------------------------------------------------
   vector<vector<bool>> visitedVect(mapImg.width(), vector<bool>(mapImg.height(), false));
   vector<vector<int>> lengthOfShortestVect(mapImg.width(), vector<int> (mapImg.height(), -1));
   vector<vector<pair<int, int>>> hint(mapImg.width(), vector<pair<int, int>>(mapImg.height(), make_pair(0,0)));
   Queue<pair<int, int>> q;
   pair<int, int> max;
   int maxDist = 0;

   visitedVect[start.first][start.second] = true;
   lengthOfShortestVect[start.first][start.second] = 0;
   
   q.enqueue(start);

   while (!q.isEmpty()) {
      pair<int, int> curr = q.peek();
      q.dequeue();
    
      vector<pair<int, int>> compassNeighbor = neighbors(curr);
      for (auto p : compassNeighbor) {
         if (good(visitedVect, lengthOfShortestVect, curr, p)) {
            visitedVect[p.first][p.second] = true;
            lengthOfShortestVect[p.first][p.second] = lengthOfShortestVect[curr.first][curr.second] + 1;
            hint[p.first][p.second] = curr;

            if (lengthOfShortestVect[p.first][p.second] > maxDist) {
               max.first = p.first;
               max.second = p.second;
               maxDist = lengthOfShortestVect[p.first][p.second];
            }

            q.enqueue(p);
         }
      }
   }

   pair<int, int> iterator = max;
   while (iterator != start) {
      pathPts.push_back(iterator);
      iterator = hint[iterator.first][iterator.second];
   }
   pathPts.push_back(start);
   //------------------------------------------------------------------------------------------
   
}

PNG decoder::renderSolution(){

/* YOUR CODE HERE */
PNG solution = mapImg;
   for (auto p : pathPts) {
      setRed(solution, p);
   }
   return solution;
}

PNG decoder::renderMaze(){
   PNG mazeImage = mapImg;
/* YOUR CODE HERE */
    vector<vector<bool>> visitedVect(mazeImage.width(), vector<bool>(mazeImage.height(), false));
    vector<vector<int>> lengthOfShortestVect(mazeImage.width(), vector<int> (mazeImage.height(), -1));
    Queue<pair<int, int>> q;

    visitedVect[start.first][start.second] = true;
    lengthOfShortestVect[start.first][start.second] = 0;
    setGrey(mazeImage, start);
    q.enqueue(start);


    while (!q.isEmpty()) {
        pair<int, int> curr = q.peek();
        q.dequeue();

        vector<pair<int, int>> compassNeighbor = neighbors(curr);
        for (auto p : compassNeighbor) {
            if (good(visitedVect, lengthOfShortestVect, curr, p)) {
                visitedVect[p.first][p.second] = true;
                lengthOfShortestVect[p.first][p.second] = lengthOfShortestVect[curr.first][curr.second] + 1;
                setGrey(mazeImage, p);
                q.enqueue(p);
            }
        }

    }

    return redSquare(mazeImage);

}

void decoder::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
   RGBAPixel *pixelToChange = im.getPixel(loc.first, loc.second);

   pixelToChange->r = 2*(pixelToChange->r / 4);
   pixelToChange->g = 2*(pixelToChange->g / 4);
   pixelToChange->b = 2*(pixelToChange->b / 4);
}

void decoder::setRed(PNG & im, pair<int,int> loc) {
   RGBAPixel *pixelToChange = im.getPixel(loc.first, loc.second);

   pixelToChange->r = 255;
   pixelToChange->g = 0;
   pixelToChange->b = 0;
}

PNG decoder::redSquare(PNG im) {
    for(int i = start.first - 3; i <= start.first + 3; i++) {
        for(int j = start.second - 3; j <= start.second + 3; j++) {
            if (i >= 0 && i < im.width() && j >= 0 && j < im.height())
                *im.getPixel(i, j) = RGBAPixel(255, 0, 0);
        }
    }
    return im;
}


pair<int,int> decoder::findSpot(){

/* YOUR CODE HERE */
   return pathPts.front();      //--------------

}

int decoder::pathLength(){

/* YOUR CODE HERE */
   return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
   if (next.first >= 0 && next.first < mapImg.width() && next.second >= 0 && next.second < mapImg.height()) {
      if (!v[next.first][next.second]) {
         if (compare(*mapImg.getPixel(next.first, next.second), d[curr.first][curr.second])) {
            return true;
         }
         return false;
      }
      return false;
   }
   return false;

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
   vector<pair<int,int>> result;
   pair<int, int> left;
   pair<int, int> below;
   pair<int, int> right;
   pair<int, int> above;

   left.first = curr.first - 1;
   left.second = curr.second;
   below.first = curr.first;
   below.second = curr.second - 1;
   right.first = curr.first + 1;
   right.second = curr.second;
   above.first = curr.first;
   above.second = curr.second + 1;

   result.push_back(left);
   result.push_back(below);
   result.push_back(right);
   result.push_back(above);
    
   return result;

}


bool decoder::compare(RGBAPixel p, int d){

/* YOUR CODE HERE */
   int decodedR = (p.r % 4)*16;
   int decodedG = (p.g % 4)*4;
   int decodedB = (p.b % 4);

   int decode = decodedR + decodedG + decodedB;

   return decode == (d+1)%64;

}
