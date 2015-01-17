#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../../Misc/posstruct.hpp"
#include <list>

class Pathfinder {
public:
    Pathfinder(int, int);

    bool            *tiledata;
    int             w, h;

    void            setPosData(Pos, bool);

    void            computePath(Pos, Pos, std::list<Pos>&);
};

#endif
