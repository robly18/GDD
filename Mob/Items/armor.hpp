#ifndef ARMOR_H
#define ARMOR_H

#include "..\..\Main\main.hpp"


class Armor : public Item {
public:
    Armor(std::string n, int defense, int swiftness) : Item(n),
                        defense(defense), swiftness(swiftness)
                        {type = Itemtype::ARMOR;
                         size = 12;};

    int defense;
    int swiftness;

    bool use(Player*);
    bool unequip(Player*);
};

#endif
