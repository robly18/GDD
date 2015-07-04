#ifndef ATTACK_H
#define ATTACK_H

#include "..\Main\main.hpp"

#include <stdio.h>
#include <vector>

#include <random>

class Status;

struct StatusChance {
    const Status        *status;
    int                 chance; //As a percent
};

class Attack {
public:
    Attack(int cost, std::string name = "", SDL_Rect icon = {0,0,0,0},
                                bool physical = true, int minAccy = 0, int maxAccy = 0,
                                                std::vector<StatusChance> chances = {}) :
        name(name), icon(icon), cost(cost),
        physical(physical),
        minAccy(minAccy), maxAccy(maxAccy),
        chances(chances) {};
    virtual             ~Attack() {};

    std::string         name = "";

    SDL_Rect            icon = {0, 0, 0, 0};

    int                 cost;
    bool                physical;

    int                 minAccy;
    int                 maxAccy;

    virtual bool        target(Mob*, int, int) const = 0;
    bool                target(Mob* mob, const Pos p) const
                                {return target(mob,p.x,p.y);}

    virtual bool        hit(Mob*, Mob*) const = 0;

    bool                select(Player*) const;
    bool                applyChances(Mob*) const;

    virtual bool        isInRange(const Pos user, const Pos target, const int) const = 0;
    virtual bool        isHit(Mob*, int, int, int, int) const = 0;

    virtual Uint32      highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const = 0;

    std::vector
        <StatusChance> chances;
};

class TargetedAttack : public Attack {
public:
    TargetedAttack(int damage, int cost, std::string name, SDL_Rect icon,
              int minrange, int maxrange, int minAccy = 0, int maxAccy = 0,
              int radius = 0, bool physical = true, bool hurtself = false,
                                    std::vector<StatusChance> chances = {}) :
        Attack(cost, name, icon, physical, minAccy, maxAccy, chances),
        damage(damage), minrange(minrange), maxrange(maxrange),
        radius(radius), hurtself(hurtself) {}

    int                 damage;

    int                 minrange, maxrange;

    int                 radius;
    bool                hurtself;

    bool                isInRange(const Pos user, const Pos target, const int accy) const;
    bool                isHit(Mob*, int, int, int, int) const;

    bool                target(Mob*, int, int) const;
    bool                hit(Mob*, Mob*) const;

    Uint32              highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const;

private:
    Uint32              validMouseOnColor = 0xFF2525,
                        invalidMouseOnColor = 0xCF1515,
                        willGetHitColor = 0xEE2020,
                        isInRangeColor = 0xDD1111,
                        outOfRangeColor = 0xCC0505;
};

class SelfBuff : public Attack {
public:
    SelfBuff(int cost, std::string name, SDL_Rect icon, const Status* s) :
        Attack(cost, name, icon, false) {chances.push_back(StatusChance{s, 100});}

    bool                target(Mob*, int, int) const;
    bool                hit(Mob*, Mob*) const;

    bool                isInRange(Pos, Pos, int) const;
    bool                isHit(Mob*, int, int, int, int) const;

    Uint32              highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const;

private:
    Uint32              isInRangeColor = 0x1111DD,
                        outOfRangeColor = 0x0505DD,
                        validMouseOnColor = 0x2525FF,
                        invalidMouseOnColor = 0x1515CF;
};

#include "Attacks/swordatks.hpp"


#endif

