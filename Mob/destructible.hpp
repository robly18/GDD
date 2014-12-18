#ifndef DESTRUCTIBLE_H
#define DESTRUCTIBLE_H

#include <SDL.h>
#include <stdio.h>
#include "mob.hpp"

class Mob;
class Armor;

class Destructible {
public:
    Destructible(int, SDL_Rect);

    int maxHp;
    int hp;

    Armor* armor;

    inline bool isDead() {return hp <= 0;}

    const SDL_Rect deadSprite;

    virtual void die(Mob*);

    int damage(int);
    bool heal(int);

    void dropItems(Mob*);
};

class PlayerDestructible : public Destructible {
public:
    PlayerDestructible(int, SDL_Rect);

    void die(Mob*);
};

class MobDestructible : public Destructible {
public:
    MobDestructible(int, SDL_Rect);
};

#endif
