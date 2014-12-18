#ifndef GDDLIST_H
#define GDDLIST_H

#include <stdlib.h>
#include <iostream>

namespace gdd {

template <class T>
class List {
public:
    List() : List(8) {}
    List(size_t size) : size(size) {
        first = (T*) malloc(size*sizeof(T*)); }
    ~List() {free(first);}

    T* begin() const {return first;}
    T* end() const {return first + arrayLength;}

    size_t length() const {return arrayLength;}

    void push (const T &t) { arrayLength++;
        if (arrayLength > size) {reallocate(size<<1);}
        first[arrayLength-1] = t;}

    T remove(T *p) {
        T ret = *p;
        for (T *ptr = p; ptr != first+size-1; ptr++) {
            *ptr = *(ptr+1);
        } arrayLength--;
        if (arrayLength < size>>1) {reallocate(size>>1);}
        return ret;
    }


private:
    T* first = NULL;
    size_t size;
    int arrayLength = 0;

    void reallocate(size_t s) {
        size = s; first = (T*) realloc(first, size*sizeof(T*));
    }
};


}

#endif
