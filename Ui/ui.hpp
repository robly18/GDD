#ifndef UI_H
#define UI_H

#include "..\Main\engine.hpp"
#include "..\Misc\actor.hpp"
#include "..\Misc\bars.hpp"
#include "log.hpp"
#include "uistats.hpp"

class NumBar;
class Log;
class Actor;

class UiButtons;
class UiDashboard;
class UiInv;

class Ui {
public:
    Ui();

    SDL_Surface* barsurface;

    Log* log;

    enum Buttons {
        BLOCK,
        REGEN,
        QUIRKDEF,
        TARGETATK,
        AOEATK,
        QUIRKATK,
        INV,
        QUIT,
        MAP
    };


    UiButtons* buttons;
    UiDashboard* dashboard;
    UiInv* inv;

    int hx = 0, hy = 0; //Mouse clicked position
    bool h = false; //Is mouse clicked?

    void render(SDL_Renderer*);

    void checkClick(bool, int button, int x, int y);
};

#endif