#ifndef MOB_H
#define MOB_H

#include <string.h>
#include <string>
#include "..\Main\main.hpp"

#define POSRECT(x, y) (SDL_Rect{x*16,y*16,16,16})

class Ai;
class Destructible;
class Attack;
class Inventory;
class Weapon;

class Mob {
public:
    Actor           *a;
    int             x, y;
    std::string     name;

    Ai              *ai = NULL;
    Destructible    *destructible = NULL;
    Attack          *attack = NULL;
    Inventory       *inventory = NULL;

    Mob(int, int, SDL_Rect, SDL_Surface*, std::string);
    void            setPos(int, int);
    void            move(int, int);

    int             getSwiftness(); //Misleading name :( Actually faster if swiftness is lower
};

class Player : public Mob {
public:
    Player(int x, int y, SDL_Rect r, SDL_Surface *t, std::string n);


    Weapon          *weapon = NULL;

    Attack          *defaultattack;

    int             str = 10;
    int             getMaxMp() const;
};

#endif
