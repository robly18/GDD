#include "map.hpp"

bool Map::canMoveTo(int x, int y) {
    if (isWall(x, y)) return false;
    for (Mob** m = mobs2.begin(); m != mobs2.end(); m++) {
        if ((*m)->x == x && (*m)->y == y) {
            return false;
        }
    }
    return true;
}

FloorInventory* Map::getInvAt(int x, int y) {
    for (FloorInventory** inv = items.begin(); inv != items.end(); inv++) {
        if ((*inv)->x == x && (*inv)->y == y) {
            return *inv;
        }
    }
    return NULL;
}

FloorInventory* Map::addInvTo(Inventory* inv, int x, int y) {
    FloorInventory* finv = getInvAt(x, y);
    if (!finv) {
        finv = new FloorInventory(engine.texture, x,y);
        items.push(finv);
    }
    for (int n = 0; n != inv->itemNo(); n++) {
        finv->items.addItem(inv->itemAtSlot(n));
    }
    return finv;
}

FloorInventory* Map::dropItem(Item* i, int x, int y) {
    FloorInventory* finv = getInvAt(x, y);
    if (!finv) {
        finv = new FloorInventory(engine.texture, x,y);
        items.push(finv);
    }
    finv->items.addItem(i);
    return finv;
}

FloorInventory* Map::removeInvAt(int x, int y) {
    for (FloorInventory** inv = items.begin(); inv != items.end(); inv++) {
        if ((*inv)->x == x && (*inv)->y == y) {
            items.remove(inv);
            return *inv;
        }
    }
    return NULL;
}

SDL_Surface* Map::render(int mousex, int mousey) {
    static SDL_Rect floor = {0,16,16,16};
    static SDL_Rect wall = {0,32,16,16};
    for (int x = 0; x != MAPWIDTH/4; x++)
    for (int y = 0; y != MAPHEIGHT/4; y++) {
        SDL_Rect r = SDL_Rect{x*16, y*16, 16, 16};
        SDL_BlitSurface(engine.texture, !isWall(x,y) ? &floor : &wall,
                        surface, &r);
    }
    for (Mob** m = mobs1.begin(); m != mobs1.end(); m++)
        SDL_BlitSurface((*m)->a->texture, &(*m)->a->r, surface, &(*m)->a->p);
    for (FloorInventory** m = items.begin(); m != items.end(); m++)
        SDL_BlitSurface((*m)->a->texture, &(*m)->a->r, surface, &(*m)->a->p);
    if (player->attack) {
        for (int x = 0; x != 16; x++)
        for (int y = 0; y != 10; y++) {
            SDL_Rect r = {x*16, y*16, 16, 16};
            SDL_FillRect(highlightsurface, &r, player->attack->highlightColor(
                                            player,
                                            x, y,
                                            (mousex-2)/16,
                                            (mousey-2)/16));
        }
        SDL_BlitSurface(highlightsurface, NULL, surface, NULL);
    }
    for (Mob** m = mobs2.begin(); m != mobs2.end(); m++)
        SDL_BlitSurface((*m)->a->texture, &(*m)->a->r, surface, &(*m)->a->p);
    return surface;
}

