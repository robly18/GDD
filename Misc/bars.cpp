#include "bars.hpp"

NumBar::NumBar(SDL_Rect barmax, Uint32 fullc, Uint32 emptyc, BmpFont* font,
               int maxval, int* valptr,
               std::string name, bool showval) :
    barmax(barmax), emptyc(emptyc), fullc(fullc), name(name),
    showval(showval), maxval(maxval), valptr(valptr),
    font(font) {
    fontname = new FontStr(font, name);
}

void NumBar::render(SDL_Surface* surface) {
    SDL_FillRect(surface, &barmax, emptyc);
    if (valptr) {
        SDL_Rect r = {barmax.x, barmax.y, barmax.w*(*valptr)/maxval, barmax.h};
        SDL_FillRect(surface, &r, fullc);
    }
}

void NumBar::renderName(SDL_Surface* surface) {
    getNumInfo(buffer);
    fontname->setText(font, std::string(buffer));
    fontname->render(surface, font->font, barmax.x+2, barmax.y+((barmax.h-8)/2));
}

void NumBar::getNumInfo(char* str) {
    sprintf(str, showval ? "%s:%i/%i" : "%s", name.c_str(),
            valptr ? (*valptr) : 0, maxval);
}
