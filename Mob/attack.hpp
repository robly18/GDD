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
    bool                applyChances(Mob*, char*, const char*, const char*) const;

    virtual bool        isInRange(int, int, int, int) const = 0;
    virtual bool        isHit(Mob*, int, int, int, int) const = 0;

    virtual Uint32      highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const = 0;

    std::vector
        <StatusChance*> chances;
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
    SelfBuff(int cost) :
        Attack(cost) {}

    bool                target(Mob*, int, int) const;

    bool                isInRange(int, int, int, int) const;
    bool                isHit(Mob*, int, int, int, int) const;

    Uint32              highlightColor(Mob*, int mobx, int moby,
                                       int mousex, int mousey) const;

private:
    Uint32              isInRangeColor = 0x1111DD,
                        outOfRangeColor = 0x0505DD,
                        validMouseOnColor = 0x2525FF,
                        invalidMouseOnColor = 0x1515CF;
};


#endif
