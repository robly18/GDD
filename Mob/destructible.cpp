#include "destructible.hpp"

Destructible::Destructible(int maxHp, SDL_Rect deadSprite) :
    maxHp(maxHp), hp(maxHp), deadSprite(deadSprite),
    statusholder(new StatusHolder) {
}

PlayerDestructible::PlayerDestructible(int maxHp, SDL_Rect deadSprite) :
    Destructible(maxHp, deadSprite) {
}

MobDestructible::MobDestructible(int maxHp, SDL_Rect deadSprite) :
    Destructible(maxHp, deadSprite) {
}

int Destructible::damage(int dmg) {
    int defense = armor ? armor->defense : 0;
    if (dmg > defense) {
        hp -= dmg - defense;
        return dmg - defense;
    } else {
        hp--;
        return 1;
    }
}

bool Destructible::heal(int heal) {
    if (hp == maxHp) return false;
    if (hp + heal > maxHp) hp = maxHp;
    else hp += heal;
    return true;
}

void Destructible::die(Mob* mob) {
    mob->a->r = deadSprite;
    delete mob->ai;
    mob->ai = NULL;
    dropItems(mob);

    for (Mob** m = engine.map->mobs2.begin(); m != engine.map->mobs2.end(); m++) {
        if (*m == mob) {
            engine.map->mobs2.remove(m);
            break;
        }
    }
    engine.map->mobs1.push(mob);

    char buffer [64];
    sprintf(buffer, "The %s died!", mob->name.c_str());
    engine.ui->log->addMessage(buffer);
}

void Destructible::dropItems(Mob* mob) {
    if (!mob->inventory) return;
    engine.map->addInvTo(mob->inventory, mob->x, mob->y);
    delete mob->inventory;
}

void PlayerDestructible::die(Mob* mob) {
    mob->a->r = deadSprite;
    delete mob->ai;
    mob->ai = NULL;
    engine.state = engine.State::DEAD;
}
