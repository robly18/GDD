#include "log.hpp"


Log::Log(BmpFont* font) :
    font(font) {
}

void Log::addMessage(const char* msg) {
    if (strlen(msg) >= 47) {
        char buffer [47] = {};
        for (int i = 0; i != 47; i++) {
            buffer[i] = msg[i];
        }
        int i = 47;
        for (char* c = buffer+46; c != buffer; c--) {
            if (*c == ' ') {
                *c = '\0';
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

void Log::addLine(const char* msg) {
    messages.push_back(new FontStr(font, 47, std::string(msg)));
}

void Log::render(SDL_Surface* surface) {
    unsigned int msgnum = messages.size();
    if (!fullscreen) {
        SDL_FillRect(surface, &minrect, 0);
        for (unsigned int i = msgnum < 5 ? 0 : msgnum-5; i < msgnum; i++) {
            messages[i]->render(surface, font->font, 3, 188+9*(i-msgnum+5));
        }
    } else {
        SDL_FillRect(surface, &fullrect, 0);
        if (msgnum)
        for (unsigned int i = readline < 24 ? 0 : readline - 24; i <= readline; i++) {
            messages[i]->render(surface, font->font, 3, 8+9*(i-readline+24));
        }
    }
}

/**/

bool Log::checkUnclick(int hx, int hy, int mx, int my) {
    SDL_Rect r = fullscreen ? fullrect : minrect;
    if (isInRect(r, hx, hy) &&
        isInRect(r, mx, my)) {
        fullscreen = !fullscreen;
        readline = messages.size() - 1;
    }
    return fullscreen;
}

void Log::moveReadLine(int dir) {
    if ((int)readline < -dir) {
        readline = 0;
    } else {
        readline += dir;
        if (readline >= messages.size()) {
            readline = messages.size() - 1;
        }
    }
}
