#include "attack.hpp"

bool Attack::select(Player* user) {
    if (cost == 0) return true;
    if (user->weapon->mana < cost) {
        return false;
    }
    user->weapon->mana -= cost;
    user->attack = this;
    return true;
}

bool Attack::applyChances(Mob* user, char* buffer, const char* begin, const char* end) const {
    if (!chances.size()) return false;
    bool affected = false;
    sprintf(buffer, begin, user->name.c_str());
    for (std::vector<StatusChance*>::const_iterator s = chances.begin();
                                                    s != chances.end(); s++) {
        int num = std::rand() % 100;
        if (num < (*s)->chance) {
            sprintf(buffer, "%s%s %s", buffer, affected ? "," : "",
                    (*s)->status->name.c_str());
            affected = true;
            user->destructible->statusholder->pushStatus((*s)->status->clone());
        }
    }
    sprintf(buffer, "%s %s", buffer, end);
    return affected;
}

/**TargetedAttack**/

Uint32 TargetedAttack::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->x, user->y, mousex, mousey, user->accy)) {
        if (mousex == mx && mousey == my) return validMouseOnColor;
        if (isHit(user, mousex, mousey, mx, my)) return willGetHitColor;
    }
    if (isInRange(user->x, user->y, mx, my, user->accy)) {
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
}

bool TargetedAttack::isInRange(int px, int py, int mx, int my, int accy) const {
    int dx = std::abs(px-mx);
    int dy = std::abs(py-my);

    int tmpmaxrange; //After accuracy shenanigans have been computed
    if (accy >= maxAccy) {
        tmpmaxrange = maxrange;
    } else if (accy < minAccy) {
        return false;
    } else {
        tmpmaxrange = accy * (maxrange-minrange)/(maxAccy-minAccy) + minrange
                        - (maxrange-minrange)*minAccy/(maxAccy-minAccy); //complicated math plz ignore
    }

    return engine.map->hasBeenSeen(mx, my) &&
            minrange<=(dx+dy) &&
            engine.map->fovcomputer->isInSight(px, py, mx, my,tmpmaxrange);
}

bool TargetedAttack::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return  (targetx - mx) * (targetx < mx ? -1 : 1) +
            (targety - my) * (targety < my ? -1 : 1) <= radius;
}

bool TargetedAttack::target(Mob* src, int x, int y) const {
    if (!isInRange(src->x, src->y, x, y, src->accy)) return false;
    bool didhit = false;
    for (std::list<Mob*>::iterator mob = engine.map->mobs2.begin(); mob != engine.map->mobs2.end(); mob++) {
        if (isHit(src, x, y, (*mob)->x, (*mob)->y) &&
            (*mob)->destructible &&
            !(!hurtself && *mob == src)) {
            didhit = true;
            hit(src, *mob);
        }
    }
    return didhit;
}

bool TargetedAttack::hit(Mob* src, Mob* target) const {
    static char buffer [255];

    if (damage) {
        int dmg = target->destructible->damage(damage + damage * src->atk /10);

        sprintf(buffer, "%s hit %s for %i dmg %s %i hp",
                src->name.c_str(),
                src == target ? "themselves in idiocy" : target->name.c_str(),
                dmg,
                src == target ? "and now have" : "which now has",
                target->destructible->hp);
        engine.ui->log->addMessage(buffer);

        if (physical) {
            if (target->destructible->statusholder->hasEffect(SideEffect::THORN)) {
                dmg = src->destructible->damage(dmg);
                sprintf(buffer, "%s's thorns caused %s to be damaged for %i hp",
                        target->name.c_str(),
                        src->name.c_str(),
                        dmg);
                engine.ui->log->addMessage(buffer);
                if (src->destructible->isDead()) {
                    src->destructible->die(src);
                }
            }

            std::shared_ptr<Status>
                counterdebuff = target->destructible->statusholder->counterdebuff;
            if (counterdebuff) {
                sprintf(buffer, "%s was Frozen by %s's IceSkin",
                        src->name.c_str(), target->name.c_str());
                engine.ui->log->addMessage(buffer);
                src->destructible->statusholder->pushStatus(counterdebuff->clone());
            }
        }
    }

    if (applyChances(target, buffer, "%s was afflicted by", "")) engine.ui->log->addMessage(buffer);
    if (target->destructible->isDead()) {
        target->destructible->die(target);
    }

    return true;
}

/**SelfBuff**/

Uint32 SelfBuff::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->x, user->y, mx, my, user->accy)) {
        if (user->x == mousex && user->y == mousey) {
            return validMouseOnColor;
        }
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
} //MERRY CHRISTMAS FROM 24 DEC 2014

bool SelfBuff::isInRange(int px, int py, int mx, int my, int accy) const {
    return px == mx && py == my;
}

bool SelfBuff::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return targetx == mx && targety == my;
}

bool SelfBuff::target(Mob* src, int x, int y) const {
    if (!isInRange(src->x, src->y, x, y, src->accy)) return false;
    hit(src, src);
    return true;
}

bool SelfBuff::hit(Mob* src, Mob* target) const {
    char buffer [255];
    if (applyChances(src, buffer, "%s has applied", "on themselves"))
                    engine.ui->log->addMessage(buffer);
    return true;
}
