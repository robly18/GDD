#include "ai.hpp"

void PlayerAi::update(Mob* mob) {
    if (engine.state == engine.State::MOVED) {
        switch (engine.lastkey.keysym.sym) {
        case SDLK_UP: tryMoving(mob, 0,-1); break;
        case SDLK_DOWN: tryMoving(mob, 0,1); break;
        case SDLK_LEFT: tryMoving(mob, -1,0); break;
        case SDLK_RIGHT: tryMoving(mob, 1,0); break;
        default: return;
        }
        if (engine.map->player->weapon) {
            engine.map->player->weapon->cancelAttack(engine.map->player);
        } else {
            engine.map->player->attack = NULL;
        }
    }
}

void PlayerAi::tryMoving(Mob* mob, int dx, int dy) {
    if (engine.map->canMoveTo(mob->x + dx, mob->y + dy)) {
        mob->move(dx, dy);
    }
}

void TestAi::update(Mob* mob) {
    if (engine.map->canMoveTo(mob->x, mob->y+dir)) {
        mob->move(0, dir);
    } else {
        if (mob->attack) mob->attack->target(mob, mob->x, mob->y+dir);
        dir = -dir;
    }
}
