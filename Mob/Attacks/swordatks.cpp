#include "swordatks.hpp"

HitThru::HitThru(int dmg, std::string name, SDL_Rect icon, int cost, int length) :
    TargetedAttack(dmg, cost, name, icon, 1, 1), length(length) {
}

bool HitThru::isInRange(const Pos user, const Pos target, const int accy) const {
    return (std::abs(user.x - target.x) +
            std::abs(user.y - target.y) == 1);
}

bool HitThru::isHit(Mob* src, int tx, int ty, int hx, int hy) const {
    Pos tdiff = {src->x - tx, src->y - ty};
    Pos hdiff = {src->x - hx, src->y - hy};
    assert(std::abs(tdiff.x) <= 1 && std::abs(tdiff.y) <= 1);
    if (!engine.map->fovcomputer->isInSight(src->x, src->y, hx, hy, length)) return false;
    if (tdiff.x == 0) {
        return hdiff.x == 0 &&
                hdiff.y * tdiff.y > 0;
    } else {
        return hdiff.y == 0 &&
                hdiff.x * tdiff.x > 0;
    }
}

BashSwp::BashSwp(int cost, std::string name, SDL_Rect icon, int force) :
        TargetedAttack(0, cost, name, icon, 1, 1), force(force) {
}

bool BashSwp::target(Mob *user, int x, int y) const {
    if (!isInRange(user->getPos(), {x, y}, user->accy)) return false;
    Pos diff = {x-user->x, y-user->y};
    for (int i = 0; i != force; i++) {
        bool hitSomeone = false;
        for (auto m : engine.map->mobs2) {
            if (isHit(user, x, y, m->x, m->y)) {
                if (hit(user, m)) {
                    hitSomeone = true;
                    x+=diff.x; y+=diff.y;
                }
            }
        }
        if (!hitSomeone) break;
    }
    return true;
}

bool BashSwp::hit(Mob *user, Mob *target) const {
    return target->swapWith(*user);
}



