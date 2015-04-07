#include "weapon.hpp"

bool Weapon::use(Player* user) { //tpdo fix this
    engine.map->player->attack = nullptr;

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
    engine.map->player->attack = nullptr;

    if (user->inventory->addItem(this)) return true;
    engine.map->dropItem(this, user->x, user->y);
    return false;
}

void Weapon::cancelAttack(Player* user) {
    if (user->attack) {
        mana += engine.map->player->attack->cost;
        if (mana > engine.map->player->getMaxMp()) {
            mana = engine.map->player->getMaxMp();
        }
        engine.map->player->attack = NULL;
    }
}

/**Staff**/

Staff::Staff(std::string n, int attack, int maxmana, int minaccy, int maxaccy) :
    Weapon(n, attack, maxmana, WEAPONSTFF) {
    color = 0xFFCC88;
    attacks[0] = new TargetedAttack(10, 10, 0, 6, minaccy, maxaccy, 0, false, true);
    attacks[1] = new TargetedAttack(4, 5, 0, 5, minaccy, maxaccy, 3, false, true);
    defenses[0] = new SelfBuff(10, new SideEffectBuff("Thorn", 5, SideEffect::THORN,
                                                      14, 32));
    defenses[1] = new SelfBuff(20, new FixedHpRegen(5, "Regen", 8));
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
    Weapon(n, attack, maxmana, WEAPONSWRD) {
    color = 0x2222DD;
    attacks[0] = (new TargetedAttack(30, 15, 0, 1))->setName("HIT")->setIcon(SDL_Rect{16,0,8,8});
    attacks[1] = (new TargetedAttack(30, 17, 0, 0, 0, 0, 2, true, false))
                    ->setName("SPIN");
    attacks[2] = (new HitThru(20, 10, 3))
                    ->setName("HITTHRU");
    defenses[0] = (new SelfBuff(12, new SideEffectBuff("Block", 2, SideEffect::BLOCK,
                                                      0, 30)))->setName("BLOCK");
    defenses[1] = (new SelfBuff(25, new FixedHpRegen(7, "Regen", 2)))->setName("REGEN");
    defenses[2] = (new BashSwp(10, -1))->setName("BASHSWP");
}

int Sword::regenMana(int regen) {
    int maxmp = engine.map->player->getMaxMp();

    int manaregen = regen * (maxmp - mana)/maxmp;
    if (manaregen == 0) manaregen = 1;

    mana += manaregen;
    if (mana > maxmp) {
        mana = maxmp;
    }
    return mana;
}

/**Bow**/

Bow::Bow(std::string n, int attack, int maxmana, int minaccy, int maxaccy) :
    Weapon(n, attack, maxmana, WEAPONBOW) {
    color = 0x12DD12;
    attacks[0] = new TargetedAttack(2, 3, 3, 10, minaccy, maxaccy, 0, false, false);
    attacks[0]->chances.push_back(new StatusChance{new FixedHpPoison(10,"Poison",3), 100});
    attacks[1] = new TargetedAttack(15, 10, 3, 9, minaccy, maxaccy, 2, false, true);
    defenses[0] = new SelfBuff(35, new CounterDebuffBuff("IceSkin", 5,
                                        std::shared_ptr<Status>(new Frozen("Freeze", 5)),
                                        0, 0));
    defenses[1] = new SelfBuff(40, new FixedHpRegen(1, "Regen", 20));
}

int Bow::regenMana(int regen) {
    mana += regen;
    if (mana > engine.map->player->getMaxMp()) {
        mana = engine.map->player->getMaxMp();
    }
    return mana;
}
