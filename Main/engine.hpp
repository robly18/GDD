#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <SDL.h>
#include "..\Misc\gddlist.hpp"
#include "..\Misc\text.hpp"
#include "..\Ui\ui.hpp"

#include "..\Main\main.hpp"

#define SWIDTH (110+6+16*16)
#define SHEIGHT (10*16+6+46+23)

#define DEBUGMSG(s) std::cout<<s

class Map;
class Mob;
class Attack;
class NumBar;
class Ui;



class Engine {
public:
    enum class State {
        QUIT,
        INIT,
        RUNNING,
        MOVED,
        ATTACKED,
        USED,
        DEAD,
        INV,
        MAP,
    };
    State state = State::INIT;

    int init();

    void render();
    void checkEvents();
    void doTick();

    SDL_Surface* texture;

    BmpFont* font;

    Ui* ui;

    unsigned int time = 0;

    Map* map;

    SDL_KeyboardEvent lastkey;

    Attack* testattack;

    enum Mouse {
        LMB,
        RMB,
        MMB
    };
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int mx, my;

    SDL_Texture* loadTexture(char*, short r = 0xFF, short g = 0x00, short b = 0xFF);
};

extern Engine engine;

#endif
