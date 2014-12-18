#include "weapon.hpp"

bool Weapon::use(Player* user) { //tpdo fix this
    Weapon* tmpa = user->weapon;
    user->weapon = this;
    if (tmpa) {
         if (!user->inventory->addItem(tmpa)) {
            engine.map->dropItem(tmpa, user->x, user->y);
         }
    }
    mana = 0;
    return true;
}

bool Weapon::unequip(Mob* user) {
    if (user->inventory->addItem(this)) return true;
    engine.map->dropItem(this, user->x, user->y);
    return false;
}

bool Weapon::selectAttack(Player* user, int atk) {
    cancelAttack(user);
    return attacks[atk]->select(user);
}

void Weapon::cancelAttack(Player* user) {
    if (user->attack) {
        engine.map->player->weapon->mana += engine.map->player->attack->cost;
        engine.map->player->attack = NULL;
    }
}

/**Staff**/

Staff::Staff(std::string n, int attack, int maxmana) :
    Weapon(n, attack, maxmana) {
    color = 0xFFCC88;
    attacks[0] = new TargetedAttack(10, 10, 0, 6);
    attacks[1] = new TargetedAttack(4, 5, 0, 5, 3);
}

int Staff::regenMana(int regen) {
    mana += regen;
    if (mana > engine.map->player->getMaxMp()) {
        mana = engine.map->player->getMaxMp();
    }
    return mana;
}

/**Sword**/

Sword::Sword(std::string n, int attack, int maxmana) :
    Weapon(n, attack, maxmana) {
    color = 0x2222DD;
    attacks[0] = new TargetedAttack(20, 15, 0, 1);
    attacks[1] = new TargetedAttack(15, 10, 0, 0, 3, false);
}

int Sword::regenMana(int regen) {
    mana += regen;
    if (mana > engine.map->player->getMaxMp()) {
        mana = engine.map->player->getMaxMp();
    }
    return mana;
}

/**Bow**/

Bow::Bow(std::string n, int attack, int maxmana) :
    Weapon(n, attack, maxmana) {
    color = 0x12DD12;
    attacks[0] = new TargetedAttack(2, 3, 3, 10);
    attacks[1] = new TargetedAttack(15, 10, 3, 9, 2);
}

int Bow::regenMana(int regen) {
    mana += regen;
    if (mana > engine.map->player->getMaxMp()) {
        mana = engine.map->player->getMaxMp();
    }
    return mana;
}
