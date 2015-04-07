#ifndef POSSTRUCT_H
#define POSSTRUCT_H

#include <cstdlib>

struct Pos {
    int     x, y;
};

int         posDiff(const Pos &p1, const Pos &p2);

Pos         addPos(const Pos &p, const Pos &v);

bool        posEq(const Pos &p1, const Pos &p2);

bool        isUnitPos(const Pos &p);

#endif
