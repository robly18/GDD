#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <SDL.h>
#include <random>
#include <time.h>
#include "..\Misc\gddlist.hpp"
#include "..\Misc\text.hpp"
#include "..\Ui\ui.hpp"

#include "..\Main\main.hpp"

#define SWIDTH (110+6+16*16)
#define SHEIGHT (10*16+6+46+23)

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
        LOG,
    };
    State state = State::INIT;
    bool camera = false;

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

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int mx, my;

    SDL_Texture* loadTexture(char*, short r = 0xFF, short g = 0x00, short b = 0xFF);
};

extern Engine engine;

#endif
