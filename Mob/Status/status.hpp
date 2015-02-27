#ifndef STATUS_H
#define STATUS_H

#include "..\..\Main\main.hpp"
#include <list>
#include <memory>
/*
Have a few types of statuses, some good, some bad.

Good:
Block, Thorns, Ice skin
Regens
Quirks?
Potion effects

Bad:
Poison,
Frozen,
Fire?,
Stunned (can't attack),
Curse (more vulnerable to attacks)
*/
class Status;
class Attack;

enum SideEffect {
    NONE = 0,
    BLOCK = 0x01,
    THORN = 0x02,
    FROZEN = 0x04,
};

class StatusHolder {
public:
    StatusHolder() {}

    void                render(SDL_Surface*, SDL_Rect);
    void                update(Mob*);

    bool                pushStatus(Status*);
    bool                statusAt(int, int,  //Returns if there is status at pos
                                 std::list<Status*>::iterator*);
                                            //and changes pointer accordingly
    int                 poisondmg = 0;
    int                 healval = 0;

    int                 sideeffect = SideEffect::NONE;
    bool                hasEffect(SideEffect);

    std::shared_ptr
        <Status>        counterdebuff;

    bool                gotAttacked = false; //For statuses that change according to
                                            //if you got attacked or not
private:
    void                resetStats();
    std::list<Status*>  statuses;
};


class Status {
public:
    Status(std::string name, int timeLeft, SDL_Rect sprite,
                            int xpos, int ypos, bool stackable = true) :
        sprite(sprite), xpos(xpos), ypos(ypos), name(name),
                timeLeft(timeLeft), stackable(stackable) {}
    virtual             ~Status() {}

    virtual Status      *clone() = 0;

    SDL_Rect            sprite;
    int                 xpos, ypos; //The position of the sprite in a 4*4 grid
    std::string         name;       //It is what defines the 'type' of a status,
                                    //And if it will stack with another one or not
    int                 timeLeft;

    bool                stackable;

    virtual void        updateHolder(StatusHolder*, Mob*) = 0;
};

#include "poison.hpp"
#include "frozen.hpp"
#include "regen.hpp"
#include "blockingbuffs.hpp"


#endif
