#ifndef DESTRUCTIBLE_H
#define DESTRUCTIBLE_H

#include <SDL.h>
#include <stdio.h>
#include "mob.hpp"

class Mob;
class Armor;
class StatusHolder;

class Destructible {
public:
    Destructible(int, SDL_Rect);

    int                 maxHp;
    int                 hp;

    int                 def = STARTDEF;

    Armor               *armor;
    StatusHolder        *statusholder;

    inline bool         isDead() {return hp <= 0;}
    const SDL_Rect      deadSprite;

    virtual void        die(Mob*) = 0;

    int                 damage(int);
    bool                heal(int);

    void                dropItems(Mob*);
};

class PlayerDestructible : public Destructible {
public:
    PlayerDestructible(int, SDL_Rect);

    void die(Mob*);
};

class MobDestructible : public Destructible {
public:
    int             xp;
    void            die(Mob*);

    MobDestructible(int hp, SDL_Rect, int xp);
};

#endif
