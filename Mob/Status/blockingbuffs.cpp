#include "blockingbuffs.hpp"

Status* SideEffectBuff::clone() const {
    return new SideEffectBuff(name, timeLeft, sideeffect, sprite.x, sprite.y);
}

void SideEffectBuff::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    holder->sideeffect |= sideeffect;
}

/**/

Status* CounterDebuffBuff::clone() const {
    return new CounterDebuffBuff(name, timeLeft, debf, sprite.x, sprite.y);
}

void CounterDebuffBuff::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    holder->counterdebuff = debf;
}
