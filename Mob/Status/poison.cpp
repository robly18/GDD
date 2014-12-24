#include "poison.hpp"

void Poison::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--; //todo experiment with this part
    int dmg = computePoisonDmg(user);
    if (holder->poisondmg < dmg) holder->poisondmg = dmg;
}

int FixedHpPoison::computePoisonDmg(Mob* mob) {
    return str;
}

Status* FixedHpPoison::clone() {
    return new FixedHpPoison(str, name, timeLeft);
}
