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
    if (isInRange(user->x, user->y, mousex, mousey)) {
        if (mousex == mx && mousey == my) return validMouseOnColor;
        if (isHit(user, mousex, mousey, mx, my)) return willGetHitColor;
    }
    if (isInRange(user->x, user->y, mx, my)) {
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
}

bool TargetedAttack::isInRange(int px, int py, int mx, int my) const {
    int dx = (px-mx) * ((px>mx) * 2 - 1);
    int dy = (py-my) * ((py>my) * 2 - 1);
    return minrange<=(dx+dy) && (dx+dy)<=maxrange;
}

bool TargetedAttack::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return  (targetx - mx) * (targetx < mx ? -1 : 1) +
            (targety - my) * (targety < my ? -1 : 1) <= radius;
}

bool TargetedAttack::target(Mob* src, int x, int y) const {
    if (!isInRange(src->x, src->y, x, y)) return false;
    bool hit = false;
    for (Mob** mob = engine.map->mobs2.begin(); mob != engine.map->mobs2.end(); mob++) {
        if (isHit(src, x, y, (*mob)->x, (*mob)->y) &&
            (*mob)->destructible &&
            !(!hurtself && *mob == src)) {
            hit = true;
            static char buffer [255];
            int dmg = (*mob)->destructible->damage(damage);

            sprintf(buffer, "%s hit %s for %i dmg %s %i hp",
                    src->name.c_str(),
                    src == *mob ? "themselves in idiocy" : (*mob)->name.c_str(),
                    dmg,
                    src == *mob ? "and now have" : "which now has",
                    (*mob)->destructible->hp);
            engine.ui->log->addMessage(buffer);

            if (applyChances(*mob, buffer, "%s was afflicted by", "")) engine.ui->log->addMessage(buffer);
            if ((*mob)->destructible->isDead()) {
                (*mob)->destructible->die(*mob);
            }
        }
    }
    return hit;
}

/**SelfBuff**/

Uint32 SelfBuff::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->x, user->y, mx, my)) {
        if (user->x == mousex && user->y == mousey) {
            return validMouseOnColor;
        }
        return isInRangeColor;
    }
    if (mousex == mx && mousey == my) return invalidMouseOnColor;
    return outOfRangeColor;
} //MERRY CHRISTMAS FROM 24 DEC 2014

bool SelfBuff::isInRange(int px, int py, int mx, int my) const {
    return px == mx && py == my;
}

bool SelfBuff::isHit(Mob* user, int targetx, int targety, int mx, int my) const {
    return targetx == mx && targety == my;
}

bool SelfBuff::target(Mob* src, int x, int y) const {
    if (!isInRange(src->x, src->y, x, y)) return false;
    char buffer [255];
    if (applyChances(src, buffer, "%s has applied", "on themselves"))
                    engine.ui->log->addMessage(buffer);
    return true;
}
