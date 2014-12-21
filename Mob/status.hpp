#ifndef STATUS_H
#define STATUS_H

#include "..\Main\main.hpp"
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

class StatusHolder {
public:
    StatusHolder() {}

    std::list<Status*>  statuses;

    void                render(SDL_Surface*, SDL_Rect);
    void                update(Mob*);

    int                 poisondmg = 0;
};


class Status {
public:
    Status(std::string name, int timeLeft, SDL_Rect sprite) :
        sprite(sprite), name(name), timeLeft(timeLeft) {}

    virtual Status      *clone() = 0;

    SDL_Rect            sprite;
    std::string         name;

    int                 timeLeft;

    virtual void        updateHolder(StatusHolder*, Mob*) = 0;
};

class Poison : public Status {
public:
    Poison(std::string name, int timeLeft) :
        Status(name, timeLeft, SDL_Rect{0,32,4,4}) {}

    void                updateHolder(StatusHolder*, Mob*);

    virtual int         computePoisonDmg(Mob*) = 0;
};

class FixedHpPoison : public Poison {
public:
    FixedHpPoison(int str, std::string name, int timeLeft) :
        Poison(name, timeLeft), str(str) {}
    Status             *clone();

    int                 computePoisonDmg(Mob*);
private:
    int                 str;
};


#endif
