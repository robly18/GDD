#ifndef INV_H
#define INV_H

#include "item.hpp"
#include "gddlist.hpp"

class Item;

class Inventory {
public:
    virtual bool addItem(Item*) = 0;
    virtual Item* removeItem(Item*) = 0;
    virtual Item* removeItem(int) = 0;

    virtual Item* getItem(int) = 0;
    virtual Item* itemAtSlot(int) = 0; //lower level

    virtual int itemNo() = 0;

    virtual ~Inventory() {};
};

class PlayerInventory : public Inventory {
public:
    int filled = 0;

    bool addItem(Item*);
    Item* removeItem(Item*);
    Item* removeItem(int);

    Item* getItem(int);
    Item* itemAtSlot(int);

    int itemNo();


    PlayerInventory() {};
private:
    Item* items [30] = {};
};

class MobInventory : public Inventory {
public:
    bool addItem(Item*);
    Item* removeItem(Item*);
    Item* removeItem(int);

    Item* getItem(int i) {return itemAtSlot(i);};
    Item* itemAtSlot(int);

    int itemNo() {return items.length();}

    ~MobInventory() {};
private:
    gdd::List<Item*> items;
};

class FloorInventory {
public:
    FloorInventory(SDL_Surface* s, int x, int y) : x(x), y(y),
            a(new Actor(s, {0, 16+8, 16, 16},
                        {16*x, 16*y, 16, 16})) {};

    MobInventory items;

    int x, y;

    Actor* a;

    void checkExistance();
};

#endif
