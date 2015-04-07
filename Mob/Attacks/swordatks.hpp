#ifndef SWORDATKS_H
#define SWORDATKS_H

#include "../../Main/main.hpp"

class HitThru : public TargetedAttack {
public:
    HitThru(int dmg, int cost, int length);

    int                 length;

    bool                isInRange(const Pos user, const Pos target, const int accy) const;
    bool                isHit(Mob*, int, int, int, int) const;

};

class BashSwp : public TargetedAttack {
public:
    BashSwp(int cost, int force);

    int                 force;


    bool                hit(Mob*, Mob*) const;
    bool                target(Mob*, int, int) const;
};

#endif
