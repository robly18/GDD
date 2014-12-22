#include "actor.hpp"

Actor::Actor (SDL_Surface* texture, SDL_Rect r, SDL_Rect p) :
    texture(texture), r(r), p(p) {
}

//Global helper function
bool isInRect(SDL_Rect r, int x, int y) {
    return r.x <= x && x <= r.x + r.w && r.y <= y && y <= r.y + r.h;
}
