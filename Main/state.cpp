#include "state.hpp"

using namespace EngineState;

#define MOUSEINFO mouse.pressed, mouse.h.x, mouse.h.y
#define MOUSEPRESSINFO mouse.p.x, mouse.p.y
#define MOUSEUNPRESSINFO mouse.h.x, mouse.h.y, mouse.p.x, mouse.p.y

/**InGame**/

void InGame::render(Engine &e, SDL_Renderer* renderer, const MouseState mouse) {


    e.ui->buttons->render(MOUSEINFO);
    e.ui->log->render(e.ui->barsurface);
    e.ui->render(renderer);

}



void InGame::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {
    if (ev.type == SDL_MOUSEBUTTONDOWN) {
        e.ui->buttons->checkClick(ev.button.button, MOUSEPRESSINFO);
    } else if (ev.type == SDL_MOUSEBUTTONUP) {
        if (ev.button.button == SDL_BUTTON_LEFT) {
            e.ui->buttons->checkUnclick(MOUSEUNPRESSINFO);
            e.ui->log->checkUnclick(MOUSEUNPRESSINFO);
        }
    } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_i) {
        e.enginestate = (e.enginestate == e.inv) ? e.running : e.inv;
        engine.ui->inv->page = 0;
        engine.ui->inv->selecteditem = NULL;
        engine.ui->inv->updateInventory();
    }
}


/**Running**/

void Running::render(Engine &e, SDL_Renderer* renderer, const MouseState mouse) {

    SDL_Texture* t;

    t = SDL_CreateTextureFromSurface(renderer, e.map->render(mouse.p.x, mouse.p.y));
    SDL_RenderCopy(renderer, t, NULL, &maprect);
    SDL_DestroyTexture(t);


    e.ui->xp->render(MOUSEINFO);
    e.ui->dashboard->render(MOUSEINFO);

    InGame::render(e, renderer, mouse);

}

