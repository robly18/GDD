#ifndef FOVCOMPUTER_H
#define FOVCOMPUTER_H

class FovComputer {
public:
    FovComputer(int, int);

    bool        *tiledata;
    int         w, h;

    bool        isInSight(int, int, int, int);
};


#endif
