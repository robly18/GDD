#include "xp.hpp"
//261 4 - 108 14+2

XpBar::XpBar(SDL_Rect r, std::string name, int startmaxval) :
    name(name), maxxp(startmaxval) {
    bar = new NumBar(r, BARCOLF, BARCOLE, engine.font,
                     maxxp, &xpval, name, false);
}

XpHolder::XpHolder() {
    std::string names [10] = {"SWRD", "STFF", "BOW",
        "ATK", "DEF",
        "STR", "STMN", "SPD", "SGHT", "ACCY"};

    for (int i = 0; i != 3; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*i, 108, 14}, names[i], 10);
    }
    bars[3] = new XpBar(SDL_Rect{261, 4+16*3, 108/2-1, 14}, names[3], 10);
    bars[4] = new XpBar(SDL_Rect{261+108/2+1, 4+16*3, 108/2-1, 14}, names[4], 10);

    for (int i = 5; i != 10; i++) {
        bars[i] = new XpBar(SDL_Rect{261, 4+16*(i-1), 108, 14}, names[i], 10);
    }
}
