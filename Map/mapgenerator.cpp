#include "mapgenerator.hpp"

MapGenerator::MapGenerator() {
}

#define NBLANKMAP

void MapGenerator::generateMap(Map* map, const Database &d,
                               const TileProperties* wall, const TileProperties* nowall) {

    #ifdef BLANKMAP
    makeRect(SDL_Rect{0, 0, MAPWIDTH, MAPHEIGHT}, nowall, map->tiles);
    #else

    makeRect(SDL_Rect{0, 0, MAPWIDTH, MAPHEIGHT}, wall, map->tiles);

    int prevx, prevy;
    int newx, newy;
    SDL_Rect r;

    r.w = (std::rand() % 6) + 4;
    r.h = (std::rand() % 6) + 4;
    r.x = std::rand() % (MAPWIDTH-r.w-2) + 1;
    r.y = std::rand() % (MAPHEIGHT-r.h-2) + 1;

    newx = r.x + r.w/2;
    newy = r.y + r.h/2;
    makeRect(r, nowall, map->tiles);

    map->player->setPos(newx, newy);

    for (int n = 0; n != ROOMNUM-1; n++) {
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
            goto newpos;
        }
        makeRect(r, nowall, map->tiles);

        makeRect({newx<prevx ? newx : prevx, prevy,
                  std::abs(newx-prevx)+1,  1},
                 nowall, map->tiles);
        makeRect({newx, newy<prevy ? newy : prevy,
                 1, std::abs(newy-prevy)+1},
                 nowall, map->tiles);
    }
    #endif
}

/* Algorithm overview:

Make a bunch of rectangles and connect every subsequent one by a path.
First is the player's spawn and last is the exit.

Addendum: if a room is too close to the previously created one, drop it and try again.
*/

void MapGenerator::makeRect(const SDL_Rect r, const TileProperties* t, Tile* tiles) {
    for (int x = r.x; x != r.x+r.w; x++)
    for (int y = r.y; y != r.y+r.h; y++) {
        tiles[x+MAPWIDTH*y].properties = t;
    }
}

void MapGenerator::populateMap(Map* map, const Database &d) {
    Pos positions [MAPWIDTH*MAPHEIGHT]; //Will hold available positions to place mobs
    int posnum = 0; //Number of such

    for (int x = 0; x != MAPWIDTH; x++)
    for (int y = 0; y != MAPHEIGHT; y++) {
        if (!map->isWall(x, y) &&
            posDiff({x, y}, map->player->getPos()) >= 5) {
            positions[posnum++] = {x, y};
        }
    }

    TargetedAttack* a = new TargetedAttack(7, 0, 0, 10, 0, 0, 0, true, false);

    Mobdef def = d.getMobDef("CLUBGUY");
    for (int n = 0; n != MAXCLUBGUYS; n++) {
        int pos = std::rand() % posnum;
        Mob* m = d.makeMob(positions[pos], def);
        map->mobs2.push_back(m);

        positions[pos] = positions[--posnum];
    }

    def = d.getMobDef("ROGUEIMP");

    for (int n = 0; n != MAXROGUEIMPS; n++) {
        int pos = std::rand() % posnum;
        Mob* m = d.makeMob(positions[pos], def);
        map->mobs2.push_back(m);

        positions[pos] = positions[--posnum];
    }
}

