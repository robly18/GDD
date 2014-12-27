#include "blockingbuffs.hpp"

Status* SideEffectBuff::clone() {
    return new SideEffectBuff(name, timeLeft, sideeffect, sprite.x, sprite.y);
}

void SideEffectBuff::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    *(int*)&holder->sideeffect |= (int)sideeffect;
}

/**/

Status* CounterDebuffBuff::clone() {
    return new CounterDebuffBuff(name, timeLeft, debf, sprite.x, sprite.y);
}

void CounterDebuffBuff::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    holder->counterdebuff = debf;
}
