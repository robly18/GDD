#include "swordatks.hpp"

HitThru::HitThru(int dmg, int cost, int length) :
    TargetedAttack(dmg, cost, 1, 1), length(length) {
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
