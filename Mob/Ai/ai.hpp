#ifndef AI_H
#define AI_H

#include "..\..\Main\main.hpp"
#include "pathfinder.hpp"

class Ai {
public:
    virtual void update(Mob*) = 0;

    virtual ~Ai() {}

    int swiftness = 100;
};

class PlayerAi : public Ai {
public:
    void update(Mob*);
protected:
    void tryMoving(Mob*, int, int);
};

class TestAi : public Ai {
public:
    void update(Mob*);
protected:
    std::list<Pos> path;
};

#endif