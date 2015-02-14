#include "fovcomputer.hpp"

FovComputer::FovComputer(int w, int h) :
    w(w), h(h) {
    tiledata = new bool[w*h];
}

bool FovComputer::isInSight(int x1, int y1, int x2, int y2, int range) {
    if (y1 > y2) { //Now we know 1 is now lower than 2
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    if (std::abs(dx)+std::abs(dy)>range) return false;

    int x = x1, y = y1;

    if (dx == 0) { //Edge case: They are both in the same vertical position
        if (dy == 0) return true; //Edgecaseception: they're both the same place
        y++;
        while (y != y2) {
            if (tiledata[x+y*w]) return false;
            y++;
        }
        return true;
    }

    float slope = (float)dy/(float)dx;
    int xdir = x1 < x2 ? 1 : -1;


    if (slope == 1 || slope == -1) if (tiledata[x+(1+y)*w] || tiledata[x+xdir+y*w]) return false;

    if (slope < -1 || slope > 1) y++;
    else x += xdir;


    while (x != x2 || y != y2) {
        int newdy = y-y1, newdx = x-x1;

        if (tiledata[x+y*w]) {
            return false;
        }

        //Comparing 2 float numbers. due to rounding errors, this needs to have a slight tolerance
        if (std::abs(((float)newdy+0.5)/(newdx+(float)xdir/2) - slope) <= 0.001) {
            if (tiledata[x+(y+1)*w] || tiledata[x+xdir+y*w]) {
                return false;
            }
        }

        if (std::abs(((float)newdy+0.5)/(newdx+(float)xdir/2)) > std::abs(slope)) {
            x += xdir;
        } else {
            y++;
        } //todo FIX THIS PIECE OF CRAP YOU IDIOT


    }
    return true;

}

/*
Algorithm overview:

Draw a line between the source and end.
If there are any vision-blocking blocks between them, then cannot see.

In corners of tiles, all the four surrounding tiles count.

*/
