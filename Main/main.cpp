#include <iostream>
#include <SDL.h>

#include "main.hpp"

Engine engine;


int main(int argn, char* argv []) {
    if (!engine.init())
    while (engine.state != Engine::State::QUIT) {
        DEBUGMSG("Ticking\n");
        engine.doTick();
        DEBUGMSG("Rendering\n");
        engine.render();
        DEBUGMSG("Checking\n");
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

//woah this is like so old holy crap
//but yeah past me is right nobody cares about the todo list
//its all done now anyway
//well not all but
//you get the point

/*
todo: i = open inv
fix crash on take inv from full
on change wep change atk
hotkeys
*/

/*
This comment is a thank you note to Miguel
Thx bro for helping me test
I mean it
*/
