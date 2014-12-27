#ifndef UISTATS_H
#define UISTATS_H

#include "..\Main\main.hpp"


class UiPart {
public:
    UiPart(SDL_Surface*);

    SDL_Surface     *surface;

    virtual SDL_Surface *render(bool clicked, int hx, int hy) = 0;

    virtual void    checkClick(int mb, int hx, int hy) = 0;
    virtual void    checkUnclick(int hx, int hy, int x, int y) = 0;

};

class UiButtons : public UiPart {
public:
    UiButtons(SDL_Surface*);

    Actor           *invbutton;

    SDL_Surface     *render(bool clicked, int hx, int hy);
    void            checkClick(int mb, int hx, int hy);
    void            checkUnclick(int hx, int hy, int x, int y);

    NumBar          *hpbar;
    NumBar          *mpbar;

    void            updatePlayerData();
};

class UiDashboard : public UiPart {
public:
    UiDashboard(SDL_Surface*);

    SDL_Surface     *render(bool clicked, int hx, int hy);
    void            checkClick(int mb, int hx, int hy);
    void            checkUnclick(int hx, int hy, int x, int y);

    Actor           *atkbuttons [6];

    enum class AtkButtons {
        BLOCK,
        HEAL,
        QUIRKDEF,
        TARGETED,
        AOE,
        QUIRKATK
    };
};

typedef struct {
    Actor           *a;
    FontStr         *str;
    Uint32          color;
} InvActor;

#define NOITEMCOL 0xDDAA22
#define FINVPGSIZE 8

class FloorInventory;
class Item;

class UiInv : public UiPart {
public:
    UiInv(SDL_Surface*);

    InvActor        *plrinv [30];
    Item            *selecteditem = NULL;

    InvActor        *floorinv [FINVPGSIZE];

    Actor           *nextpage, *prevpage;

    Actor           *slots[4];



    FloorInventory  *finv;
    unsigned int    page = 0;

    SDL_Surface     *render(bool clicked, int hx, int hy);
    void            checkClick(int mb, int hx, int hy);
    void            checkUnclick(int hx, int hy, int x, int y);

    void            updateInventory();

    bool checkPlrInvClick(int hx, int hy, int x, int y);
    bool checkFloorInvClick(int hx, int hy, int x, int y);
    bool checkOtherButtonsClick(int hx, int hy, int x, int y);
};


#endif
