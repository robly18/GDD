#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "map.hpp"
#include "../Misc/posstruct.hpp"
#include <random>



#define MAXCLUBGUYS 10
#define MAXROGUEIMPS 30

class Map;
struct Tile;

class MapGenerator {
public:
    void            generateMap(Map*);
    void            populateMap(Map*);

private:
    void            makeRect(SDL_Rect, bool, Tile*);
};




#endif
