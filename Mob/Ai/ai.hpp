#ifndef AI_H
#define AI_H

#include "..\..\Main\main.hpp"
#include "pathfinder.hpp"
#include <random>

class Ai {
public:
    virtual void update(Mob*) = 0;

    Ai(int s, int o);

    virtual ~Ai() {}

    unsigned int swiftness;
    unsigned int timeoffset;
};

class PlayerAi : public Ai {
public:
    PlayerAi();

    void update(Mob*);
protected:
    void tryMoving(Mob*, int, int);
};

class BasicAi : public Ai {
public:
    BasicAi(int swiftness);

    void            update(Mob*);
protected:
    Pos             seekingPos;
    bool            sawPlayer;

    std::list<Pos>  path;
};

#endif
