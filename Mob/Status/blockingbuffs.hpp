#ifndef BLOCKINGBUFFS_H
#define BLOCKINGBUFFS_H

#include "status.hpp"
#include <memory>

class SideEffectBuff : public Status {
public:
    SideEffectBuff(std::string name, int timeLeft, int effects, Pos spritepos) :
        Status(name, timeLeft, SDL_Rect{spritepos.x, spritepos.y, 4, 4}, 3, 0, false),
        sideeffect((SideEffect)effects) {}

    Status              *clone() const;

    void                updateHolder(StatusHolder*, Mob*);
private:
    SideEffect          sideeffect;
};

class CounterDebuffBuff : public Status {
public:
    CounterDebuffBuff(std::string name, int timeLeft,
                      std::shared_ptr<Status> debf, int spritex, int spritey) :
                          Status(name, timeLeft, SDL_Rect{spritex, spritey, 4, 4},
                                 3, 0, false), debf(debf) {}

    Status                      *clone() const;

    void                        updateHolder(StatusHolder*, Mob*);

private:
    std::shared_ptr<Status>    debf;
};

#endif
