#include <iostream>
#include <SDL.h>

#include "main.hpp"

Engine engine;

#define NSHOWFPS

#include <time.h>


int main(int argn, char* argv []) {
    #ifdef SHOWFPS
    time_t t = time(NULL), prevt = t;
    int frames = 0;
    #endif
    if (!engine.init())
    while (engine.checkEvents() == 0) {
        #ifdef SHOWFPS
        time(&t);
        frames++;
        if (t != prevt) {
            std::cout<<frames<<"\n";
            frames = 0;
            prevt = t;
        }
        #endif
        DEBUGMSG("Rendering\n");
        engine.render();
        DEBUGMSG("Checking\n");
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
fix crash on take inv from full (done)
on change wep change atk (done)
hotkeys

reset upon death :T

EDIT: all done
*/

/*
This comment is a thank you note to Miguel
Thx bro for helping me test
I mean it
*/
