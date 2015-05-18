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

    if (SDL_Init( SDL_INIT_EVERYTHING )) LOGERROR()

    std::srand(RANDSEED);

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


    database = new Database;
    interpreter = new Interpreter;
    if (!interpreter->open("mobs.data")) {
        std::cout<<"Failed to open mobs.data file\n";
        return 1;
    }

    interpreter->parseToDatabase(*database);

    interpreter->close();

    for (auto m : database->mobdefs) {
        std::cout<<"\nThere's a mobdef here with name "<<m.second.name<<" and "<<m.second.hp<<" hp\n";
    }

    map = new Map();
    map->generateMap(*database);



    ui = new Ui();

    DEBUGMSG("UI Created\n");

    DEBUGMSG("Engine init done\n");


    running = new EngineState::Running;
    inv = new EngineState::Inv;
    viewmap = new EngineState::Viewmap;
    viewlog = new EngineState::Viewlog;
    viewprompt = new EngineState::Viewprompt;

    enginestate = running;

    return 0;
}

void Engine::restartGame() {
    map->restartMap();

    delete ui;
    ui = new Ui;

    time = 0;
}

void Engine::render() {
    SDL_RenderClear(renderer);

    enginestate->render(*this, renderer, mouse);

    SDL_RenderPresent(renderer);
}

int Engine::checkEvents() {
    SDL_Event e;
    SDL_GetMouseState(&mouse.p.x, &mouse.p.y);
    mouse.p.x /= 2;
    mouse.p.y /= 2;
    while (SDL_PollEvent(&e)) {
        enginestate->actOnEvent(*this, e, mouse);
        if (!enginestate) return 1;
        switch (e.type) {
        case SDL_QUIT:
            return 1;
            break;//todo sort this ALLLLLLLL out
        case SDL_MOUSEBUTTONDOWN://sorted out :D...mostly
            mouse.pressed = true;
            mouse.h = mouse.p;
            break;
        case SDL_MOUSEBUTTONUP:
            mouse.pressed = false;
            break;
        }
    }
    return 0;
}

void Engine::doTick(int flag) {

    if (flag != MOVEDFLAG) lastkey.keysym.sym = SDLK_SPACE;

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
                !mob->destructible->statusholder->hasEffect(SideEffect::FROZEN) &&
                !mob->destructible->isDead()) {
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

    if (flag == MOVEDFLAG && map->player->weapon) {
            map->player->weapon->regenMana(
                            map->player->weplvls[map->player->weapon->weapontype]);
    }

    if (flag != USEDFLAG)
        if (map->hasFlag(map->player->x, map->player->y, TileProperty::EXIT)) {
            ui->addPrompt(new NextLevelPrompt);
        }

}
