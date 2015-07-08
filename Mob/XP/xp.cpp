#include "xp.hpp"

XpBar::XpBar(SDL_Rect r, std::string name, int type, int startmaxval, int namelen) :

    bar(new NumBar(r, BARCOLF, BARCOLE, engine.font,
                     startmaxval, &xpval, name+":", false)),
    name(name),
    screenname(new FontStr(engine.font, "0")),
    type(type),
    maxxp(startmaxval) {

    numpos.x = r.x + 2 + 8*namelen;
    numpos.y = r.y + (r.h-8)/2;
}

void XpBar::focus(bool foc) {
    f = foc;
    if (f) {
        bar->emptyc = BARCOLE + FOCUSCOL;
        bar->fullc = BARCOLF + FOCUSCOL;
    } else {
        bar->emptyc = BARCOLE;
        bar->fullc = BARCOLF;
    }
}

bool XpBar::isFocus() {
    return f;
}

int XpBar::addXp(int xp) {
    int oldlvl = lvl;

    xpval += xp * (f?5:1); //if focus, multiply xp by 5


    while (xpval >= maxxp) {
        if (lvl < 99) {
            lvl++;
            xpval-=maxxp;

            maxxp += maxxp / 20 + 5;
            bar->maxval = maxxp;



            char buffer[64];

            sprintf(buffer, "%i", lvl);
            screenname->setText(engine.font, std::string(buffer));

            engine.ui->log->addMessage(buffer,
                    "Player leveled up %s to lvl %i!", name.c_str(), lvl);
            engine.map->player->levelUp(type);
        } else {
            xpval = maxxp;
            break;
        }
    }

    return lvl - oldlvl;
}

void XpBar::render(SDL_Surface* surface) {
    bar->render(surface);
    bar->renderName(surface);

    screenname->render(surface, engine.font->font, numpos.x, numpos.y);
}

/**/

XpHolder::XpHolder() {
    std::string names [10] = {"SWRD", "STFF", "BOW",
        "ATK", "DEF",
        "STR", "STMN", "SPD", "SGHT", "ACCY"};

    for (int i = 0; i != 3; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*i, 108, 14}, names[i], i, 10, 5);
    }

    bars[3] = new XpBar(SDL_Rect{261, 4+16*3, 108/2-1, 14}, names[ATK], ATK, 10, 4);

    bars[4] = new XpBar(SDL_Rect{261+108/2+1, 4+16*3, 108/2-1, 14}, names[DEF], DEF, 10, 4);

    for (int i = 5; i != 10; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*(i-1), 108, 14}, names[i], i, 10, 5);
    }

    setFocus(f);
}

void XpHolder::setFocus(int bar) {
    bars[f]->focus(false);
    if (f == SWRD) {
        bars[STFF]->focus(false);
        bars[BOW]->focus(false);
    }
    bar = bar>BOW ? bar : SWRD;
    f = bar;
    bars[f]->focus(true);
    if (f == SWRD) {
        bars[BOW]->focus(true);
        bars[STFF]->focus(true);
    }
}

void XpHolder::levelUp(int val, int weapontype) {
    if (weapontype != -1) bars[weapontype]->addXp(val);

    for (XpBar **bar = bars+3; bar != bars+10; bar++) {
        (*bar)->addXp(val);
    }
}

void XpHolder::log(Log &l, int i) {
    char buffer[255];
    XpBar &bar = *bars[i];
    l.addMessage(buffer, "\n%s XP bar: Level %i (%i/%i)", bars[i]->name.c_str(),
                 bar.lvl, bar.xpval, bar.maxxp);
    if (bar.isFocus()) {
        l.addMessage(
"The brighter yellow-y thing means this bar is your focus.\n\
Focus makes the selected XP level up 5x as fast. Left click to change focus.\n");
    }
    switch (i) {
    case SWRD:
        l.addMessage(
"Weapon XP linked to swords.\n\
Weapon XP boosts mana regeneration when using certain types of weapons.");
        break;
    case BOW:
        l.addMessage(
"Weapon XP linked to bows.\n\
Weapon XP boosts mana regeneration when using certain types of weapons.");
        break;
    case STFF:
        l.addMessage(
"Weapon XP linked to staffs.\n\
Weapon XP boosts mana regeneration when using certain types of weapons.");
        break;
    case ATK:
        l.addMessage(
"Directly linked to how much damage you deal.\nApplies a multiplier on your attacks,\
ranging from 1 to nearly 11 at max level.");
        break;
    case DEF:
        l.addMessage(
"Related to how much health you have.\nEach level adds a certain amount of health to your bar.\
 Subject to change. In fact all of these are. This one is just the most.");
        break;
    case STR:
        l.addMessage(
"Directly linked to how effective your armor is.\nApplies a multiplier on your armor's defense,\
 ranging from 1 to nearly 11 at max level. Effectively useless if wearing none.");
        break;
    case STMN:
        l.addMessage(
"Related to how much mana you have.\nApplies a multiplier on the mana granted by your weapon.");
        break;
    case SPD:
        l.addMessage(
"Linked to your swiftness.\nSwiftness, quite counterintuitively, makes you faster the lower it is.");
        break;
    case SGHT:
        l.addMessage(
"Come on, do you really need to know what this one does?\nAllows you to see one tile further\
 every 14 levels.");
        break;
    case ACCY:
        l.addMessage(
"Related to your accuracy.\nUseless when using swords, but necessary to attack using certain\
 bows and staffs. On those, the more accuracy you have, the further you can shoot.");
        break;
    }
}


