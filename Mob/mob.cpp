#include "mob.hpp"

Mob::Mob(int x, int y, SDL_Rect r, SDL_Surface* t, std::string n) :
    x(x), y(y) {
    a = new Actor(t, r, POSRECT(x, y));
    name = n;
}

void Mob::setPos(int x, int y) {
    Mob::x = x;
    Mob::y = y;
    a->p = POSRECT(x, y);
}

void Mob::setPos(Pos p) {
    Mob::x = p.x;
    Mob::y = p.y;
    a->p = POSRECT(p.x, p.y);
}

Pos Mob::getPos() {
    return Pos{x, y};
}

void Mob::move(int mx, int my) {
    setPos(x+mx, y+my);
}

int Mob::getSwiftness() {
    if (!ai) return 0;
    if (destructible && destructible->armor) {
        return ai->swiftness + destructible->armor->swiftness;
    } else {
        return ai->swiftness;
    }
}

Player::Player (int x, int y, SDL_Rect r, SDL_Surface *t, std::string n) :
        Mob(x, y, r, t, n) {
    defaultattack = new TargetedAttack(1, 0, 0, 1);
    xpholder = new XpHolder;
}


int Player::getMaxMp() const {
    return stmn * (weapon ? weapon->maxmana : 0);
}

void Player::levelUp(int type) {
    switch (type) {
    case XpHolder::STR:
        str = str*5/4;
        break;
    case XpHolder::STMN:
        stmn = stmn*5/4;
        engine.ui->buttons->mpbar->maxval = getMaxMp();
        break;
    }
}
