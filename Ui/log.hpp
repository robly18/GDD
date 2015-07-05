#ifndef LOG_H
#define LOG_H

#include <SDL.h>
#include <vector>
#include <stdarg.h>

#include "..\Misc\text.hpp"

extern bool isInRect(SDL_Rect, int, int);

class Log {
public:
    Log(BmpFont*);

    std::vector<FontStr*>   msgs; //0 = oldest; msgs.size() - 1 = newest

    void                    addMessage(const char*);
    void                    addMessage(char*, const char*, ...);
                            //For printf like syntax
                            //Additional char* required to be passed as a buffer

    void                    render(SDL_Surface*);

    bool                    checkUnclick(int, int, int, int);
    bool                    checkClick(int, int);

    void                    moveReadLine(int);

    void                    logHelp();
private:
    void                    addLine(const char*);
    BmpFont                 *font;

    bool                    fullscreen = false;
    unsigned int            readline = 0; //The line where the log starts when fullscreen

    const SDL_Rect          fullrect = {2,2,368,231},
                            minrect = {2,187,368,46};
};


#endif
