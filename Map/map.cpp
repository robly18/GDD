#include "map.hpp"

Map::Map () :
    nowall(new TileProperties{false, true}),
    wall(new TileProperties{true, false}),
    exit(new TileProperties{false, true, TileProperty::EXIT}),

    fovcomputer(new FovComputer(MAPWIDTH, MAPHEIGHT)),
    pathfinder(new Pathfinder(MAPWIDTH, MAPHEIGHT)),

    generator(new MapGenerator),

    surface(SDL_CreateRGBSurface(0, SCREENTILEW*16, SCREENTILEH*16, 24, 0, 0, 0, 0)),

    highlightsurface(SDL_CreateRGBSurface(0, 16*16, 10*16, 24,
                                                    0x0000FF,
                                                    0x00FF00,
                                                    0xFF0000,
                                                    0xFFFFFF)) {
    SDL_SetSurfaceAlphaMod(highlightsurface, 128+64);

    makePlayer();

    player->inventory->addItem(new Armor("TestArmr", 2, 10));
    player->inventory->addItem(new Staff("Tststaff", 10, 10, -10, 10));
    player->inventory->addItem(new Sword("SORD....", 10, 3));
    player->inventory->addItem(new HpPotion("t1ck", 10));
    player->inventory->addItem(new HpPotion("t0ck", 10));
    //player->inventory->addItem(new Bow("BowB4Me", 10, 7, 10, 20));
};

void Map::restartMap() {
    for (auto m : mobs1) delete m;
    mobs1.clear();

    for (auto m : items) delete m;
    items.clear();

    for (auto m : mobs2) delete m;
    mobs2.clear();

    player = nullptr;

    makePlayer();
    generateMap();
}

std::list<Mob*>::iterator Map::killMob(std::list<Mob*>::iterator m) {
    Mob* mob = *m;

    mob->a->r = mob->destructible->deadSprite;
    delete mob->ai;
    mob->ai = nullptr;

    mob->destructible->dropItems(mob);

    mobs1.push_back(mob);

    if (mobs2.size() - 1 == 1) {
        spawnExit(mob->getPos());
    }

    return mobs2.erase(m);
}

void Map::spawnExit(Pos p) {
    tiles[POSTOINDEX(p)].properties = exit;
}

bool Map::canMoveTo(int x, int y) {
    if (isWall(x, y)) return false;
    for (std::list<Mob*>::iterator m = mobs2.begin(); m != mobs2.end(); m++) {
        if ((*m)->x == x && (*m)->y == y) {
            return false;
        }
    }
    return true;
}

bool Map::canMoveTo(Pos p) {
    return canMoveTo(p.x, p.y);
}

void Map::updateFovData() {
    int losrange = player->sght;
    for (int x = -losrange-1; x != losrange+2; x++)
    for (int y = -losrange-1; y != losrange+2; y++) {
        int cx = player->x + x;
        int cy = player->y + y;
        if (INBOUNDS(cx, cy)) {
            tiles[cx+MAPWIDTH*cy].isSeen = fovcomputer->isInSight(cx, cy, player->x, player->y, losrange);
            if (tiles[cx+MAPWIDTH*cy].isSeen) tiles[cx+MAPWIDTH*cy].hasBeenSeen = true;
        }
    }
}

bool Map::hasBeenSeen(int x, int y) const {
    if (!INBOUNDS(x, y)) return false;
    return tiles[x+y*MAPWIDTH].hasBeenSeen;
}

bool Map::isBeingSeen(int x, int y) const {
    if (!INBOUNDS(x, y)) return false;
    return tiles[x+y*MAPWIDTH].isSeen;
}

bool Map::canSeeThrough(int x, int y) const {
    return INBOUNDS(x, y) && tiles[x+y*MAPWIDTH].properties->seeThrough;
}

bool Map::hasFlag(int x, int y, int flag) const {
    return INBOUNDS(x,y) && (tiles[x+y*MAPWIDTH].properties->flags & flag);
}

FloorInventory* Map::getInvAt(int x, int y) const {
    for (auto inv : items) {
        if (inv->x == x && inv->y == y) {
            return inv;
        }
    }
    return NULL;
}

