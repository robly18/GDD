#include "engine.hpp"

SDL_Texture* Engine::loadTexture(char* filename, short r, short g, short b) {
    SDL_Surface* s = SDL_LoadBMP(filename);
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, r, g, b));
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

#define LOGERROR() \
    { \
        std::cout<<"An error has occurred: "<<SDL_GetError(); \
        return 1; \
    }

int Engine::init() {
    DEBUGMSG("Initializing engine\n");

    if (SDL_Init( SDL_INIT_EVERYTHING )) LOGERROR()

    std::srand(::time(NULL));

    window = SDL_CreateWindow("Generic Dungeon Dweller", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SWIDTH*2, SHEIGHT*2, SDL_WINDOW_SHOWN);
    if (!window) LOGERROR()

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) LOGERROR()

    SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0xDD, 0);

    texture = SDL_LoadBMP("texture.bmp");
    if (!texture) LOGERROR()

    SDL_SetColorKey(texture, SDL_TRUE, SDL_MapRGB(texture->format, 0xFF, 0, 0xFF));

    SDL_Surface* s = SDL_LoadBMP("terminal.bmp");
    if (!s) LOGERROR()

    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 0, 0, 0));
    font = new BmpFont(s);

    DEBUGMSG("Images and fonts loaded\n");


    map = new Map();
    map->generateMap();


    DEBUGMSG("Map and player made\n");

    ui = new Ui();

    DEBUGMSG("UI Created\n");

    DEBUGMSG("Engine init done\n");

    state = State::RUNNING;
    return 0;
}

void Engine::render() {
    SDL_RenderClear(renderer);
    static SDL_Rect maprect = {2*2, 2*2, 16*16*2, 10*16*2};
    static SDL_Texture* t;

    switch (state) {
    case State::RUNNING:
    case State::ATTACKED:
    case State::MOVED:
    case State::DEAD:
        t = SDL_CreateTextureFromSurface(renderer, map->render(mx, my));
        SDL_RenderCopy(renderer, t, NULL, &maprect);
        SDL_DestroyTexture(t);
        break;
    case State::MAP:
        t = SDL_CreateTextureFromSurface(renderer, map->mapview());
        SDL_RenderCopy(renderer, t, NULL, &maprect);
        SDL_DestroyTexture(t);
        break;
    case State::INV:
        break;
    default: break;
    }

    ui->render(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::checkEvents() {
    SDL_Event e;
    SDL_GetMouseState(&mx, &my);
    mx=mx>>1;
    my=my>>1;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            state = State::QUIT;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_c) {
                camera = !camera;
                if (!camera) map->resetCamera();
            }
            switch (state) {
            case State::RUNNING:
                if (!camera) {
                    switch (e.key.keysym.sym) {
                    case SDLK_DOWN:
                    case SDLK_UP:
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                    case SDLK_SPACE:
                        lastkey = e.key;
                        state = State::MOVED;
                        break;
                    case SDLK_m:
                        map->checkMapData();
                        state = State::MAP;
                        break;
                    //debugging controls:
                    {
                    static int debug_xp = 1;
                    case SDLK_p:
                        map->player->xpholder->levelUp(debug_xp, -1);
                        break;
                    case SDLK_o:
                        std::cin>>debug_xp;
                        break;
                    }

                    default:
                        break;
                    }
                } else {
                    switch (e.key.keysym.sym) {
                    case SDLK_DOWN: map->cameray++; break;
                    case SDLK_UP: map->cameray--; break;
                    case SDLK_LEFT: map->camerax--; break;
                    case SDLK_RIGHT: map->camerax++; break;
                    case SDLK_SPACE: lastkey = e.key; state = State::MOVED; break;

                    default: break;
                    }
                }
                break;
            case State::LOG:
                switch (e.key.keysym.sym) {
                case SDLK_UP:   ui->log->moveReadLine(-1);  break;
                case SDLK_DOWN: ui->log->moveReadLine(1);   break;
                default: break;
                }
                break;
            case State::MAP:
                if (e.key.keysym.sym == SDLK_m) {
                    state = State::RUNNING;
                }
                break;
            default: break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (state == State::RUNNING) {
                int atkx = (mx-2)/16 + map->camerax;
                int atky = (my-2)/16 + map->cameray; //The pos of mouse on the map

                if (e.button.button == SDL_BUTTON_LEFT) {
                    if (map->player->attack) { //todo delegate this mess to map class
                        if (0 <= atkx-map->camerax && atkx-map->camerax < 16 &&
                            0 <= atky-map->cameray && atky-map->cameray < 16) {
                            if (map->player->attack->target(map->player, atkx, atky)) {
                                state = State::ATTACKED;
                                if (!map->player->attack->select(map->player))
                                    map->player->attack = NULL;
                            } else if (map->player->weapon) {
                                map->player->weapon->cancelAttack(map->player);
                            } else {
                                map->player->attack = NULL;
                            }
                        }
                    }
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    map->inspect(Pos{atkx, atky});
                }
            }
            ui->checkClick(true, e.button.button, mx, my);
            break;
        case SDL_MOUSEBUTTONUP:
            ui->checkClick(false, e.button.button, mx, my);
            break;
        }
    }
}

void Engine::doTick() {
    if (state == State::MOVED ||
        state == State::ATTACKED ||
        state == State::USED) {

        do {
            for (auto mob : map->mobs2) {
                int swiftness = mob->getSwiftness();
                if (swiftness) {
                    if (time % swiftness == mob->ai->timeoffset) {
                        if (mob->destructible)
                            mob->destructible->statusholder->update(mob);
                    }
                }
            }
            for (auto mob : map->mobs2) {
                if (mob->getSwiftness())
                if (time % mob->getSwiftness() == mob->ai->timeoffset)
                if (mob->destructible &&
                    !mob->destructible->statusholder->hasEffect(SideEffect::FROZEN)) {
                    mob->ai->update(mob);
                }
            }
            for (auto m = map->mobs2.begin(); m != map->mobs2.end();) {
                if ((*m)->destructible->isDead()) {
                    m = map->killMob(m);
                } else {
                    m++;
                }
            }
            time++;
            if (map->player->destructible->isDead()) return;
        } while (time % map->player->getSwiftness());

        map->updateFovData();
        if (state != State::ATTACKED &&
            state != State::USED &&
            map->player->weapon) map->player->weapon->regenMana(
                                        map->player->weplvls[map->player->weapon->weapontype]);
        if (state == State::USED) state = State::INV;
        else state = State::RUNNING;
    }

}
