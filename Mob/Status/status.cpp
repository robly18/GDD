#include "status.hpp"

void StatusHolder::render(SDL_Surface* surface, SDL_Rect r) {
    for (std::list<Status*>::iterator s = statuses.begin();
                                        s != statuses.end(); s++) {
        SDL_Rect renderrect = r;
        renderrect.x += (*s)->xpos * 4;
        renderrect.y += (*s)->ypos * 4;
        SDL_BlitSurface(engine.texture, &(*s)->sprite, surface, &renderrect);
    }
}

void StatusHolder::resetStats() {
    poisondmg = 0;
    healval = 0;
    sideeffect = SideEffect::NONE;
    counterdebuff.reset();
}

void StatusHolder::update(Mob* mob) {
    resetStats();

    for (std::list<Status*>::iterator s = statuses.begin();
                                        s != statuses.end(); s++) {
        (*s)->updateHolder(this, mob);
    }
    statuses.remove_if([](Status* s){return !s->timeLeft;});

    if (poisondmg) {
        if (mob->destructible->hp != 1) {
            if (mob->destructible->hp > poisondmg) {
                mob->destructible->hp -= poisondmg;
            } else {
                poisondmg = mob->destructible->hp - 1;
                mob->destructible->hp = 1;
            }
            char buffer [255];
            engine.ui->log->addMessage(buffer,
                    "%s took %i poison damage and now has %i health",
                    mob->name.c_str(), poisondmg, mob->destructible->hp);
        }
    }
    if (healval) {
        if (mob->destructible->hp != mob->destructible->maxHp) {
            if (mob->destructible->hp + healval < mob->destructible->maxHp) {
                mob->destructible->hp += healval;
            } else {
                healval = mob->destructible->maxHp - mob->destructible->hp;
                mob->destructible->hp = mob->destructible->maxHp;
            }
            char buffer [255];
            engine.ui->log->addMessage(buffer,
                    "%s regenerated %i health and now has %i health",
                    mob->name.c_str(), healval, mob->destructible->hp);
        }
    }

    gotAttacked = false;
}

bool StatusHolder::pushStatus(Status* s) {
    std::list<Status*>::iterator stacking;

    if (s->stackable || !statusAt(s->xpos, s->ypos, &stacking)) {
        statuses.push_back(s);
    } else {
        delete *stacking;
        *stacking = s;
    }
    return true;
}

bool StatusHolder::statusAt(int x, int y, std::list<Status*>::iterator* changing) {
    for (std::list<Status*>::iterator s = statuses.begin();
                                        s != statuses.end(); s++) {
        if ((*s)->xpos == x && (*s)->ypos == y) {
            *changing = s;
            return true;
        }
    }
    return false;
}

bool StatusHolder::hasEffect(SideEffect e) {
    return (int)sideeffect & (int)e;
}
