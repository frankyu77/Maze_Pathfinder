
#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    start = s;
    base = baseim;
    maze = mazeim;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc) {
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d) {
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
    int value = d%64;

    changeColorChannel(pixel->r, value, 5);
    changeColorChannel(pixel->g, value, 3);
    changeColorChannel(pixel->b, value, 1);
}

void treasureMap::changeColorChannel(unsigned char & color, int LOBValue, int choice) {
    int firstBit = ((LOBValue >> choice) & 1) << 1;
    int secondBit = (LOBValue >> (choice-1)) & 1;
    color = ((color & (unsigned char)0xfc) | firstBit) | secondBit;
}

PNG treasureMap::renderMap() {
    // 0
    PNG treasureImage = base;

    // 1.a
    vector<vector<bool>> visited(treasureImage.width(), vector<bool>(treasureImage.height(), false));
    // 1.b
    vector<vector<int>> distances(treasureImage.width(), vector<int>(treasureImage.height(), -1));
    // 2
    Queue<pair<int, int>> pixelsToProcess;

    // 3.a
    visited[start.first][start.second] = true;
    // 3.b
    distances[start.first][start.second] = 0;
    // 3.c
    setLOB(treasureImage, start, 0);
    // 3.d
    pixelsToProcess.enqueue(start);

    //4
    while (!pixelsToProcess.isEmpty()) {
        // 4.a
        pair<int, int> current = pixelsToProcess.peek();
        pixelsToProcess.dequeue();

        // 4.b
        for (auto neighbor : neighbors(current)) {
            // 4.b.i
            if (good(visited, current, neighbor)) {
                // 4.b.i.1
                visited[neighbor.first][neighbor.second] = true;
                // 4.b.i.2
                distances[neighbor.first][neighbor.second] = distances[current.first][current.second] + 1;
                // 4.b.i.3
                setLOB(treasureImage, neighbor, distances[neighbor.first][neighbor.second]);
                // 4.b.i.4
                pixelsToProcess.enqueue(neighbor);
            }
        }
    }
    // 5
    return treasureImage;
} 


PNG treasureMap::renderMaze(){
    PNG mazeImage = base;
    RGBAPixel * mazePixel = base.getPixel(start.first, start.second);

    vector<vector<bool>> visited(mazeImage.width(), vector<bool>(mazeImage.height(), false));
    Queue<pair<int, int>> pixelsToProcess;

    visited[start.first][start.second] = true;
    setGrey(mazeImage, start);
    pixelsToProcess.enqueue(start);

    while (!pixelsToProcess.isEmpty()) {
        pair<int, int> current = pixelsToProcess.peek();
        pixelsToProcess.dequeue();

        for (auto neighbor : neighbors(current)) {
            if (good(visited, current, neighbor)) {
                visited[neighbor.first][neighbor.second] = true;
                if (*mazeImage.getPixel(neighbor.first, neighbor.second) == *mazePixel) {
                    setGrey(mazeImage, neighbor);
                }
                pixelsToProcess.enqueue(neighbor);
            }
        }
    }

    return drawRedSquare(mazeImage);
}

PNG treasureMap::drawRedSquare(PNG img) {
    for(int i = start.first - 3; i <= start.first + 3; i++) {
        for(int j = start.second - 3; j <= start.second + 3; j++) {
            if (i >= 0 && i < img.width() && j >= 0 && j < img.height())
                *img.getPixel(i, j) = RGBAPixel(255, 0, 0);
        }
    }
    return img;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    if (next.first >= 0 && next.first < maze.width() &&
        next.second >= 0 && next.second < maze.height() &&
        !v[next.first][next.second] &&
        *maze.getPixel(curr.first, curr.second) == *maze.getPixel(next.first, next.second)) {
        return true;
    }
    return false;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int, int>> result;

    vector<pair<int, int>> directions = { {-1, 0}, {0, -1}, {0, 1}, {1, 0} };  // Left, Below, Right, Above

    for (auto dir : directions) {
        pair<int, int> next = {curr.first + dir.first, curr.second + dir.second};
        result.push_back(next);
    }

    return result;
}