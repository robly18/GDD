#ifndef FOVCOMPUTER_H
#define FOVCOMPUTER_H

#include <cmath>
#include <algorithm>

#include <iostream>

#define LOSRANGE 6

class FovComputer {
public:
    FovComputer(int, int);

    bool        *tiledata;
    int         w, h;

    bool        isInSight(int, int, int, int);
};


#endif
