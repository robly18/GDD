#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <SDL.h>
#include <random>
#include <time.h>
#include "..\Misc\gddlist.hpp"
#include "..\Misc\text.hpp"
#include "..\Ui\ui.hpp"

#include "state.hpp"

#include "..\Main\main.hpp"


#define SWIDTH (110+6+16*16)
#define SHEIGHT (10*16+6+46+23)

class Map;
class Mob;
class Attack;
class NumBar;
class Ui;

class Database;
class Interpreter;

struct MouseState {
    Pos p;

    bool pressed;
    Pos h; //pos of where was pressed
};

class Engine {
public:
    bool camera = false;

    int init();
    void restartGame();

    void render();
    int  checkEvents();

    void doTick(int flags);
    enum {MOVEDFLAG, ATTACKEDFLAG, USEDFLAG};

    SDL_Surface* texture;

    BmpFont* font;

    Ui* ui;

    unsigned int time = 0;

    Map* map;

    SDL_KeyboardEvent lastkey;


    EngineState::State          *enginestate;

    EngineState::State          *running,
                                *inv,
                                *viewmap,
                                *viewlog;
    EngineState::Viewprompt     *viewprompt;

    Database                    *database;
    Interpreter                 *interpreter;

private:

    MouseState                  mouse;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* loadTexture(char*, short r = 0xFF, short g = 0x00, short b = 0xFF);
};

extern Engine engine;

#endif
