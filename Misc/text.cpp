#include "text.hpp"

const SDL_Rect* BmpFont::computeChars() const {
    SDL_Rect* c =  (SDL_Rect*) malloc(256*sizeof(SDL_Rect));
    for (int x = 0; x != 16; x++) {
        for (int y = 0; y != 16; y++) {
            c[x*16+y] = SDL_Rect{x*8, y*8, 8, 8};
        }
    }
    return c;
}

BmpFont::BmpFont(SDL_Surface* surface) :
    chars(computeChars()), font(surface) {
}

const SDL_Rect* BmpFont::operator[] (int i) const {
    return chars+i;
}

FontStr::FontStr(const BmpFont *font, int maxlen, std::string s) {
    chars = (const SDL_Rect**) calloc(maxlen + 1, sizeof(const SDL_Rect*));
    setText(font, s);
}

FontStr::~FontStr() {
    free(chars);
}

void FontStr::render(SDL_Surface* targetsurface, SDL_Surface* texture, int x, int y) {
    SDL_Rect p = {x,y, 8, 8};
    for (const SDL_Rect** r = chars; *r != NULL; r++) {
        SDL_BlitSurface(texture, *r, targetsurface, &p);
        p.x += 8;
    }
}

void FontStr::setText(const BmpFont *font, std::string txt) {
    str = txt;
    unsigned int i = 0;
    for (i = 0; i != str.length(); i++) {
        chars[i] = (*font)[str[i]];
    }
    chars[i] = NULL;
}
