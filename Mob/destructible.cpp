#include "destructible.hpp"

Destructible::Destructible(int maxHp, SDL_Rect deadSprite) :
    maxHp(maxHp), hp(maxHp), armor(nullptr), statusholder(new StatusHolder),
    deadSprite(deadSprite) {
}

PlayerDestructible::PlayerDestructible(int maxHp, SDL_Rect deadSprite) :
    Destructible(maxHp, deadSprite) {
}

MobDestructible::MobDestructible(int maxHp, SDL_Rect deadSprite, int xp) :
    Destructible(maxHp, deadSprite), xp(xp) {
}

int Destructible::damage(int dmg) {
    statusholder->gotAttacked = true;
    int defense = armor ? armor->defense * def / STARTDEF : 0;
    if (!statusholder->hasEffect(SideEffect::BLOCK) &&
        dmg > defense) {
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

void Destructible::dropItems(Mob* mob) {
    if (!mob->inventory) return;
    if (mob->inventory->itemNo() > 0)
        engine.map->addInvTo(mob->inventory, mob->x, mob->y);
    delete mob->inventory;
}

void PlayerDestructible::die(Mob* mob) {
    //engine.state = engine.State::DEAD;
    engine.ui->addPrompt(new DeadPrompt);
}

void MobDestructible::die(Mob* mob) {
    char buffer [64];
    sprintf(buffer, "The %s died!", mob->name.c_str());
    engine.ui->log->addMessage(buffer);

    mob->name+="(DEAD)";

    engine.map->player->xpholder->levelUp(xp,
                                          engine.map->player->weapon ?
                                            engine.map->player->weapon->weapontype : -1);

    sprintf(buffer, "The player gained %i XP!", xp);
    engine.ui->log->addMessage(buffer);
}





