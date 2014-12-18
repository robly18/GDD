#include "potion.hpp"

bool HpPotion::use(Player* user) {
    return user->destructible->heal(heals);
}
