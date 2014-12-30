#include "fovcomputer.hpp"

FovComputer::FovComputer(int w, int h) :
    w(w), h(h) {
    tiledata = new bool[w*h];
}

bool FovComputer::isInSight(int x1, int y1, int x2, int y2) {
    int dx = (x1 - x2) * (x1<x2?-1:1);
    int dy = (y1 - y2) * (y1<y2?-1:1);
    return dx+dy < 4;
}
