#include "regen.hpp"


void Regen::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    int heals = computeHeals(user);
    holder->healval += heals;
}


Status* FixedHpRegen::clone() {
    return new FixedHpRegen(str, name, timeLeft);
}

int FixedHpRegen::computeHeals(Mob* user) {
    return str;
}
