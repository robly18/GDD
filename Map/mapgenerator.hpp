#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "map.hpp"
#include "../Misc/posstruct.hpp"
#include "../Data/interpreter.hpp"
#include <random>




class Map;
struct Tile;
struct TileProperties;
class Database;

class MapGenerator {
public:
    MapGenerator();

    void                    generateMap(Map*, const Database&,
                                        const TileProperties *wall,
                                        const TileProperties *nowall);
    void                    populateMap(Map*, const Database&);

private:
    void                    makeRect(const SDL_Rect, const TileProperties*, Tile*);
};




#endif
