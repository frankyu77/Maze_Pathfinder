#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

/* YOUR CODE HERE */
    base = baseim;
    maze = mazeim;
    start = s;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
    RGBAPixel *pixelToChange = im.getPixel(loc.first, loc.second);

    pixelToChange->r = 2*(pixelToChange->r / 4);
    pixelToChange->g = 2*(pixelToChange->g / 4);
    pixelToChange->b = 2*(pixelToChange->b / 4);

}

//-----------------------------------------------------------------------------HELPER
PNG treasureMap::setRed(PNG im){
    for(int i = start.first - 3; i <= start.first + 3; i++) {
        for(int j = start.second - 3; j <= start.second + 3; j++) {
            if (i >= 0 && i < im.width() && j >= 0 && j < im.height())
                *im.getPixel(i, j) = RGBAPixel(255, 0, 0);
        }
    }
    return im;

}


void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

/* YOUR CODE HERE */
    RGBAPixel *pixelToChange = im.getPixel(loc.first, loc.second);

    unsigned int encodedValue = d % 64;
    
    unsigned int encodedR = encodedValue >> 4;
    unsigned int encodedG = (encodedValue & 15) >> 2;
    unsigned int encodedB = (encodedValue & 3);

    unsigned int changedR = (pixelToChange->r & 252) | encodedR;
    unsigned int changedG = (pixelToChange->g & 252) | encodedG;
    unsigned int changedB = (pixelToChange->b & 252) | encodedB;

    pixelToChange->r = (unsigned char) changedR;
    pixelToChange->g = (unsigned char) changedG;
    pixelToChange->b = (unsigned char) changedB;


}

PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    PNG copy = base;
    vector<vector<bool>> visitedVect(copy.width(), vector<bool>(copy.height(), false));
    vector<vector<int>> lengthOfShortestVect(copy.width(), vector<int> (copy.height(), -1));
    Queue<pair<int, int>> q;

    visitedVect[start.first][start.second] = true;
    lengthOfShortestVect[start.first][start.second] = 0;
    setLOB(copy, start, 0);
    q.enqueue(start);

    //cout<<"loop123"<<"\n";

    while (!q.isEmpty()) {
        pair<int, int> curr = q.peek();
        q.dequeue();

        vector<pair<int, int>> compassNeighbor = neighbors(curr);
        for (auto p : compassNeighbor) {
            if (good(visitedVect, curr, p)) {
                //cout<<"left exist"<<"\n";
                visitedVect[p.first][p.second] = true;
                lengthOfShortestVect[p.first][p.second] = lengthOfShortestVect[curr.first][curr.second] + 1;
                setLOB(copy, p, lengthOfShortestVect[p.first][p.second]);
                q.enqueue(p);
            }
        }

    }


    return copy;
}


PNG treasureMap::renderMaze(){

/* YOUR CODE HERE */
    PNG copy = base;
    RGBAPixel *copyPixel = base.getPixel(start.first, start.second);

    vector<vector<bool>> visitedVect(copy.width(), vector<bool>(copy.height(), false));
    Queue<pair<int, int>> q;

    visitedVect[start.first][start.second] = true;
    setGrey(copy, start);
    q.enqueue(start);


    while (!q.isEmpty()) {
        pair<int, int> curr = q.peek();
        q.dequeue();

        vector<pair<int, int>> compassNeighbor = neighbors(curr);
        for (auto p : compassNeighbor) {
            if (good(visitedVect, curr, p)) {
                visitedVect[p.first][p.second] = true;
    
                if (*copy.getPixel(p.first, p.second) == *copyPixel) {
                    setGrey(copy, p);
                }
                q.enqueue(p);
            }
        }
    }

    return setRed(copy);
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
    if (next.first < maze.width() && next.first >= 0 && next.second < maze.height() && next.second >= 0) {
        if (!v[next.first][next.second]) {
            if (*maze.getPixel(curr.first, curr.second) == *maze.getPixel(next.first, next.second)) {
                return true;
            }
            return false;

        }
        return false;
    }
    return false;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

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

