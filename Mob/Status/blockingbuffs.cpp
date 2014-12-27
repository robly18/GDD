#include "blockingbuffs.hpp"

Status* SideEffectBuff::clone() {
    return new SideEffectBuff(name, timeLeft, sideeffect, sprite.x, sprite.y);
}

void SideEffectBuff::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--;
    *(int*)&holder->sideeffect |= (int)sideeffect;
}
