#ifndef ACTOR_H
#define ACTOR_H

#include <SDL.h>

class Actor {
public:
    SDL_Surface* texture;
    SDL_Rect r;
    SDL_Rect p;

    Actor(SDL_Surface*, SDL_Rect r, SDL_Rect p);
};

#endif
