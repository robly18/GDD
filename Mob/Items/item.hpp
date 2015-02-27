#ifndef ITEM_H
#define ITEM_H

#include <string.h>

#include "..\mob.hpp"
namespace Itemtype {
    enum {
        NONE = -1,
        USE = 0,
        ARMOR,
        WEAPON,
        RING,
    };
};

class Player;

class Item {
public:
    Item(std::string n) : name(n) {}
    std::string     name;
    int             size = 1;
    int             type = Itemtype::NONE;

    Uint32          color = 0xAAAAAA;

    virtual bool    use(Player*) = 0;
};


#include "armor.hpp"
#include "weapon.hpp"
#include "potion.hpp"

#endif
