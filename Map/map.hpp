#ifndef MAP_H
#define MAP_H

#define SCREENTILEW 16
#define SCREENTILEH 10
#define MAPWIDTH 64
#define MAPHEIGHT 40

#include "..\Main\main.hpp"
#include "mapgenerator.hpp"
#include "fovcomputer.hpp"

class Inventory;
class FloorInventory;
class Item;
class Player;
class MapGenerator;
class FovComputer;

struct Tile {
    bool blocking = false;
    bool seeThrough = true;

    bool isSeen = false;
    bool hasBeenSeen = false;
};

class Map {
public:
    Map();

    Tile                        tiles[MAPWIDTH*MAPHEIGHT];
    bool                        isWall(int x, int y) {return x < 0 || x >= MAPWIDTH ||
                                                                y < 0 || y >= MAPHEIGHT ||
                                                                tiles[x+y*MAPWIDTH].blocking;}
    void                        setWall(int x, int y, bool w) {tiles[x+y*MAPWIDTH].blocking = w;}

    bool                        hasBeenSeen(int x, int y) {return tiles[x+y*MAPWIDTH].hasBeenSeen;}

    FovComputer                 *fovcomputer;

    bool                        canMoveTo(int, int);

    gdd::List<Mob*>             mobs1;
    gdd::List<FloorInventory*>  items;
    gdd::List<Mob*>             mobs2; //3 layers. Bottommost-mobs1, middlemost-items, topmost-mobs2 (collidable)

    Player*                     player = NULL;
    int                         camerax = 0, cameray = 0;
    void                        resetCamera();

    FloorInventory*             getInvAt(int, int);
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
