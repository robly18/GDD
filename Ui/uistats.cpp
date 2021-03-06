#include "uistats.hpp"



/***/

UiPart::UiPart(SDL_Surface* s) : surface(s) {
}

/***/

UiButtons::UiButtons(SDL_Surface* s) :
    UiPart(s) {
    invbutton = new Actor(surface, SDL_Rect{16, 8, 8, 8}, SDL_Rect{288, 148, 54, 14});
    invtext = new FontStr(engine.font, "Inv");
    hpbar = new NumBar(SDL_Rect{2, 164, 96, 10}, 0x0000FF, 0x000077,
                       engine.font,
                        engine.map->player->destructible->maxHp,
                        &engine.map->player->destructible->hp, "HP");
    mpbar = new NumBar(SDL_Rect{2, 164+12, 96, 10}, 0xFF0000, 0x770000,
                       engine.font,
                        engine.map->player->getMaxMp(),
                        engine.map->player->weapon ?
                        &engine.map->player->weapon->mana : NULL,
                        "MP");
}

SDL_Surface* UiButtons::render(bool clicked, int hx, int hy) {
    hpbar->render(surface);
    hpbar->renderName(surface);

    mpbar->render(surface);
    mpbar->renderName(surface);

    SDL_Rect p = invbutton->p;

    SDL_FillRect(surface, &p, clicked && isInRect(p, hx, hy) ? 0x885512 : 0xDDAA22);
    p.x += 5;
    p.y += 3;
    SDL_BlitSurface(engine.texture, &invbutton->r, surface, &p);
    p.x += 8 + 2;
    invtext->render(surface, engine.font->font, p.x, p.y);
    return surface;
}

void UiButtons::checkClick(int mb, int hx, int hy) {}

void UiButtons::checkUnclick(int hx, int hy, int x, int y) {
    if (isInRect(invbutton->p, x, y)) {
        engine.enginestate = engine.enginestate == engine.inv ?
                                engine.running : engine.inv;
        engine.ui->inv->page = 0;
        engine.ui->inv->selecteditem = NULL;
        engine.ui->inv->updateInventory();
    }
}

void UiButtons::updatePlayerData() {
    mpbar->maxval = engine.map->player->getMaxMp();
    mpbar->valptr = engine.map->player->weapon ?
                    &engine.map->player->weapon->mana : NULL;
}

/***/

UiDashboard::UiDashboard(SDL_Surface* s) :
    UiPart(s) {
    for (int x = 0; x != 3; x++)
    for (int y = 0; y != 2; y++) {
        atkbuttons[x+3*y] = new Actor(engine.texture, SDL_Rect{0,0,1,1},
                                      SDL_Rect{101+68*x, 164+12*y, 67, 10});
        atknames[x+3*y] = new FontStr(engine.font, "");
    }
}

SDL_Surface* UiDashboard::render(bool clicked, int hx, int hy) {
    for (int i = 0; i != 6; i++) {
        SDL_Rect p = atkbuttons[i]->p;
        SDL_Rect iconp = {p.x + 1, p.y + 1, 8, 8};

        SDL_FillRect(surface, &p,
                     clicked && isInRect(p, hx, hy) ? 0x885512 : 0xDDAA22);
        atknames[i]->render(surface, engine.font->font, p.x+1+8+1, p.y+1);
        SDL_BlitSurface(engine.texture, atkicons+i, surface, &iconp);
    }
    return surface;
}

void UiDashboard::checkClick(int mb, int hx, int hy) {}

void UiDashboard::checkUnclick(int hx, int hy, int x, int y) {
    for (Actor** a = atkbuttons; a != atkbuttons+6; a++) {
        if (isInRect((*a)->p, hx, hy)) {
            if (isInRect((*a)->p, x, y))
                switch ((int) (a - atkbuttons)) {
                case AtkButtons::TARGETED:
                    if (!engine.map->player->weapon) {
                        engine.map->player->attack = engine.map->player->defaultattack;
                    }
                case AtkButtons::AOE:
                case AtkButtons::QUIRKATK:
                    if (engine.map->player->weapon) {
                        engine.map->player->weapon->attacks[a-atkbuttons-3]->select(engine.map->player);
                    }
                    break;
                case AtkButtons::BLOCK:
                case AtkButtons::HEAL:
                case AtkButtons::QUIRKDEF:
                    if (engine.map->player->weapon) {
                        engine.map->player->weapon->defenses[a-atkbuttons]->select(engine.map->player);
                    }
                    break;
                default:
                    break;
            }
            return;
        }
    }
}


void UiDashboard::refreshButtons() {
    if (engine.map->player->weapon) {
        for (int x = 0; x != 3; x++) {
            atknames[x]->setText(engine.font, engine.map->player->weapon->defenses[x]->name);
            atkicons[x] = engine.map->player->weapon->defenses[x]->icon;
            atknames[x+3]->setText(engine.font, engine.map->player->weapon->attacks[x]->name);
            atkicons[x+3] = engine.map->player->weapon->attacks[x]->icon;
        }
    }
}

