#ifndef XP_H
#define XP_H

#include "..\..\Main\main.hpp"
#include <memory>
#include <string>

#define BARCOLF 0x22DDDD
#define BARCOLE 0x22AAAA

#define FOCUSCOL (0x222222)

class NumBar;

class XpBar {
public:
    XpBar(SDL_Rect pos, std::string name, int max, int namelen);

    NumBar                      *bar;
    void                        focus(bool);
    bool                        isFocus();

    void                        addXp(int);

    std::string                 name;
    std::unique_ptr<FontStr>    screenname;

    int                         xpval = 0;
    int                         maxxp;

    int                         lvl = 0;

    void                        render(SDL_Surface*);

private:
    bool                        f;
    Pos                         numpos;
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

    void            setFocus(int);

    void            levelUp(int val, int weapontype);

private:
    int             f = ATK; //If it is SWRD, it applies to all the first 3
};

#endif
