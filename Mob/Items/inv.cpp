#include "inv.hpp"

bool PlayerInventory::addItem(Item* i) {
    if (filled != 0) {
        if (30 - filled + 1 <= i->size) {
            return false;
        }
    }
    items[filled] = i;
    filled += i->size;
    return true;
}

Item* PlayerInventory::removeItem(Item* i) {
    std::cout<<"OKAY THIS IS 8EING CALLED AND I SWEAR I'M GONNA KILL SOME8ODY\n";

    for (int n = 0; n != 30; n++) {
        if (getItem(n) == i) {
            removeItem(n);
            return i;
        }
    }
    return NULL;
}

Item* PlayerInventory::removeItem(int index) {
    Item* retitem = items[index];
    int placeindex = index;
    int getindex = index + retitem->size;
    while (placeindex <= filled) {
        if (getindex >= 30) {
            items[placeindex] = nullptr;
            goto end;
        }
        items[placeindex] = items[getindex];
        if (items[placeindex] == nullptr) break;
        items[getindex] = nullptr;
        getindex += items[placeindex]->size;
        placeindex += items[placeindex]->size;
    }

    items[getindex] = NULL;

    end:
    filled -= retitem->size;
    return retitem;
}

Item* PlayerInventory::getItem(int index) {
    if (*items == NULL) return NULL;
    int i = index;
    while (i > 0) {
        if (items[i] != NULL) {
            break;
        }
        i--;
    }
    if (index - i < items[i]->size) return items[i];
    else return NULL;
}

Item* PlayerInventory::itemAtSlot(int i) {
    return items[i];
}

int PlayerInventory::itemNo() {
    return 30;
}

bool MobInventory::addItem(Item* item) {
    items.push(item);
    return true;
}

Item* MobInventory::removeItem(Item* i) {
    for (Item** j = items.begin(); j != items.end(); j++) {
        if (*j == i) {
            return items.remove(j);
        }
    }
    return NULL;
}

Item* MobInventory::removeItem(int i) {
    return items.remove(items.begin() + i);
}

Item* MobInventory::itemAtSlot(int i) {
    if (i >= itemNo()) return NULL;
    return *(items.begin() + i);
}

/***/

void FloorInventory::checkExistance() {
    if (!items.itemNo()) {
        engine.map->removeInvAt(x, y);
        delete this;
    }
}


