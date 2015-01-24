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
        engine.map->resetCamera();
    }
}

void TestAi::update(Mob* mob) {
    engine.map->pathfinder->computePath(mob->getPos(), engine.map->player->getPos(), path);

    if (path.size())
    if (engine.map->canMoveTo(path.front())) {
        mob->setPos(path.front());
    }
}
