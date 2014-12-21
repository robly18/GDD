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
    index = (index+1)%5;
    if (messages[index]) delete messages[index];
    messages[index] = new FontStr(font, 47, std::string(msg));
}

void Log::render(SDL_Surface* surface) {
    SDL_Rect r = {2,187,368,46};
    SDL_FillRect(surface, &r, 0);
    int i = index;
    do {if (messages[i]){
                messages[i]->render(surface, font->font, 3, //dammit modulus operator
                                             188+9*((i-index-1+10)%5));
        }
        i=(i+1)%5;
    } while (i != index);
}
