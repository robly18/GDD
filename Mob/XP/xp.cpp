#include "xp.hpp"

XpBar::XpBar(SDL_Rect r, std::string name, int startmaxval) :
    name(name), maxxp(startmaxval) {
    bar = new NumBar(r, BARCOLF, BARCOLE, engine.font,
                     maxxp, &xpval, name, false);
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

void XpBar::addXp(int xp) {
    xpval += xp * (f?2:1); //if focus, multiply xp by 2

    while (xpval >= maxxp) {
        lvl++;
        xpval-=maxxp;

        char buffer[64];
        sprintf(buffer, "Player leveled up %s to lvl %i!", name.c_str(), lvl);
        engine.ui->log->addMessage(buffer);

        maxxp*=2;
        bar->maxval = maxxp;
    }

}

/**/

XpHolder::XpHolder() {
    std::string names [10] = {"SWRD", "STFF", "BOW",
        "ATK", "DEF",
        "STR", "STMN", "SPD", "SGHT", "ACCY"};

    for (int i = 0; i != 3; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*i, 108, 14}, names[i], 10);
    }

    bars[3] = new XpBar(SDL_Rect{261, 4+16*3, 108/2-1, 14}, names[3], 10);
    bars[3]->focus(true);

    bars[4] = new XpBar(SDL_Rect{261+108/2+1, 4+16*3, 108/2-1, 14}, names[4], 10);

    for (int i = 5; i != 10; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*(i-1), 108, 14}, names[i], 10);
    }
}

void XpHolder::setFocus(int bar) {
    bars[f]->focus(false);
    if (f == SWRD) {
        bars[BOW]->focus(false);
        bars[STFF]->focus(false);
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
    bars[weapontype]->addXp(val);
    for (XpBar **bar = bars+3; bar != bars+10; bar++) {
        (*bar)->addXp(val);
    }
}


