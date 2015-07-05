#include "log.hpp"


Log::Log(BmpFont* font) :
    font(font) {
}

void Log::logHelp() {
    addMessage("\nHello! I'm the Log.\
 Here you can see any important messages that come up.\
\nLeft click me to expand, and press up and down to scroll. Try it now!");
}

void Log::addMessage(const char* msg) {
    char buffer [47] = {};
    for (int i = 0; i != 47; i++) {
        buffer[i] = msg[i];
        if (msg[i] == '\0') {
            addLine(msg);
            return;
        }
        if (msg[i] == '\n') {
            buffer[i] = '\0';
            addLine(buffer);
            addMessage(msg+i+1);
            return;
        }
    }

    int i = 47;
    for (int j = sizeof(buffer)-1; j != 0; j--) {
        if (buffer[j] == ' ') {
            buffer[j] = '\0';
            break;
        }
        i--;
    }
    addLine(buffer);
    addMessage(msg+i);
}

void Log::addMessage(char* buffer, const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    vsprintf(buffer, msg, args);
    addMessage(buffer);

    va_end(args);
}

void Log::addLine(const char* msg) {
    msgs.push_back(new FontStr(font, std::string(msg)));
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
bool Log::checkClick(int x, int y) {
    SDL_Rect r = fullscreen ? fullrect : minrect;
    return isInRect(r, x, y);
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
