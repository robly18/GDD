#include "status.hpp"

void StatusHolder::render(SDL_Surface* surface, SDL_Rect r) {
    for (std::list<Status*>::iterator s = statuses.begin();
                                        s != statuses.end(); s++) {
        SDL_Rect renderrect = r;
        renderrect.x += (*s)->xpos;
        renderrect.y += (*s)->ypos;
        SDL_BlitSurface(engine.texture, &(*s)->sprite, surface, &renderrect);
    }
}

void StatusHolder::update(Mob* mob) {
    poisondmg = 0;
    healval = 0;
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
            sprintf(buffer, "%s took %i poison damage and now has %i health",
                    mob->name.c_str(), poisondmg, mob->destructible->hp);
            engine.ui->log->addMessage(buffer);
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
            sprintf(buffer, "%s regenerated %i health and now has %i health",
                    mob->name.c_str(), healval, mob->destructible->hp);
            engine.ui->log->addMessage(buffer);
        }
    }
}