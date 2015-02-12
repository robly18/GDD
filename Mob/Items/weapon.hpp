#ifndef WEAPON_H
#define WEAPON_H

#include "item.hpp"

class Attack;

enum {
    WEAPONSWRD = 0,
    WEAPONSTFF = 1,
    WEAPONBOW = 2,
};

class Weapon : public Item {
public:
    Weapon(std::string n, int attack, int maxmana, int w) : Item(n), attack(attack),
                    maxmana(maxmana), weapontype(w) {type = Itemtype::WEAPON;
                                        size = 8;}

    int             attack;

    Attack          *attacks [3];
    Attack          *defenses [3];

    bool            use(Player*);
    bool            unequip(Mob*);
    bool            selectAttack(Player*, int);
    void            cancelAttack(Player*);

    int             mana = 0;
    int             maxmana = 0;

    virtual int     regenMana(int regen) = 0; //todo fix

    int             weapontype;
};

class Staff : public Weapon {
public:
    Staff(std::string n, int attack, int maxmana);

    int regenMana(int regen);
};

class Sword : public Weapon {
public:
    Sword(std::string n, int attack, int maxmana);

    int regenMana(int regen);
};

class Bow : public Weapon {
public:
    Bow(std::string n, int attack, int maxmana);

    int regenMana(int regen);
};


#endif
