#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "map.hpp"
#include "../Misc/posstruct.hpp"
#include <random>




class Map;
struct Tile;
struct TileProperties;

class MapGenerator {
public:
    MapGenerator();

    void                    generateMap(Map*,
                                        const TileProperties *wall,
                                        const TileProperties *nowall);
    void                    populateMap(Map*);

private:
    void                    makeRect(const SDL_Rect, const TileProperties*, Tile*);
};




#endif
