#ifndef POISON_H
#define POISON_H

#include "status.hpp"

class Poison : public Status {
public:
    Poison(std::string name, int timeLeft) :
        Status(name, timeLeft, SDL_Rect{0,32,4,4}, 0, 0) {}

    void                updateHolder(StatusHolder*, Mob*);

    virtual int         computePoisonDmg(Mob*) = 0;
};

class FixedHpPoison : public Poison {
public:
    FixedHpPoison(int str, std::string name, int timeLeft) :
        Poison(name, timeLeft), str(str) {}
    Status             *clone() const;

    int                 computePoisonDmg(Mob*);
private:
    int                 str;
};

#endif
