#include "ui.hpp"

Ui::Ui() {
    barsurface = SDL_CreateRGBSurface(0, SWIDTH, SHEIGHT, 24, 0,0,0,0);
    SDL_SetColorKey(barsurface, SDL_TRUE, 0xFF00FF);




    dashboard = new UiDashboard(barsurface);

    inv = new UiInv(barsurface);
    buttons = new UiButtons(barsurface);

    xp = new UiXP(barsurface);


    log = new Log(engine.font);
}

void Ui::render(SDL_Renderer* renderer) { //okay next step is this
        //glhf future self

    SDL_Texture* ui = SDL_CreateTextureFromSurface(renderer, barsurface);
    SDL_RenderCopy(renderer, ui, NULL, NULL);
    SDL_DestroyTexture(ui);
    SDL_FillRect(barsurface, NULL, 0xFF00FF);
}

void Ui::addPrompt(Prompt *p) {
    assert(engine.viewprompt->prompt==nullptr);
    engine.viewprompt->prompt = p;
    engine.enginestate = engine.viewprompt;
}

void Ui::closePrompt(EngineState::State *s) {
    assert(engine.viewprompt->prompt!=nullptr);
    delete engine.viewprompt->prompt;
    engine.viewprompt->prompt = nullptr;
    engine.enginestate = s;
}
