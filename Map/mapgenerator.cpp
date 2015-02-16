#include "mapgenerator.hpp"

//#define BLANKMAP

void MapGenerator::generateMap(Map* map) {
    makeRect(SDL_Rect{0, 0, MAPWIDTH, MAPHEIGHT}, false, map->tiles);

    #ifndef BLANKMAP
    makeRect(SDL_Rect{0, 0, MAPWIDTH, MAPHEIGHT}, true, map->tiles);

    int prevx, prevy;
    int newx, newy;
    SDL_Rect r;

    r.w = (std::rand() % 6) + 4;
    r.h = (std::rand() % 6) + 4;
    r.x = std::rand() % (MAPWIDTH-r.w-2) + 1;
    r.y = std::rand() % (MAPHEIGHT-r.h-2) + 1;

    newx = r.x + r.w/2;
    newy = r.y + r.h/2;
    makeRect(r, false, map->tiles);

    map->player->setPos(newx, newy);

    for (int n = 0; n != 10; n++) {
        prevx = newx;
        prevy = newy;

        newpos:
        r.w = (std::rand() % 6) + 4;
        r.h = (std::rand() % 6) + 4;
        r.x = std::rand() % (MAPWIDTH-r.w-2)+1;
        r.y = std::rand() % (MAPHEIGHT-r.h-2)+1;

        newx = r.x + r.w/2;
        newy = r.y + r.h/2;
        if ((newx - prevx < 16 && newx - prevx > -16) ||
            (newy - prevy < 10 && newy - prevy > -10)) {
            DEBUGMSG("Rejected room\n");
            goto newpos;
        }
        makeRect(r, false, map->tiles);
        for (int x = newx; x != prevx; newx<prevx?x++:x--) {
            map->tiles[x+MAPWIDTH*newy].blocking = false;
            map->tiles[x+MAPWIDTH*newy].seeThrough = true;
        }
        for (int y = newy; y != prevy; newy<prevy?y++:y--) {
            map->tiles[prevx+MAPWIDTH*y].blocking = false;
            map->tiles[prevx+MAPWIDTH*y].seeThrough = true;
        }
    }
    #endif
}

/* Algorithm overview:

Make a bunch of rectangles and connect every subsequent one by a path.
First is the player's spawn and last is the exit.

Addendum: if a room is too close to the previously created one, drop it and try again.
*/

void MapGenerator::makeRect(SDL_Rect r, bool w, Tile* tiles) {
    for (int x = r.x; x != r.x+r.w; x++)
    for (int y = r.y; y != r.y+r.h; y++) {
        tiles[x+MAPWIDTH*y].blocking = w;
        tiles[x+MAPWIDTH*y].seeThrough = !w;
    }
}

#define MAXMOBS 15

void MapGenerator::populateMap(Map* map) {
    Pos positions [MAPWIDTH*MAPHEIGHT]; //Will hold available positions to place mobs
    int posnum = 0; //Number of such

    for (int x = 0; x != MAPWIDTH; x++)
    for (int y = 0; y != MAPHEIGHT; y++) {
        if (!map->isWall(x, y) &&
            !map->isWall(x, y+1) &&
            !map->isWall(x, y-1) &&
            !map->isWall(x+1, y) &&
            !map->isWall(x-1, y) &&
            posDiff({x, y}, map->player->getPos()) >= 5) {
            positions[posnum++] = {x, y};
        }
    }

    TargetedAttack* a = new TargetedAttack(10, 0, 0, 10);

    for (int n = 0; n != MAXMOBS; n++) {
        int pos = std::rand() % posnum;
        Mob* m = new Mob(positions[pos].x, positions[pos].y,
                         SDL_Rect{0, 8, 8, 8}, engine.texture, "Somefin");
        m->ai = new BasicAi(120);
        m->destructible = new MobDestructible(10, SDL_Rect{0, 30, 16, 16}, 6);
        m->attack = a;
        m->inventory = new MobInventory;
        m->inventory->addItem(new HpPotion("Potfin", 30));
        map->mobs2.push_back(m);

        positions[pos] = positions[--posnum];
    }
}

