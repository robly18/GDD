#include "map.hpp"

bool Map::canMoveTo(int x, int y) {
    if (isWall(x, y)) return false;
    for (Mob** m = mobs2.begin(); m != mobs2.end(); m++) {
        if ((*m)->x == x && (*m)->y == y) {
            return false;
        }
    }
    return true;
}

FloorInventory* Map::getInvAt(int x, int y) {
    for (FloorInventory** inv = items.begin(); inv != items.end(); inv++) {
        if ((*inv)->x == x && (*inv)->y == y) {
            return *inv;
        }
    }
    return NULL;
}

FloorInventory* Map::addInvTo(Inventory* inv, int x, int y) {
    FloorInventory* finv = getInvAt(x, y);
    if (!finv) {
        finv = new FloorInventory(engine.texture, x,y);
        items.push(finv);
    }
    for (int n = 0; n != inv->itemNo(); n++) {
        finv->items.addItem(inv->itemAtSlot(n));
    }
    return finv;
}

FloorInventory* Map::dropItem(Item* i, int x, int y) {
    FloorInventory* finv = getInvAt(x, y);
    if (!finv) {
        finv = new FloorInventory(engine.texture, x,y);
        items.push(finv);
    }
    finv->items.addItem(i);
    return finv;
}

FloorInventory* Map::removeInvAt(int x, int y) {
    for (FloorInventory** inv = items.begin(); inv != items.end(); inv++) {
        if ((*inv)->x == x && (*inv)->y == y) {
            items.remove(inv);
            return *inv;
        }
    }
    return NULL;
}

SDL_Surface* Map::render(int mousex, int mousey) {
    static SDL_Rect floor = {0,16,16,16};
    static SDL_Rect wall = {0,32,16,16};
    for (int x = 0; x != SCREENTILEW; x++)
    for (int y = 0; y != SCREENTILEH; y++) {
        SDL_Rect r = SDL_Rect{x*16, y*16, 16, 16};
        SDL_BlitSurface(engine.texture, !isWall(x+camerax,y+cameray) ? &floor : &wall,
                        surface, &r);
    }
    for (Mob** m = mobs1.begin(); m != mobs1.end(); m++)
        renderMobActor((*m)->a);
    for (FloorInventory** m = items.begin(); m != items.end(); m++)
        renderMobActor((*m)->a);
    if (player->attack) {
        for (int x = 0; x != SCREENTILEW; x++)
        for (int y = 0; y != SCREENTILEH; y++) {
            SDL_Rect r = {x*16, y*16, 16, 16};
            SDL_FillRect(highlightsurface, &r, player->attack->highlightColor(
                                            player,
                                            x+camerax, y+cameray,
                                            (mousex-2)/16+camerax,
                                            (mousey-2)/16+cameray));
        }
        SDL_BlitSurface(highlightsurface, NULL, surface, NULL);
    }
    for (Mob** m = mobs2.begin(); m != mobs2.end(); m++) {
        SDL_Rect pos = renderMobActor((*m)->a);
        if ((*m)->destructible) {
            (*m)->destructible->statusholder->render(surface, pos);
        }
    }
    return surface;
}

SDL_Rect Map::renderMobActor(Actor* a) {
    SDL_Rect pos = a->p;
    pos.x -= 16*camerax;
    pos.y -= 16*cameray;
    SDL_BlitSurface(a->texture, &a->r, surface, &pos);
    return pos;
}

void Map::resetCamera() {
    camerax = ((player->x - SCREENTILEW/2 + 7) & ~7) - 3;
    cameray = ((player->y - SCREENTILEH/2 + 7) & ~7) - 3;
}

/**Map Generation**/

void Map::generateMap() {
    generateDungeon();

    makePlayer();
    populateDungeon();

    //player->inventory->addItem(new Armor("TestArmr", 8, -80));
    player->inventory->addItem(new Staff("Tststaff", 10, 10));
    player->inventory->addItem(new Sword("SORD....", 10, 3));
    player->inventory->addItem(new Bow("BowB4Me", 10, 7));

    resetCamera();
}

void Map::makePlayer() {
    if (!player ) {
        player = new Player(5,5,SDL_Rect{0, 0, 16, 16}, engine.texture,"Player");
        player->ai = new PlayerAi;
        player->destructible = new PlayerDestructible(50, SDL_Rect{0,8,4,4});
        player->inventory = new PlayerInventory;
        mobs2.push(player);
    }
}

void Map::populateDungeon() {
    Mob* m = new Mob(3,2,SDL_Rect{0,8,8,8}, engine.texture,"AI Test Mob");
    m->ai = new TestAi();
    m->destructible = new MobDestructible(100, SDL_Rect{0,0,10,10});
    m->destructible->armor = new Armor("Testarmr", 4, 20);
    m->attack = new TargetedAttack(10, 0, 0, 10);
    m->inventory = new MobInventory;
    m->inventory->addItem(new HpPotion("HPPot?", 100));
    mobs2.push(m);

    mobs2.push(new Mob(2,5,SDL_Rect{8,8,8,8}, engine.texture,"Lazy bum"));
}

void Map::generateDungeon() {
    setWall(8,8,true);
}

