#ifndef XP_H
#define XP_H

#include "..\..\Main\main.hpp"

#define BARCOLF 0x00FFFF
#define BARCOLE 0x00AAAA

class NumBar;

class XpBar {
public:
    XpBar(SDL_Rect, std::string, int);

    NumBar          *bar;

    std::string     name;

    int             xpval = 0;
    int             maxxp;

    int             lvl = 0;
};

/*Xp bars for:

3 diff wep types
atk n def
str
stmn
spd
sght
accy

*/

class XpHolder {
public:
    XpHolder();

    enum {
        SWRD = 0,
        STFF,
        BOW,
        ATK,
        DEF,
        STR,
        STMN,
        SPD,
        SGHT,
        ACCY
    };

    XpBar           *bars[10];
};

#endif
