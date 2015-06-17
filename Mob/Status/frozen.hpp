#ifndef FROZEN_H
#define FROZEN_H

#include "status.hpp"

class Frozen : public Status {
public:
    Frozen(std::string name, int timeLeft) :
        Status(name, timeLeft, SDL_Rect{30, 14, 4, 4}, 0, 1) {}

    Status*             clone() const;

    void                updateHolder(StatusHolder*, Mob*);
};

#endif
