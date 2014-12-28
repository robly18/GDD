#include "frozen.hpp"

Status* Frozen::clone() {
    return new Frozen(name, timeLeft);
}

void Frozen::updateHolder(StatusHolder* holder, Mob* mob) {
    timeLeft--;
    *(int*)&holder->sideeffect |= (int) SideEffect::FROZEN;
}