/***/

UiXP::UiXP(SDL_Surface* s) :
    UiPart(s) {
    holder = engine.map->player->xpholder;
}

SDL_Surface* UiXP::render(bool clicked, int hx, int hy) {
    for (XpBar** b = holder->bars; b != holder->bars+10; b++) {
        (*b) ->render(surface);
    }
    return surface;
}

void UiXP::checkClick(int mb, int hx, int hy) {
    for (int i = 0; i != 10; i++) {
        if (isInRect(holder->bars[i]->bar->barmax, hx, hy)) {
            if (mb == SDL_BUTTON_LEFT) {
                holder->setFocus(i);
            } else if (mb == SDL_BUTTON_RIGHT) {
                holder->log(*engine.ui->log, i);
            }
            break;
        }
    }
}

void UiXP::checkUnclick(int hx, int hy, int x, int y) {}

/***/

UiInv::UiInv(SDL_Surface* s) :
    UiPart(s) {
    for (int x = 0; x != 3; x++)
    for (int y = 0; y != 10; y++) {
        plrinv[y+10*x] = new InvActor {
                            new Actor(engine.texture, SDL_Rect{0,0,1,1},
                                      SDL_Rect{6 + 86*x, 4 + 16*y, 80, 14}),
                            new FontStr(engine.font, "Nothing"),
                            NOITEMCOL};
    }
    for (int y = 0; y != FINVPGSIZE; y++) {
        floorinv[y] = new InvActor {
                            new Actor(engine.texture, SDL_Rect{0,0,1,1},
                                      SDL_Rect{261, 4 + 16*y, 108, 14}),
                            new FontStr(engine.font, "Nothing"),
                            NOITEMCOL};
    }
    prevpage = new Actor(engine.texture, SDL_Rect{0,16, 52, 14},
                         SDL_Rect{261, 132, 53, 14});
    prevname = new FontStr(engine.font, "<==");
    nextpage = new Actor(engine.texture, SDL_Rect{0,16, 52, 14},
                         SDL_Rect{261+53+2, 132, 53, 14});
    nextname = new FontStr(engine.font, "==>");

    std::string names[4] = {"USE","ARMOR","WEAPON","MISC"};
    for (int i = 0; i != 4; i++) {
        slots[i] = new Actor(engine.texture, SDL_Rect{32 + 8*(i&1), 16 + 8*(i&2)/2 ,8,8},
                             SDL_Rect {101+67*i, 164, 66, 22});
        slotnames[i] = new FontStr(engine.font, names[i]);
        itemnames[i] = new FontStr(engine.font, "-");
    }
}

SDL_Surface* UiInv::render(bool clicked, int hx, int hy) {
    for (int i = 0; i != 30; i++) {
        SDL_FillRect(surface, &plrinv[i]->a->p,
                     plrinv[i]->color -
                     (clicked && isInRect(plrinv[i]->a->p, hx, hy) ? 0x101010 : 0) -
                     (selecteditem &&
                      engine.map->player->inventory->getItem(i) == selecteditem ?
                            0x101010 : 0));
        plrinv[i]->str->render(surface, engine.font->font,
                               plrinv[i]->a->p.x + 2, plrinv[i]->a->p.y + 2);
    }
    for (InvActor** a = floorinv; a != floorinv+FINVPGSIZE; a++) {
        SDL_FillRect(surface, &(*a)->a->p,
                     (*a)->color -
                     (clicked && isInRect((*a)->a->p, hx, hy) ? 0x101010 : 0));
        (*a)->str->render(surface, engine.font->font, (*a)->a->p.x + 2, (*a)->a->p.y + 2);
    }
    {int i = 0;
        for (auto a : slots) {
            SDL_FillRect(surface, &a->p, 0xDD8822-
                         (clicked && isInRect(a->p, hx, hy) ? 0x101010 : 0));
            SDL_Rect r = {a->p.x + 2, a->p.y + 2, 16, 16};
            SDL_BlitSurface(engine.texture, &a->r, surface, &r);
            r.x += 8+2;
            slotnames[i]->render(surface, engine.font->font, r.x, r.y);
            r.x -= 8+2;
            r.y += 8+2;
            itemnames[i]->render(surface, engine.font->font, r.x, r.y);
            i++;
        }
    }
    SDL_Rect p = prevpage->p;
    SDL_FillRect(surface, &p, 0xDD8822-
                     (clicked && isInRect(p, hx, hy) ? 0x101010 : 0));
    p.x += 14;
    p.y += 3;
    prevname->render(surface, engine.font->font, p.x, p.y);

    p = nextpage->p;
    SDL_FillRect(surface, &p, 0xDD8822-
                     (clicked && isInRect(p, hx, hy) ? 0x101010 : 0));
    p.x += 15;
    p.y += 3;
    nextname->render(surface, engine.font->font, p.x, p.y);
    return surface;
}

