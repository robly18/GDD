#include "engine.hpp"

SDL_Texture* Engine::loadTexture(char* filename, short r, short g, short b) {
    SDL_Surface* s = SDL_LoadBMP(filename);
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, r, g, b));
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

#define LOGERROR() { \
        std::cout<<"An error has occurred: "<<SDL_GetError(); \
        return 1; \
    }

int Engine::init() {
    DEBUGMSG("Initializing engine\n");

    if (SDL_Init( SDL_INIT_EVERYTHING )) LOGERROR()

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
    Player* player = new Player(5,5,SDL_Rect{0, 0, 16, 16}, texture,"Player");
    player->ai = new PlayerAi;
    player->destructible = new PlayerDestructible(50, SDL_Rect{0,8,4,4});
    player->inventory = new PlayerInventory;
    //player->inventory->addItem(new Armor("TestArmr", 8, -80));
    player->inventory->addItem(new Staff("Tststaff", 10, 10));
    player->inventory->addItem(new Sword("SORD....", 10, 3));
    player->inventory->addItem(new Bow("BowB4Me", 10, 7));
    map->mobs2.push(player);
    map->player = player;

    DEBUGMSG("Map and player made\n");

    ui = new Ui();

    DEBUGMSG("UI Created\n");


    testattack = new TargetedAttack(10, 0, 0, 10);

    map->setWall(0,0,true);
    Mob* m = new Mob(3,2,SDL_Rect{0,8,8,8}, texture,"AI Test Mob");
    m->ai = new TestAi();
    m->destructible = new MobDestructible(10, SDL_Rect{0,0,10,10});
    m->destructible->armor = new Armor("Testarmr", 10, 20);
    m->attack = testattack;
    m->inventory = new MobInventory;
    m->inventory->addItem(new HpPotion("HPPot?", 100));
    map->mobs2.push(m);
    map->mobs2.push(new Mob(2,5,SDL_Rect{8,8,8,8}, texture,"Lazy bum"));

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
            if (state == State::RUNNING) {
                lastkey = e.key;
                state = State::MOVED;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (state == State::RUNNING) {
                if (map->player->attack) {
                    if (map->player->attack->target(map->player, (mx-2)/16, (my-2)/16)) {
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
            ui->checkClick(true, Mouse::LMB, mx, my);
            break;
        case SDL_MOUSEBUTTONUP:
            ui->checkClick(false, Mouse::LMB, mx, my);
            break;
        }
    }
}

void Engine::doTick() {
    if (state == State::MOVED ||
        state == State::ATTACKED ||
        state == State::USED) {
        do {
            for (Mob** mob = map->mobs2.begin(); mob != map->mobs2.end(); mob++) {
                if ((*mob)->ai)
                if (time % (*mob)->getSwiftness() == 0) {
                    (*mob)->ai->update(*mob);
                }
            }
            time++;
            if (map->player->destructible->isDead()) return;
        } while (time % map->player->getSwiftness());
        if (state != State::ATTACKED &&
            state != State::USED &&
            map->player->weapon) map->player->weapon->regenMana(map->player->str);
        if (state == State::USED) state = State::INV;
        else state = State::RUNNING;
    }

}
