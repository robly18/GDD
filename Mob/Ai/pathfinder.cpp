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

#define GETCOST(n, destination) ((n).cost + posDiff((n).pos, destination))

void Pathfinder::computePath(Pos start, Pos end, std::list<Pos> &path) {
    std::list<PosNode> nodes;
    std::list<PosNode> availablenodes;
    availablenodes.push_back(PosNode{start, nullptr, 0});

    static Pos dirs [4] = {{1,0}, {0,1}, {-1, 0}, {0, -1}};

    while (true) {
        std::list<PosNode>::iterator nextnode = availablenodes.begin();
        int mincost = GETCOST(*nextnode, end);

        //This selects the minimum possible cost node
        for (std::list<PosNode>::iterator p = nextnode++;
                    p != availablenodes.end(); p++) {
            int tempcost = GETCOST(*p, end);
            if (tempcost < mincost) {
                nextnode = p;
                mincost = tempcost;
            }
        }

        nodes.push_front(*nextnode);
        availablenodes.erase(nextnode);

        if (nextnode->pos.x == end.x &&
            nextnode->pos.y == end.y) {
            break;
        }

        for (Pos* adding = dirs; adding != dirs + 4; adding++) {
            availablenodes.push_front(PosNode{
                                      addPos(nextnode->pos, *adding),
                                      &*nextnode, nextnode->cost+1});
        }
    }

    for (PosNode *p = &*nodes.begin(); p != nullptr; p = p->prev) {
        path.push_front(p->pos);
    }
}