FloorInventory* Map::addInvTo(Inventory* inv, int x, int y) {
    FloorInventory* finv = getInvAt(x, y);
    if (!finv) {
        finv = new FloorInventory(engine.texture, x,y);
        items.push_back(finv);
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
        items.push_back(finv);
    }
    finv->items.addItem(i);
    return finv;
}

FloorInventory* Map::removeInvAt(int x, int y) {
    for (std::list<FloorInventory*>::iterator inv = items.begin(); inv != items.end(); inv++) {
        if ((*inv)->x == x && (*inv)->y == y) {
            items.remove(*inv);
            return *inv;
        }
    }
    return NULL;
}

bool Map::attack (Pos pos) {
    if (player->attack) { //todo delegate this mess to map class
        if (0 <= pos.x-camerax && pos.x-camerax < 16 && //ok done
            0 <= pos.y-cameray && pos.y-cameray < 16) {
            if (player->attack->target(player, pos.x, pos.y)) {
                if (!player->attack->select(player))
                    player->attack = NULL;
                return true;
            } else if (player->weapon) {
                player->weapon->cancelAttack(player);
            } else {
                player->attack = NULL;
            }
        }
    }
    return false;
}

SDL_Surface* Map::render(int mousex, int mousey) {
    static SDL_Rect floor = {0,16,16,16}, flooroos = {16,16,16,16};
    static SDL_Rect wall = {0,32,16,16}, walloos = {16,32,16,16};
    static SDL_Rect unseen = {0, 48, 16, 16};
    static SDL_Rect stairs = {32, 32, 16, 16}, stairsoos = {48, 32, 16, 16};
    //Ugly hack, do remove in the future

    for (int x = 0; x != SCREENTILEW; x++)
    for (int y = 0; y != SCREENTILEH; y++) {
        SDL_Rect r = SDL_Rect{x*16, y*16, 16, 16}; //this will need refinement soon
        if (INBOUNDS(x+camerax, y+cameray) && hasBeenSeen(x+camerax, y+cameray)) {
            if (!hasFlag(x+camerax, y+cameray, TileProperty::EXIT)) {
                if (isBeingSeen(x+camerax, y+cameray)) {
                    SDL_BlitSurface(engine.texture, !isWall(x+camerax,y+cameray) ? &floor : &wall,
                                    surface, &r);
                } else {
                    SDL_BlitSurface(engine.texture, !isWall(x+camerax,y+cameray) ? &flooroos : &walloos,
                                    surface, &r);
                }
            } else {
                SDL_BlitSurface(engine.texture,
                                isBeingSeen(x+camerax, y+cameray) ? &stairs : &stairsoos,
                                surface, &r);
            }
        } else {
            SDL_BlitSurface(engine.texture, &unseen,
                            surface, &r);
        }
    }
    for (auto m : mobs1) {
        if (isBeingSeen(m->x, m->y)) {
            renderMobActor(m->a);
        }
    }
    for (auto m : items) {
        if (isBeingSeen(m->x, m->y)) {
            renderMobActor(m->a);
        }
    }
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
    for (auto m : mobs2) {
        #ifndef SEEALLMOBS
        if (isBeingSeen(m->x, m->y))
        #endif
        {
            SDL_Rect pos = renderMobActor(m->a);
            if (m->destructible) {
                m->destructible->statusholder->render(surface, pos);
            }
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

void Map::resetCamera() { //wtf was i on when i wrote this code
    camerax = ((player->x - SCREENTILEW/2 + 7) & ~7) - 3;
    cameray = ((player->y - SCREENTILEH/2 + 3) & ~3) - 1;
}

SDL_Surface* Map::mapview() {
    static SDL_Rect floor = {0,16,4,4};
    static SDL_Rect wall = {0,32,4,4};
    static SDL_Rect unseen = {0, 48, 4, 4};
    static SDL_Rect enemy = {36, 24, 4, 4};
    for (int x = 0; x != MAPWIDTH; x++)
    for (int y = 0; y != MAPHEIGHT; y++) {
        SDL_Rect r = SDL_Rect{x*4, y*4, 4, 4};
        if (hasBeenSeen(x, y))
            SDL_BlitSurface(engine.texture, !isWall(x,y) ? &floor : &wall,
                            surface, &r);
        else
            SDL_BlitSurface(engine.texture, &unseen,
                            surface, &r);
    }
    if (seentiles * 2 > seeabletiles)
    for (auto m : mobs2) {
        if (m != player) {
            SDL_Rect r = SDL_Rect{m->spawn.x * 4, m->spawn.y * 4, 4, 4};
            SDL_BlitSurface(engine.texture, &enemy, surface, &r);
        }
    }
    return surface;
}

void Map::checkMapData() {
    seentiles = 0;
    for (Tile t : tiles) {
        if (t.properties->seeThrough &&
            t.hasBeenSeen) {
            seentiles++;
        }
    }
}

/**Map Generation**/

void Map::generateMap() {
    DEBUGMSG("Erasing what was already here\n");

    for (auto m : mobs1) delete m;
    for (auto i : items) delete i;
    assert(mobs2.size() == 1); //in principle, there should only be the player...

    mobs1.clear();
    items.clear();

    DEBUGMSG("Generating new map\n");

    generator->generateMap(this, wall, nowall);
    DEBUGMSG("Populating...\n");

    generator->populateMap(this);
    DEBUGMSG("Done.\n");

    seeabletiles = 0; seentiles = 0;

    for (int x = 0; x != MAPWIDTH; x++)
    for (int y = 0; y != MAPHEIGHT; y++) {
        fovcomputer->tiledata[x+y*MAPWIDTH] = !canSeeThrough(x, y);
        pathfinder->tiledata[x+y*MAPWIDTH] = !isWall(x, y);
        if (canSeeThrough(x, y)) {
            seeabletiles++;
        }

        tiles[x+y*MAPWIDTH].isSeen = false;
        tiles[x+y*MAPWIDTH].hasBeenSeen =
                            #ifdef SEEALLTILES
                                true;
                            #else
                                false;
                            #endif
    }

    resetCamera();

    updateFovData();
}

void Map::makePlayer() {
    if (!player ) {
        player = new Player(5,5,SDL_Rect{0, 64, 16, 16}, engine.texture,"Player");
        player->ai = new PlayerAi;
        player->destructible = new PlayerDestructible(50, SDL_Rect{0,8,4,4});
        player->inventory = new PlayerInventory;
        mobs2.push_back(player);
    }
}

/** Map inspection **/

void Map::inspect(Pos p) const {
    if (isBeingSeen(p.x, p.y)) {
        std::cout<<"In this tile, there are:\n";
        for (auto mob : mobs1) {
            if (mob->x == p.x && mob->y == p.y) {
                std::cout<<mob->name<<"\n";
            }
        }
        auto inv = getInvAt(p.x, p.y);
        if (inv) {
            std::cout<<"An inv containing:\n";
            for (int i = 0; i != inv->items.itemNo(); i++) {
                std::cout<<inv->items.getItem(i)->name<<"\n";
            }
        }
        for (auto mob : mobs2) {
            if (mob->x == p.x && mob->y == p.y) {
                /*Important data about a mob:
                 *Name (duh)
                 *Hp
                 *ATK power<-
                 *DEFense <--these 2 are for later
                 *Swiftness (Relative)
                 *Offset
                 *Sight
                 **Maybe more later... TODO make some of these shenanigans up to the mob
                **/
                char buffer[255];
                engine.ui->log->addMessage(buffer,
                            "Inspecting mob: %s", mob->name.c_str());
                engine.ui->log->addMessage(buffer,
                            "SGHT: %i", mob->sght);
                engine.ui->log->addMessage(buffer,
                            "HP: %i/%i", mob->destructible->hp, mob->destructible->maxHp);
                engine.ui->log->addMessage(buffer,
                            "Swiftness: %i (%i)",
                                    mob->getSwiftness(),
                                    player->getSwiftness() - mob->getSwiftness());
                engine.ui->log->addMessage(buffer,//I'll be honest with you:
                            "Time until next move: %i", //I'm not completely sure what
                            mob->getSwiftness() - //this does but it works so keep it
                            (engine.time - mob->ai->timeoffset - 1) % mob->getSwiftness() - 1);
            }
        }
    }
}

