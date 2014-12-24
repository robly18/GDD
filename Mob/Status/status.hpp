#ifndef STATUS_H
#define STATUS_H

#include "..\..\Main\main.hpp"
#include <list>
/*
Have a few types of statuses, some good, some bad.

Good:
Block, Reflect(?), Ice skin
Regens
Quirks?
Potion effects

Bad:
Poison,
Frozen,
Stunned (can't attack),
Curse (more vulnerable to attacks)
*/
class Status;

class StatusHolder { //todo add stackable attribute?
public:
    StatusHolder() {}

    std::list<Status*>  statuses;

    void                render(SDL_Surface*, SDL_Rect);
    void                update(Mob*);

    int                 poisondmg = 0;
    int                 healval = 0;
};


class Status {
public:
    Status(std::string name, int timeLeft, SDL_Rect sprite, int xpos, int ypos) :
        sprite(sprite), xpos(xpos), ypos(ypos), name(name), timeLeft(timeLeft) {}

    virtual Status      *clone() = 0;

    SDL_Rect            sprite;
    int                 xpos, ypos;
    std::string         name;

    int                 timeLeft;

    virtual void        updateHolder(StatusHolder*, Mob*) = 0;
};

#include "poison.hpp"
#include "regen.hpp"


#endif
