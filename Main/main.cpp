#include <iostream>
#include <SDL.h>

#include "main.hpp"

Engine engine;


int main(int argn, char* argv []) {
    if (!engine.init())
    while (engine.state != Engine::State::QUIT) {
        engine.doTick();
        engine.render();
        engine.checkEvents();
    }
    return 0;
}

/*
TODO:
Maybe enemies,
HP
Add map scrolling?
*/

//Who cares about the todo list you chump

