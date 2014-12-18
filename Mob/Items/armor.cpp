#include "armor.hpp"


bool Armor::use(Player* user) { //tpdo fix this
    Armor* tmpa = user->destructible->armor;
    user->destructible->armor = this;
    if (tmpa) {
         if (!user->inventory->addItem(tmpa)) {
            engine.map->dropItem(tmpa, user->x, user->y);
         }
    }
    return true;
}

bool Armor::unequip(Player* user) {
    if (user->inventory->addItem(this)) return true;
    engine.map->dropItem(this, user->x, user->y);
    return false;
}
