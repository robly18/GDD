#include "mapgenerator.hpp"

void MapGenerator::generateMap(Map* map) {
    makeRect(SDL_Rect{0, 0, MAPWIDTH, MAPHEIGHT}, true, map->tiles);

    int prevx, prevy;
    int newx, newy;
    SDL_Rect r;

    r.w = (std::rand() % 6) + 4;
    r.h = (std::rand() % 6) + 4;
    r.x = std::rand() % (MAPWIDTH-r.w);
    r.y = std::rand() % (MAPHEIGHT-r.h);

    newx = r.x + r.w/2;
    newy = r.y + r.h/2;
    makeRect(r, false, map->tiles);

    map->player->setPos(newx, newy);

    for (int n = 0; n != 10; n++) {
        prevx = newx;
        prevy = newy;

        r.w = (std::rand() % 6) + 4;
        r.h = (std::rand() % 6) + 4;
        r.x = std::rand() % (MAPWIDTH-r.w);
        r.y = std::rand() % (MAPHEIGHT-r.h);

        newx = r.x + r.w/2;
        newy = r.y + r.h/2;
        makeRect(r, false, map->tiles);
        for (int x = newx; x != prevx; newx<prevx?x++:x--) {
            map->tiles[x+MAPWIDTH*newy].blocking = false;
        }
        for (int y = newy; y != prevy; newy<prevy?y++:y--) {
            map->tiles[prevx+MAPWIDTH*y].blocking = false;
        }
    }
}

/* Algorithm overview:

Make a bunch of rectangles and connect every subsequent one by a path
*/

void MapGenerator::makeRect(SDL_Rect r, bool w, Tile* tiles) {
    for (int x = r.x; x != r.x+r.w; x++)
    for (int y = r.y; y != r.y+r.h; y++) {
        tiles[x+MAPWIDTH*y].blocking = w;
    }
}

void MapGenerator::populateMap(Map* map) {
    Mob* m = new Mob(3,2,SDL_Rect{0,8,8,8}, engine.texture,"AI Test Mob");
    m->ai = new TestAi();
    m->destructible = new MobDestructible(100, SDL_Rect{0,0,10,10});
    m->destructible->armor = new Armor("Testarmr", 4, 20);
    m->attack = new TargetedAttack(10, 0, 0, 10);
    m->inventory = new MobInventory;
    m->inventory->addItem(new HpPotion("HPPot?", 100));
    map->mobs2.push(m);

    map->mobs2.push(new Mob(2,5,SDL_Rect{8,8,8,8}, engine.texture,"Lazy bum"));
}

