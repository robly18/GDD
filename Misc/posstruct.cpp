#include "posstruct.hpp"

int posDiff(const Pos &p1, const Pos &p2) {
    return (p1.x - p2.x) * (p1.x < p2.x ? -1 : 1) +
            (p1.y - p2.y) * (p1.y < p2.y ? -1 : 1);
}

Pos addPos(const Pos &p, const Pos &v) {
    return Pos{p.x+v.x, p.y+v.y};
}

bool posEq(const Pos &p1, const Pos &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool isUnitPos(const Pos &p) {
    return std::abs(p.x) + std::abs(p.y) == 1;
}
