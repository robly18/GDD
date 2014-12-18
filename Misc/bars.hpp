#ifndef BARS_H
#define BARS_H

#include <SDL.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "text.hpp"

#define MAXSTRLEN 16

class NumBar {
public:
    NumBar(SDL_Rect, Uint32, Uint32, BmpFont*,
           int maxval, int* valptr, std::string name, bool showval = true);
    SDL_Rect barmax;

    Uint32 emptyc;
    Uint32 fullc;

    std::string name;
    char buffer [MAXSTRLEN] = {};
    bool showval;

    int maxval;
    int* valptr;

    BmpFont* font;
    FontStr* fontname;

    void render(SDL_Surface*);
    void renderName(SDL_Surface*);
    void getNumInfo(char*);
};

#endif
