#include "attack.hpp"

bool Attack::select(Player* user) const {
    if (cost == 0) return true;
    if (user->weapon->mana < cost) {
        return false;
    }
    user->weapon->mana -= cost;
    user->attack = this;
    return true;
}

bool Attack::applyChances(Mob* user) const {
    if (!chances.size()) return false;
    bool affected = false;
    for (std::vector<StatusChance>::const_iterator s = chances.begin();
                                                    s != chances.end(); s++) {
        int num = std::rand() % 100;
        if (num < s->chance) {
            affected = true;
            user->destructible->statusholder->pushStatus(s->status->clone());
        }
    }
    return affected;
}

/**TargetedAttack**/

Uint32 TargetedAttack::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->getPos(), {mousex, mousey}, user->accy)) {
        if (mousex == mx && mousey == my) return validMouseOnColor;
        if (isHit(user, mousex, mousey, mx, my)) return willGetHitColor;
    }
    if (isInRange(user->getPos(), {mx, my}, user->accy)) {
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
}

bool TargetedAttack::isInRange(Pos user, Pos target, int accy) const {
    int dx = std::abs(user.x-target.x);
    int dy = std::abs(user.y-target.y);

    int tmpmaxrange; //After accuracy shenanigans have been computed
    if (accy >= maxAccy) {
        tmpmaxrange = maxrange;
    } else if (accy < minAccy) {
        return false;
    } else {
        tmpmaxrange = accy * (maxrange-minrange)/(maxAccy-minAccy) + minrange
                        - (maxrange-minrange)*minAccy/(maxAccy-minAccy); //complicated math plz ignore
    }

    return engine.map->hasBeenSeen(target.x, target.y) &&
            minrange<=(dx+dy) &&
            engine.map->fovcomputer->isInSight(user.x, user.y,
                                               target.x, target.y,tmpmaxrange);
}

bool TargetedAttack::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return  (targetx - mx) * (targetx < mx ? -1 : 1) +
            (targety - my) * (targety < my ? -1 : 1) <= radius;
}

bool TargetedAttack::target(Mob* src, int x, int y) const {
    if (src->destructible->isDead()) return true;
    if (!isInRange(src->getPos(), {x, y}, src->accy)) return false;
    for (auto m : engine.map->mobs2) {
        if (isHit(src, x, y, m->x, m->y) &&
            m->destructible &&
            !(!hurtself && m == src)) {

            hit(src, m);

        }
    }
    return true;
}

bool TargetedAttack::hit(Mob* src, Mob* target) const {

    if (damage) {
        int dmg = target->destructible->damage(damage * src->atk /STARTATK);
        DEBUGMSG("Logging messages is my fame");
        if (physical) {
            if (target->destructible->statusholder->hasEffect(SideEffect::THORN)) {
                dmg = src->destructible->damage(dmg);
                if (src->destructible->isDead()) {
                    src->destructible->die(src);
                }
            }

            std::shared_ptr<Status>
                counterdebuff = target->destructible->statusholder->counterdebuff;
            if (counterdebuff) {
                src->destructible->statusholder->pushStatus(counterdebuff->clone());
            }
        }
    }

    if (applyChances(target)) {
    }
    if (target->destructible->isDead()) {
        target->destructible->die(target);
    }

    return true;
}

/**SelfBuff**/

Uint32 SelfBuff::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->getPos(), {mx, my}, user->accy)) {
        if (user->x == mousex && user->y == mousey) {
            return validMouseOnColor;
        }
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
} //MERRY CHRISTMAS FROM 24 DEC 2014

bool SelfBuff::isInRange(Pos p, Pos m, int accy) const {
    return p.x == m.x && p.y == m.y;
}

bool SelfBuff::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return targetx == mx && targety == my;
}

bool SelfBuff::target(Mob* src, int x, int y) const {
    if (!isInRange(src->getPos(), {x, y}, src->accy)) return false;
    hit(src, src);
    return true;
}

bool SelfBuff::hit(Mob* src, Mob* target) const {
    applyChances(src);
    return true;
}
