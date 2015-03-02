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

#define SEEALLMOBS
#define SEEALLTILES


class Inventory;
class FloorInventory;
class Item;
class Player;
class MapGenerator;
class FovComputer;
class Pathfinder;

struct Tile {
    bool blocking = false;
    bool seeThrough = true;

    bool isSeen = false;
    bool hasBeenSeen =
    #ifdef SEEALLTILES
        true;
    #else
        false;
    #endif
};

#define INBOUNDS(x, y) (0<=x && x<MAPWIDTH && 0<=y && y<MAPHEIGHT)
#define POSINBOUNDS(p) INBOUNDS(p.x, p.y)

class Map {
public:
    Map();

    Tile                        tiles[MAPWIDTH*MAPHEIGHT];
    bool                        isWall(int x, int y) {return x < 0 || x >= MAPWIDTH ||
                                                                y < 0 || y >= MAPHEIGHT ||
                                                                tiles[x+y*MAPWIDTH].blocking;}
    void                        setWall(int x, int y, bool w) {tiles[x+y*MAPWIDTH].blocking = w;}

    bool                        canSeeThrough(int x, int y) const;
    bool                        hasBeenSeen(int x, int y) const;
    bool                        isBeingSeen(int x, int y) const;

    FovComputer                 *fovcomputer;
    Pathfinder                  *pathfinder;

    bool                        canMoveTo(int, int);
    bool                        canMoveTo(Pos);

    std::list<Mob*>             mobs1;
    std::list<FloorInventory*>  items;
    std::list<Mob*>             mobs2; //3 layers. Bottommost-mobs1, middlemost-items, topmost-mobs2 (collidable)

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

    void                        generateMap();
    void                        updateFovData();

private:
    void                        makePlayer();
    MapGenerator                *generator;

    SDL_Rect                    renderMobActor(Actor*);
};

#endif
