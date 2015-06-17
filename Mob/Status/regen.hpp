#ifndef REGEN_H
#define REGEN_H

#include "status.hpp"

class Regen : public Status {
public:
    Regen(std::string name, int timeLeft, SDL_Rect r) :
        Status(name, timeLeft, r, 3, 1, false) {}

    void                updateHolder(StatusHolder*, Mob*);

    virtual int         computeHeals(Mob*) = 0;
};

class FixedHpRegen : public Regen {
public:
    FixedHpRegen(int str, std::string name, int timeLeft) :
        Regen(name, timeLeft, SDL_Rect{12,14,4,4}), str(str) {}
    Status*             clone() const;

    int                 computeHeals(Mob*);
private:
    int str;
};


#endif
