#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>

#include <iostream>

class BmpFont {
private:
    const SDL_Rect* computeChars() const;
public:
    const SDL_Rect* chars; //size: 256

    BmpFont(SDL_Surface*);

    SDL_Surface* font;

    const SDL_Rect* operator [] (int) const;
};

class FontStr {
public:
    const SDL_Rect** chars;
    std::string str;

    FontStr(const BmpFont*, int maxlen, std::string);
    ~FontStr();

    void render(SDL_Surface*, SDL_Surface*, int x, int y);
    void setText(const BmpFont *font, std::string);
};


#endif
