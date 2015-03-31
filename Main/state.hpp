#ifndef STATE_H
#define STATE_H

#include <SDL.h>
#include "..\Misc\posstruct.hpp"

class Engine;
struct MouseState;
class Prompt;

//Add this to every State class: prototypes for the state functions
#define STATEFUNCS() \
    virtual void        render(Engine&, SDL_Renderer*, const MouseState); \
    virtual void        actOnEvent(Engine&, SDL_Event&, const MouseState); \

namespace EngineState {

class State {
public:
    virtual void        render(Engine&, SDL_Renderer*, const MouseState) = 0;
    virtual void        actOnEvent(Engine&, SDL_Event&, const MouseState) = 0;


};

class InGame : public State {
public:
    STATEFUNCS();

protected:
    void                doTick(Engine&, int flag);

    enum {MOVED, ATTACKED, USED};

};

class Running : public InGame {
public:
    STATEFUNCS();
protected:
    SDL_Rect maprect = {2*2, 2*2, 16*16*2, 10*16*2};
};

class Viewmap : public InGame {
public:
    STATEFUNCS();
private:
    SDL_Rect maprect = {2*2, 2*2, 16*16*2, 10*16*2};
};

class Inv : public InGame {
public:
    STATEFUNCS();
};

class Viewprompt : public Running {
public:
    STATEFUNCS();

    Prompt      *prompt;
};

class Viewlog : public State {
public:
    STATEFUNCS();
};

}
#include "main.hpp"


#endif
