#include "frozen.hpp"

Status* Frozen::clone() const {
    return new Frozen(name, timeLeft);
}

void Frozen::updateHolder(StatusHolder* holder, Mob* mob) {
    timeLeft--;
    holder->sideeffect |= SideEffect::FROZEN;
}
