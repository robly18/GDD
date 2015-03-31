#ifndef PROMPT_H
#define PROMPT_H

#include "../Main/main.hpp"
#include <memory>

#define MARGIN 8
#define TEXTMARGIN 4

#define PROMPTCOLOR 0xFF4444
#define UNCLICKEDBUTTONCOLOR 0xAA3333
#define CLICKEDBUTTONCOLOR 0x772222

typedef std::pair<std::unique_ptr<FontStr>, Pos> FontStrPos;

class Prompt {
public:
    Prompt(std::string text, std::string opt1, std::string opt2);
    virtual ~Prompt() {}

    void                        render(SDL_Surface*, const Pos* = nullptr);

    void                        unclick(const Pos hp, const Pos p);


private:
    std::vector<FontStrPos>     text;

    FontStrPos                  opt1,
                                opt2;

    virtual void                doOpt1() = 0;
    virtual void                doOpt2() = 0;

    SDL_Rect                    promptwindow,
                                button1,
                                button2;

    void                        figureOutText(std::string);
    void                        figureOutButtons(std::string, std::string);
};

class NextLevelPrompt : public Prompt {
public:
    NextLevelPrompt();

private:
    void                        doOpt1();
    void                        doOpt2();
};

class DeadPrompt : public Prompt {
public:
    DeadPrompt();
private:
    void                        doOpt1();
    void                        doOpt2();
};

#endif