void Running::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {
    switch (ev.type) {
    case SDL_KEYDOWN:
        switch (ev.key.keysym.sym) {
        case SDLK_m:
            e.map->checkMapData();
            e.enginestate = e.viewmap;
            break;
        case SDLK_c:
            e.camera = !e.camera;
            if (!e.camera) e.map->resetCamera();
            break;
        {
            static int atknum = 3;

            case SDLK_a: atknum--;
            case SDLK_s: atknum--;
            /*case SDLK_d:*/ atknum--;
            if (e.map->player->weapon) e.map->player->weapon->attacks[atknum]->select(e.map->player);
            atknum = 3;
            break;

            case SDLK_q: atknum--;
            case SDLK_w: atknum--;
            /*case SDLK_e:*/ atknum--;
            if (e.map->player->weapon) e.map->player->weapon->defenses[atknum]->select(e.map->player);
            atknum = 3;
            break;
        }

        }
        if (!e.camera) {
            switch (ev.key.keysym.sym) {
                case SDLK_DOWN:
                case SDLK_UP:
                case SDLK_LEFT:
                case SDLK_RIGHT:
                case SDLK_SPACE:
                    e.lastkey = ev.key;
                    e.doTick(e.MOVEDFLAG);
                    break;
                //debugging controls:
                {static int debug_xp = 1;
                case SDLK_p: e.map->player->xpholder->levelUp(debug_xp, -1); break;
                case SDLK_o: std::cin>>debug_xp; break;}
            }
        } else {
            switch (ev.key.keysym.sym) {
                case SDLK_DOWN: e.map->cameray++; break;
                case SDLK_UP: e.map->cameray--; break;
                case SDLK_LEFT: e.map->camerax--; break;
                case SDLK_RIGHT: e.map->camerax++; break;
                case SDLK_SPACE: e.lastkey = ev.key; break;
            }
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        {
            int atkx = (mouse.p.x-2)/16 + e.map->camerax;
            int atky = (mouse.p.y-2)/16 + e.map->cameray; //The pos of mouse on the map

            if (ev.button.button == SDL_BUTTON_LEFT) {
                if (e.map->attack({atkx, atky})) {
                    e.doTick(e.ATTACKEDFLAG);
                }
            } else if (ev.button.button == SDL_BUTTON_RIGHT) {
                e.map->inspect({atkx, atky});
            }
        }
        e.ui->dashboard->checkClick(ev.button.button, MOUSEPRESSINFO);
        e.ui->xp->checkClick(ev.button.button, MOUSEPRESSINFO);
        break;
    case SDL_MOUSEBUTTONUP:
        if (ev.button.button == SDL_BUTTON_LEFT) {
            e.ui->dashboard->checkUnclick(MOUSEUNPRESSINFO);
        }
        break;
    }
    InGame::actOnEvent(e, ev, mouse);
}

void InGame::doTick(Engine &e, int flag) {

}

/**Map**/



void Viewmap::render(Engine &e, SDL_Renderer* renderer, const MouseState mouse) {

    SDL_Texture* t;


    t = SDL_CreateTextureFromSurface(renderer, e.map->mapview());
    SDL_RenderCopy(renderer, t, NULL, &maprect);
    SDL_DestroyTexture(t);


    e.ui->xp->render(MOUSEINFO);
    e.ui->dashboard->render(MOUSEINFO);

    InGame::render(e, renderer, mouse);

}

void Viewmap::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {
    switch (ev.type) {
    case SDL_KEYDOWN:
        switch (ev.key.keysym.sym) {
        case SDLK_m:
            e.enginestate = e.running;
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        e.ui->dashboard->checkClick(ev.button.button, MOUSEPRESSINFO);
        e.ui->xp->checkClick(ev.button.button, MOUSEPRESSINFO);
        break;
    case SDL_MOUSEBUTTONUP:
        if (ev.button.button == SDL_BUTTON_LEFT) {
            e.ui->dashboard->checkUnclick(MOUSEUNPRESSINFO);
        }
        break;
    }
    InGame::actOnEvent(e, ev, mouse);
}

/**Inv**/

void Inv::render(Engine &e, SDL_Renderer* renderer, const MouseState mouse) {

    e.ui->inv->render(MOUSEINFO);

    InGame::render(e, renderer, mouse);

}

void Inv::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {


    if (ev.type == SDL_MOUSEBUTTONDOWN) {
        e.ui->inv->checkClick(ev.button.button, MOUSEPRESSINFO);
    } else if (ev.type == SDL_MOUSEBUTTONUP) {
        if (ev.button.button == SDL_BUTTON_LEFT) {
            e.ui->inv->checkUnclick(MOUSEUNPRESSINFO);
        }
    }
    InGame::actOnEvent(e, ev, mouse);
}

/**Prompt**/

void Viewprompt::render(Engine &e, SDL_Renderer *renderer, const MouseState mouse) {

    SDL_Texture* t;

    t = SDL_CreateTextureFromSurface(renderer, e.map->render(mouse.p.x, mouse.p.y));
    SDL_RenderCopy(renderer, t, NULL, &maprect);
    SDL_DestroyTexture(t);

    e.ui->xp->render(MOUSEINFO);
    e.ui->dashboard->render(MOUSEINFO);
    const Pos* p = mouse.pressed ? &mouse.h : NULL;
    prompt->render(e.ui->barsurface, p);

    InGame::render(e, renderer, mouse);
}

void Viewprompt::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {
    if (ev.type == SDL_MOUSEBUTTONUP) {
        prompt->unclick(mouse.h, mouse.p);
    }
}

/**Log**/


void Viewlog::render(Engine &e, SDL_Renderer* renderer, const MouseState mouse) {

    e.ui->buttons->render(MOUSEINFO);
    e.ui->render(renderer);

}

void Viewlog::actOnEvent(Engine &e, SDL_Event &ev, const MouseState mouse) {

    switch (ev.key.keysym.sym) {
    case SDLK_UP:   e.ui->log->moveReadLine(-1);  break;
    case SDLK_DOWN: e.ui->log->moveReadLine(1);   break;
    }
}

