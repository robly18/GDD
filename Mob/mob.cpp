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
    case XpHolder::ATK:
        atk++;
        break;
    case XpHolder::DEF:
        destructible->maxHp += 5;
        destructible->heal(5);
        engine.ui->buttons->hpbar->maxval = destructible->maxHp;
        break;
    case XpHolder::STR:
        str = str + 1;
        break;
    case XpHolder::STMN:
        stmn = stmn + 1;
        engine.ui->buttons->mpbar->maxval = getMaxMp();
        break;
    case XpHolder::SPD:
        ai->swiftness--;
        if (xpholder->bars[type]->lvl == 99) ai->swiftness--;
        //If you reach the top level, bonus buff to round it to 50 instead of 51
        break;
    case XpHolder::SGHT:
        if (xpholder->bars[type]->lvl % 14 == 0) {
            sght++; //Only up sight once every 14 levels
            engine.map->updateFovData();
        }
        break;
    case XpHolder::ACCY:
        accy++;
        break;
    }
}
