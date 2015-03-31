#include "prompt.hpp"

Prompt::Prompt(std::string text, std::string opt1, std::string opt2) {
    figureOutText(text);
    figureOutButtons(opt1, opt2);
}

void Prompt::figureOutText(std::string s) {

    std::vector <std::string> lines;

    std::string nextline;

    for (auto c : s) {
        if (c == '\n') {
            lines.push_back(nextline);
            nextline.clear();
        } else {
            nextline += c;
        }
    }
    lines.push_back(nextline);

    int i = 0;
    int maxlinew = 0;
    for (auto &line : lines) {
        text.push_back(FontStrPos(
                            std::unique_ptr<FontStr>
                                (new FontStr(engine.font, line.size(), line)),
                            Pos{(SWIDTH-line.size()*8)/2,
                            (SHEIGHT-lines.size()*8)/2 + i*9 - 1}));
        maxlinew = std::max(maxlinew, (int)line.size());
        i++;
    }

    promptwindow = SDL_Rect{(SWIDTH - maxlinew*8)/2 - MARGIN, (SHEIGHT-lines.size()*9)/2 - MARGIN,
                            maxlinew*8 + MARGIN*2, i*9-1 + 8 + TEXTMARGIN*2 + MARGIN*3};
}

void Prompt::figureOutButtons(std::string o1, std::string o2) {
    int y = promptwindow.y + promptwindow.h - MARGIN - 8 - TEXTMARGIN*2;
    int midx = promptwindow.x + promptwindow.w/2;
    int maxsize = std::max(o1.size(), o2.size());
    button1 = SDL_Rect {midx - 1 - TEXTMARGIN*2 - maxsize*8, y,
                        maxsize*8 + TEXTMARGIN*2, 8 + TEXTMARGIN*2};
    opt1 = FontStrPos(std::unique_ptr<FontStr>(new FontStr(engine.font, o1.size(), o1)),
                      Pos{button1.x + TEXTMARGIN + (maxsize - o1.size())*4,
                            button1.y + TEXTMARGIN});

    button2 = SDL_Rect {midx + 1, y,
                        maxsize*8 + TEXTMARGIN*2, 8 + TEXTMARGIN*2};
    opt2 = FontStrPos(std::unique_ptr<FontStr>(new FontStr(engine.font, o2.size(), o2)),
                      Pos{button2.x + TEXTMARGIN + (maxsize - o2.size())*4,
                            button2.y + TEXTMARGIN});
}

void Prompt::render(SDL_Surface* surface, const Pos* pos) {
    SDL_FillRect(surface, &promptwindow, PROMPTCOLOR);
    SDL_FillRect(surface, &button1, pos && isInRect(button1, pos->x, pos->y) ?
                                        CLICKEDBUTTONCOLOR : UNCLICKEDBUTTONCOLOR);
    SDL_FillRect(surface, &button2, pos && isInRect(button2, pos->x, pos->y) ?
                                        CLICKEDBUTTONCOLOR : UNCLICKEDBUTTONCOLOR);
    for (auto &line : text) {
        line.first->render(surface, engine.font->font, line.second.x, line.second.y);
    }
    opt1.first->render(surface, engine.font->font, opt1.second.x, opt1.second.y);
    opt2.first->render(surface, engine.font->font, opt2.second.x, opt2.second.y);
}

void Prompt::unclick(const Pos hp, const Pos p) {
    if (isInRect(button1, p.x, p.y) &&
        isInRect(button1, hp.x, hp.y)) {
        doOpt1();
    } else if (isInRect(button2, p.x, p.y) &&
                isInRect(button2, hp.x, hp.y)) {
        doOpt2();
    }
}



NextLevelPrompt::NextLevelPrompt() :
    Prompt("You have reached\nthe end of the level!\n\
Do you want to\ngo to the next level?", "Not yet", "Yeah!") {}

void NextLevelPrompt::doOpt1() {
    engine.ui->closePrompt(engine.running);
}

void NextLevelPrompt::doOpt2() {
    engine.ui->closePrompt(engine.running);

    engine.map->generateMap();
}


DeadPrompt::DeadPrompt() :
    Prompt("Whoops. It seems that you've died.\nWould you like to try again?",
           "Respawn", "Ragequit") {}

void DeadPrompt::doOpt1() {
    engine.restartGame();
    engine.ui->closePrompt(engine.running);
}

void DeadPrompt::doOpt2() {
    engine.ui->closePrompt(nullptr);
}
