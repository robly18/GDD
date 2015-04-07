#include "log.hpp"


Log::Log(BmpFont* font) :
    font(font) {
}

void Log::addMessage(const char* msg) {
    std::cout<<msg<<"\n";
    std::cout<<padding[0]<<"\n";

    if (strlen(msg) >= 47) {
        char buffer [47] = {};
        for (int i = 0; i != 47; i++) {
            buffer[i] = msg[i];
        }
        int i = 47;
        for (char* c = buffer+46; c != buffer; c--) {
            if (*c == ' ') {
                *c = '\0';
                std::cout<<"\nBuffer is: ";
                std::cout<<buffer<<"\n";
                break;
            }
            i--;
        }
        addLine(buffer);
        addMessage(msg+i);
    } else {
        addLine(msg);
    }
}

void Log::addMessage(char* buffer, const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    vsprintf(buffer, msg, args);
    addMessage(buffer);

    va_end(args);
}

void Log::addLine(const char* msg) {
    msgs.push_back(new FontStr(font, 47, std::string(msg)));
}

void Log::render(SDL_Surface* surface) {
    unsigned int msgnum = msgs.size();
    if (!fullscreen) {
        SDL_FillRect(surface, &minrect, 0);
        for (unsigned int i = msgnum < 5 ? 0 : msgnum-5; i < msgnum; i++) {
            msgs[i]->render(surface, font->font, 3, 188+9*(i-msgnum+5));
        }
    } else {
        SDL_FillRect(surface, &fullrect, 0);
        if (msgnum)
        for (unsigned int i = readline < 24 ? 0 : readline - 24; i <= readline; i++) {
            msgs[i]->render(surface, font->font, 3, 8+9*(i-readline+24));
        }
    }
}

/**/

bool Log::checkUnclick(int hx, int hy, int mx, int my) {
    SDL_Rect r = fullscreen ? fullrect : minrect;
    if (isInRect(r, hx, hy) &&
        isInRect(r, mx, my)) {
        fullscreen = !fullscreen;
        readline = msgs.size() - 1;
    }
    return fullscreen;
}

void Log::moveReadLine(int dir) {
    if ((int)readline < -dir) {
        readline = 0;
    } else {
        readline += dir;
        if (readline >= msgs.size()) {
            readline = msgs.size() - 1;
        }
    }
}
