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

Uint32 Attack::highlightColor(Mob* user, int mx, int my,
                              int mousex, int mousey) const {
    if (isInRange(user->x, user->y, mousex, mousey)) {
        if (mousex == mx && mousey == my) return 0xFF2525;
        if (isHit(user, mousex, mousey, mx, my)) return 0xEE2020;
    }
    if (isInRange(user->x, user->y, mx, my)) {
        return 0xDD1111;
    }
    if (mousex == mx && mousey == my) return 0xCF1515;
    return 0xCC0505;
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
            int dmg = (*mob)->destructible->damage(damage);
            static char buffer [255];
            sprintf(buffer, "%s hit %s for %i dmg %s %i hp",
                    src->name.c_str(),
                    src == *mob ? "themselves in idiocy" : (*mob)->name.c_str(),
                    dmg,
                    src == *mob ? "and now have" : "which now has",
                    (*mob)->destructible->hp);
            engine.ui->log->addMessage(buffer);
            if ((*mob)->destructible->isDead()) {
                (*mob)->destructible->die(*mob);
            }
        }
    }
    return hit;
}
