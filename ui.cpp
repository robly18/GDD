#include "ui.hpp"

Ui::Ui() {
    DEBUGMSG("Creating UI\n");
    barsurface = SDL_CreateRGBSurface(0, SWIDTH, SHEIGHT, 24, 0,0,0,0);
    SDL_SetColorKey(barsurface, SDL_TRUE, 0xFF00FF);

    DEBUGMSG("Creating buttons\n");

    DEBUGMSG("Dashboard\n");
    dashboard = new UiDashboard(barsurface);
    DEBUGMSG("Inv\n");
    inv = new UiInv(barsurface);
    DEBUGMSG("\n\n"<<sizeof(FontStr)<<"\n\n");
    buttons = new UiButtons(barsurface);

    DEBUGMSG("Buttons created\n");

    log = new Log(engine.font);
    DEBUGMSG("UI Created\n");
}

void Ui::render(SDL_Renderer* renderer) {
    SDL_FillRect(barsurface, NULL, 0xFF00FF);

    buttons->render(h, hx, hy);

    if (engine.state != engine.State::INV) {
        dashboard->render(h, hx, hy);
    } else {
        inv->render(h, hx, hy);
    }

    log->render(barsurface);

    SDL_Texture* ui = SDL_CreateTextureFromSurface(renderer, barsurface);
    SDL_RenderCopy(renderer, ui, NULL, NULL);
    SDL_DestroyTexture(ui);
}

void Ui::checkClick(bool clicking, int button, int x, int y) {
    if (clicking) {
        h = true; hx = x; hy = y;
        buttons->checkClick(button, x, y);
        if (engine.state != engine.State::INV) dashboard->checkClick(button, x, y);
        else inv->checkClick(button, x, y);
    } else {
        h = false;
        buttons->checkUnclick(hx, hy, x, y);
        if (engine.state != engine.State::INV) dashboard->checkUnclick(hx, hy, x, y);
        else inv->checkUnclick(hx, hy, x, y);
    }
}