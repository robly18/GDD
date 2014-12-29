#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "map.hpp"
#include <random>

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
