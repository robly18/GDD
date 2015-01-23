#include "pathfinder.hpp"

Pathfinder::Pathfinder(int w, int h) :
    w(w), h(h) {
    tiledata = new bool [w*h];
}

struct PosNode {
    Pos pos;
    PosNode* prev;
    int cost;
};

void Pathfinder::setPosData(Pos pos, bool wall) {
    tiledata[pos.x + pos.y * w] = wall;
}
bool Pathfinder::canGoThrough(Pos pos) {
    return tiledata[pos.x + pos.y*w];
}

#define GETCOST(n, destination) ((n).cost + posDiff((n).pos, destination))

#include <iostream>

void Pathfinder::computePath(const Pos start, const Pos end,
                             std::list<Pos> &path) {
    std::list<PosNode> nodes;
    std::list<PosNode> availablenodes;
    availablenodes.push_back(PosNode{start, nullptr, 0});

    static Pos dirs [4] = {{1,0}, {0,1}, {-1, 0}, {0, -1}};

    //std::cout<<"Starting path computation\n";

    while (true) {
        //std::cout<<"Iterating...\n";
        std::list<PosNode>::iterator nextnode = availablenodes.begin();
        //std::cout<<"Picking node...\n";
        int mincost = GETCOST(*nextnode, end);

        //This selects the minimum possible cost node
        for (std::list<PosNode>::iterator p = std::next(nextnode);
                    p != availablenodes.end(); p++) {
            int tempcost = GETCOST(*p, end);
            if (tempcost < mincost) {
                nextnode = p;
                mincost = tempcost;
            }
        }

        nodes.push_front(*nextnode);
        availablenodes.erase(nextnode);

        nextnode = nodes.begin();

        if (nextnode->pos.x == end.x &&
            nextnode->pos.y == end.y) {
            break;
        }

        //std::cout<<nextnode->pos.x<<";"<<end.x<<";"<<nextnode->pos.y<<";"<<end.y<<"\n";
        for (Pos* adding = dirs; adding != dirs + 4; adding++) {
            //std::cout<<"Itr..\n";
            Pos npos = addPos(nextnode->pos, *adding);
            if (canGoThrough(npos)) {
                //std::cout<<"Cangothru...\n";
                availablenodes.push_front(PosNode{
                                          npos,
                                          &*nextnode, nextnode->cost+1});
            }
            //std::cout<<"Dn.\n";
        }

        //std::cout<<"Done iterating.\n\n";
    }
    //std::cout<<"Done.\n\n";

    for (PosNode *p = nodes.begin()->prev; p->prev != nullptr; p = p->prev) {
        path.push_front(p->pos);
    }
    //std::cout<<"Donedone.\n\n";
}
