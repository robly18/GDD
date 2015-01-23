#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../../Misc/posstruct.hpp"
#include <list>
#include <stdlib.h>

class Pathfinder {
public:
    Pathfinder(int, int);

    bool            *tiledata;
    int             w, h;

    void            setPosData(Pos, bool);
    bool            canGoThrough(Pos);

    void            computePath(Pos, Pos, std::list<Pos>&);
private:
    bool            *computedata;
};

#endif