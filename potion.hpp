#ifndef POTION_H
#define POTION_H

#include "item.hpp"

class Potion : public Item {
public:
    Potion(std::string n) : Item(n) {type = Itemtype::USE;}
};

class HpPotion : public Potion {
public:
    HpPotion(std::string n, int heals) : Potion(n), heals(heals)
                                    {color = 0xFFAAAA;}

    int heals;

    bool use(Player*);
};

#endif
