#ifndef MAP_H
#define MAP_H

#define SCREENTILEW 16
#define SCREENTILEH 10
#define MAPWIDTH 64
#define MAPHEIGHT 40

#include "..\Main\main.hpp"
#include "mapgenerator.hpp"
#include "fovcomputer.hpp"
#include <list>
#include <memory>


class Inventory;
class FloorInventory;
class Item;
class Player;
class MapGenerator;
class FovComputer;
class Pathfinder;

struct TileProperties { //what happens when i read shenanigans on design patterns
        TileProperties(bool b, bool s, int f = 0) :
            blocking(b), seeThrough(s), flags(f) {};
    bool blocking; //friday 13th march 2015
    bool seeThrough;
    int flags;
};//todo implement exit shenanigans
//maybe turn blocking and seethrough into flags?

namespace TileProperty {
    enum {
        EXIT = 0x01,
    };
}

struct Tile {
    const TileProperties    *properties;

    bool                    isSeen = false;
    bool                    hasBeenSeen =
                            #ifdef SEEALLTILES
                                true;
                            #else
                                false;
                            #endif
};

#define INBOUNDS(x, y) (0<=x && x<MAPWIDTH && 0<=y && y<MAPHEIGHT)
#define POSINBOUNDS(p) INBOUNDS(p.x, p.y)

#define POSTOINDEX(p) (p.x + p.y * MAPWIDTH)

class Map {
    friend class MapGenerator;
public:
    Map();
    void                        restartMap();

    bool                        isWall(int x, int y) {return x < 0 || x >= MAPWIDTH ||
                                                                y < 0 || y >= MAPHEIGHT ||
                                                                tiles[x+y*MAPWIDTH].properties->blocking;}

    bool                        canSeeThrough(int x, int y) const;
    bool                        hasBeenSeen(int x, int y) const;
    bool                        isBeingSeen(int x, int y) const;

    bool                        hasFlag(int x, int y, int flag) const;

    FovComputer                 *fovcomputer;
    Pathfinder                  *pathfinder;

    bool                        canMoveTo(int, int);
    bool                        canMoveTo(Pos);

    std::list<Mob*>             mobs1;
    std::list<FloorInventory*>  items;
    std::list<Mob*>             mobs2; //3 layers. Bottommost-mobs1, middlemost-items, topmost-mobs2 (collidable)

    void                        spawnExit(Pos);

    std::list<Mob*>::iterator   killMob(std::list<Mob*>::iterator);

    Player*                     player = nullptr;
    int                         camerax = 0, cameray = 0;
    void                        resetCamera();

    void                        inspect(Pos) const;

    FloorInventory*             getInvAt(int, int) const;
    FloorInventory*             addInvTo(Inventory*, int, int);
    FloorInventory*             dropItem(Item*, int, int);

    FloorInventory*             removeInvAt(int, int);

    SDL_Surface*                surface;
    SDL_Surface*                highlightsurface;

    SDL_Surface*                render(int mousex, int mousey);
    SDL_Surface*                mapview();

    void                        checkMapData();

    void                        generateMap();
    void                        updateFovData();

    bool                        attack(Pos);

private:
    Tile                        tiles[MAPWIDTH*MAPHEIGHT];


    void                        makePlayer();
    MapGenerator                *generator;

    SDL_Rect                    renderMobActor(Actor*);

    int                         seeabletiles;
    int                         seentiles;



    const TileProperties    *nowall,
                            *wall,
                            *exit;
};

#endif
