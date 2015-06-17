#include "regen.hpp"


void Regen::updateHolder(StatusHolder* holder, Mob* user) {

    if (holder->gotAttacked) {
        timeLeft = 0;
        return;
    }

    timeLeft--;
    int heals = computeHeals(user);
    holder->healval += heals;
}


Status* FixedHpRegen::clone() const {
    return new FixedHpRegen(str, name, timeLeft);
}

int FixedHpRegen::computeHeals(Mob* user) {
    return str;
}
