#ifndef LOG_H
#define LOG_H

#include <SDL.h>
#include <vector>
#include "..\Misc\text.hpp"

extern bool isInRect(SDL_Rect, int, int);

class Log {
public:
    Log(BmpFont*);

    std::vector<FontStr*>   messages; //0 = oldest; messages.size() - 1 = newest

    void                    addMessage(const char*);

    void                    render(SDL_Surface*);

    bool                    checkUnclick(int, int, int, int);
    void                    moveReadLine(int);
private:
    void                    addLine(const char*);
    BmpFont                 *font;

    bool                    fullscreen = false;
    unsigned int            readline = 0; //The line where the log starts when fullscreen

    const SDL_Rect          fullrect = {2,2,368,231},
                            minrect = {2,187,368,46};
};


#endif
