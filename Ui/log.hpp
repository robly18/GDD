#ifndef LOG_H
#define LOG_H

#include <SDL.h>
#include "..\Misc\text.hpp"

class Log {
public:
    Log(BmpFont*);

    FontStr* messages [5] = {}; //index = most recent; index+1 = longest ago
    int index = 4;


    void addMessage(const char*);

    void render(SDL_Surface*);
private:
    void addLine(const char*);
    BmpFont* font;
};


#endif
