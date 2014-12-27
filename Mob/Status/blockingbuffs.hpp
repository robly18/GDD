#ifndef BLOCKINGBUFFS_H
#define BLOCKINGBUFFS_H

#include "status.hpp"

class SideEffectBuff : public Status {
public:
    SideEffectBuff(std::string name, int timeLeft, int effects, int spritex, int spritey) :
        Status(name, timeLeft, SDL_Rect{spritex, spritey, 4, 4}, 3, 0, false),
        sideeffect((SideEffect)effects) {}

    Status              *clone();

    void                updateHolder(StatusHolder*, Mob*);
private:
    SideEffect          sideeffect;
};

#endif
