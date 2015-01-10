#ifndef FOVCOMPUTER_H
#define FOVCOMPUTER_H

#include <cmath>
#include <algorithm>

#include <iostream>

class FovComputer {
public:
    FovComputer(int, int);

    bool        *tiledata;
    int         w, h;

    bool        isInSight(int, int, int, int, int);
};


#endif
