#include "mob.hpp"

Mob::Mob(int x, int y, SDL_Rect r, SDL_Surface* t, std::string n) :
    x(x), y(y), spawn(Pos{x, y}) {
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

int Mob::getSwiftness() const {
    if (!ai) return 0;
    if (destructible && destructible->armor) {
        return ai->swiftness + destructible->armor->swiftness;
    } else {
        return ai->swiftness;
    }
}

bool Mob::swapWith(Mob &m) {
    Pos thispos = getPos();
    setPos(m.getPos());
    m.setPos(thispos);
    return true;
}

Player::Player (int x, int y, SDL_Rect r, SDL_Surface *t, std::string n) :
        Mob(x, y, r, t, n) {
    defaultattack = new TargetedAttack(1, 0, "LAME", NOICON, 0, 1);
    xpholder = new XpHolder;
}


int Player::getMaxMp() const {
    return stmn * (weapon ? weapon->maxmana : 0) / 10;
}

void Player::levelUp(int type) {
    switch (type) {
    case XpHolder::SWRD:
    case XpHolder::BOW:
    case XpHolder::STFF:
        weplvls[type]++;
        break;
    case XpHolder::ATK:
        atk++;
        break;
    case XpHolder::DEF:
        destructible->maxHp += 5;
        destructible->heal(5);
        engine.ui->buttons->hpbar->maxval = destructible->maxHp;
        break;
    case XpHolder::STR:
        destructible->def++;
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


/**log infos**/

void Mob::logInfo(Log &log) const {
    char buffer [255];
    Mobdef def = engine.database->getMobDef(name);
    log.addMessage(buffer, "%s Description: %s", name.c_str(), def.desc.c_str());
    log.addMessage(buffer, "Inspecting mob: %s", name.c_str());
    log.addMessage(buffer, "HP: %i/%i; ATK: %i; DEF: %i; XP: %i",
                        destructible->hp, def.hp,
                        def.atk, def.def, def.xp);
    log.addMessage(buffer, "SGHT: %i", def.sght);
    log.addMessage(buffer, "Swiftness: %i (%i)",
                                    getSwiftness(),
                                    engine.map->player->getSwiftness() - getSwiftness());
    log.addMessage(buffer, "Time until next move: %i",
                            getSwiftness() -
                            (engine.time - ai->timeoffset - 1) % getSwiftness() - 1);
}

void Player::logInfo(Log &log) const {
    char buffer [255];
    log.addMessage("This is you! Here's a few stats:");
    if (destructible->armor)
        log.addMessage(buffer, "DEF: %i", destructible->armor->defense);
    else
        log.addMessage("DEF: N/A");
    log.addMessage(buffer, "SGHT: %i", sght);
    log.addMessage(buffer, "Swiftness: %i", getSwiftness());

}