void UiInv::checkClick(int mb, int hx, int hy) {
}

void UiInv::checkUnclick(int hx, int hy, int x, int y) {
    if (!checkPlrInvClick(hx, hy, x, y))
    if (!checkFloorInvClick(hx, hy, x, y))
        checkOtherButtonsClick(hx, hy, x, y);
    updateInventory();
    engine.ui->buttons->updatePlayerData();
}

/** Ui Inv Shenanigans **/

bool UiInv::checkPlrInvClick(int hx, int hy, int x, int y) {
    for (InvActor** a = plrinv; a != plrinv+30; a++) {
        if (isInRect((*a)->a->p, hx, hy)) {
            if(isInRect((*a)->a->p, x, y)) {
                Item* i = engine.map->player->inventory->itemAtSlot(a-plrinv);
                if (i == selecteditem) selecteditem = NULL;
                else selecteditem = i;
            }
            return true;
        }
    }
    return false;
}

bool UiInv::checkFloorInvClick(int hx, int hy, int x, int y) {
    for (int n = 0; n != FINVPGSIZE; n++) {
        if (isInRect(floorinv[n]->a->p, hx, hy)) {
            if (isInRect(floorinv[n]->a->p, x, y)) {
                if (selecteditem) {
                    engine.map->dropItem(selecteditem,
                                         engine.map->player->x,
                                         engine.map->player->y);
                    engine.map->player->inventory->removeItem(selecteditem);
                    selecteditem = NULL;
                } else if (finv) {
                    Item* i = finv->items.getItem(n+page*FINVPGSIZE);
                    if (i) {
                        if (engine.map->player->inventory->addItem(i)) {
                            finv->items.removeItem(n+page*FINVPGSIZE);
                            finv->checkExistance();
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool UiInv::checkOtherButtonsClick(int hx, int hy, int x, int y) {
    if (finv) {
        if (isInRect(prevpage->p, hx, hy) &&
            isInRect(prevpage->p, x, y)) {
            if (page != 0) page--;
            return true;
        }
        if (isInRect(nextpage->p, hx, hy) &&
            isInRect(nextpage->p, x, y)) {
            if ((page+1)*FINVPGSIZE <= (unsigned int) finv->items.itemNo()) page++;
            return true;
        }
    }
    if (selecteditem) {
        if (isInRect(slots[selecteditem->type]->p, hx, hy) &&
            isInRect(slots[selecteditem->type]->p, x, y)) {
            if (selecteditem->use(engine.map->player)) {
                engine.map->player->inventory->removeItem(selecteditem);
                engine.doTick(engine.USEDFLAG);
                engine.ui->dashboard->refreshButtons();
                if (selecteditem->type != Itemtype::USE) {
                    itemnames[selecteditem->type]->setText(engine.font, selecteditem->name);
                }
            }
        }
        selecteditem = NULL;
    } else {
        for (int i = 1; i != 4; i++) {
            if (isInRect(slots[i]->p, hx, hy) &&
                isInRect(slots[i]->p, x, y)) {
                switch (i) {
                case Itemtype::ARMOR:
                    if (engine.map->player->destructible->armor)
                        if (engine.map->player->destructible->armor->unequip(engine.map->player)) {
                            engine.map->player->destructible->armor = NULL;
                            itemnames[i]->setText(engine.font, "-");
                        }
                    break;
                case Itemtype::WEAPON:
                    if (engine.map->player->weapon) {
                        if (engine.map->player->weapon->unequip(engine.map->player)) {
                            engine.map->player->weapon = NULL;
                            engine.ui->dashboard->refreshButtons();
                            itemnames[i]->setText(engine.font, "-");
                        }
                    }
                    break;
                default: break;
                }
            }
        }
    }
    return false;
}

/**/

void UiInv::updateInventory() {
    Item* i;

    for (InvActor** a = plrinv; a != plrinv+30; a++) {
        i = engine.map->player->inventory->getItem(a-plrinv);
        (*a)->color = i ? i->color : NOITEMCOL;
        i = engine.map->player->inventory->itemAtSlot(a-plrinv);
        (*a)->str->setText(engine.font, i ? i->name : "-");
    }
    finv = engine.map->getInvAt(engine.map->player->x, engine.map->player->y);


    for (int n = 0; n != FINVPGSIZE; n++) {
        floorinv[n]->color = NOITEMCOL;
        floorinv[n]->str->setText(engine.font, "-");
    }
    if (finv) {
        if ((page+1)*FINVPGSIZE > (unsigned int) finv->items.itemNo()) {
            page = finv->items.itemNo()/FINVPGSIZE;
        }
        for (int n = 0; n != FINVPGSIZE; n++) {
            i = finv->items.itemAtSlot(n + page*FINVPGSIZE);
            if (!i) break;
            floorinv[n]->color = i->color;
            floorinv[n]->str->setText(engine.font, i->name);
        }
    }
}
