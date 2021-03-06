#include "ai.hpp"

Ai::Ai(int s, int offset) :
    swiftness(s), timeoffset(offset) {}

PlayerAi::PlayerAi() :
    Ai(150, 0) {}

void PlayerAi::update(Mob* mob) {
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

void PlayerAi::tryMoving(Mob* mob, int dx, int dy) {
    if (engine.map->canMoveTo(mob->x + dx, mob->y + dy)) {
        mob->move(dx, dy);
    }
}



BasicAi::BasicAi(int s) :
    Ai(s, std::rand() % s) {}

void BasicAi::lookForPlayer(Mob* mob) {
    if (engine.map->fovcomputer->isInSight(mob->x, mob->y,
                                           engine.map->player->x,
                                           engine.map->player->y,
                                           mob->sght)) {
        seekingPos = engine.map->player->getPos();
        path.clear();
        engine.map->pathfinder->computePath(mob->getPos(), seekingPos, path);
        sawPlayer = true;
    }
}

void BasicAi::update(Mob* mob) {

    lookForPlayer(mob);

    Pos mobpos = mob->getPos();


    if (path.size() && hasntMovedIn < 3) { //If we have places to go, go there
        if (engine.map->canMoveTo(path.front())) {
            mob->setPos(path.front());
            path.pop_front();
            hasntMovedIn = 0;
        } else if (posEq(path.front(), engine.map->player->getPos())) {
            static_cast<const Attack*>(mob->attack)->target(mob, path.front());
            if (mob->destructible->isDead()) return;
        } else hasntMovedIn++;
    } else { //If not, wander randomly
        path.clear();
        if (posDiff(mobpos, mob->spawn) > MAXAWAYFROMSPAWN) {
            engine.map->pathfinder->computePath(mob->getPos(), mob->spawn, path);
            mob->setPos(path.front());
            path.pop_front();
        } else {

            Pos movingTo = mob->getPos();

            int randomNum = std::rand();
            movingTo = addPos(movingTo, (randomNum&1) ? Pos {0, (randomNum & 2) - 1} :
                                                        Pos {(randomNum & 2) - 1, 0});

            if (posDiff(movingTo, mob->spawn) <= MAXAWAYFROMSPAWN &&
                engine.map->canMoveTo(movingTo)) {
                mob->setPos(movingTo);
            }
        }

        sawPlayer = false;
    }

    lookForPlayer(mob);

}
