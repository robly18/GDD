#include "status.hpp"

void StatusHolder::render(SDL_Surface* surface, SDL_Rect r) {
    for (std::list<Status*>::iterator s = statuses.begin();
                                        s != statuses.end(); s++) {
        SDL_BlitSurface(engine.texture, &(*s)->sprite, surface, &r);
    }
}

void StatusHolder::update(Mob* mob) {
    poisondmg = 0;
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
}

/**Statuses**/

void Poison::updateHolder(StatusHolder* holder, Mob* user) {
    timeLeft--; //todo experiment with this part
    int dmg = computePoisonDmg(user);
    if (holder->poisondmg < dmg) holder->poisondmg = dmg;
}

int FixedHpPoison::computePoisonDmg(Mob* mob) {
    return str;
}

Status* FixedHpPoison::clone() {
    return new FixedHpPoison(str, name, timeLeft);
}
