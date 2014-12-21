#ifndef ATTACK_H
#define ATTACK_H

#include "..\Main\main.hpp"

#include <stdio.h>
#include <vector>

#include <random>

class Status;

struct StatusChance {
    Status              *status;
    int                 chance; //As a percent
};

class Attack {
public:
    Attack(int cost) : cost(cost) {};
    virtual             ~Attack() {};

    int                 cost;

    virtual bool        target(Mob*, int, int) const = 0;
    bool                select(Player*);

    virtual bool        isInRange(int, int, int, int) const = 0;
    virtual bool        isHit(Mob*, int, int, int, int) const = 0;

    Uint32              highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const;
};

class TargetedAttack : public Attack {
public:
    TargetedAttack(int damage, int cost,
              int minrange, int maxrange, int radius = 0, bool hurtself = true) :
        Attack(cost), damage(damage), minrange(minrange), maxrange(maxrange),
        radius(radius), hurtself(hurtself) {}

    int                 damage;

    int                 minrange, maxrange;

    int                 radius;
    bool                hurtself;

    bool                isInRange(int, int, int, int) const;
    bool                isHit(Mob*, int, int, int, int) const;

    bool                target(Mob*, int, int) const;

    std::vector
        <StatusChance*> chances;
};


#endif

