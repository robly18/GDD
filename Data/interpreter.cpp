#include "interpreter.hpp"


const Mobdef &Database::getMobDef(std::string name) const {
    return mobdefs.at(name);
}

Mob *Database::makeMob(Pos p, Mobdef def) const {
    Mob* m = new Mob(p.x, p.y,
                        SDL_Rect{def.texture.x * 16, def.texture.y * 16, 16, 16},
                        engine.texture, def.name);
    m->sght = def.sght;
    m->ai = new BasicAi(def.swiftness);
    m->destructible = new MobDestructible(def.hp, SDL_Rect{def.deadtexture.x * 16,
                                                            def.deadtexture.y * 16, 16, 16}, def.xp);
    m->attack = new TargetedAttack(def.atk, 0, "ayy", NOICON, 0, 10, 0, 0, 0, true, false);
    m->inventory = new MobInventory;
    m->inventory->addItem(new HpPotion("HPPot", 40));

    if (def.def) {
        m->destructible->armor = new Armor("SKIN", def.def, 0);
    }

    return m;
}

const Itemdef &Database::getItemDef(const std::string name) const {
    return itemdefs.at(name);
}

Item *Database::makeItem(Itemdef def) {

    Item *i;

    if (def.itemtype == Itemtype::WEAPON) {
        i = new Sword(def.name, 10, def.data.wep.maxmana);
        for (int j = 0; j != 3; j++) {
            ((Weapon*)i)->attacks[j] = getAtk(*def.data.wep.atks[j]);
            ((Weapon*)i)->defenses[j] = getAtk(*def.data.wep.atks[j+3]);
        }
    } else if (def.itemtype == Itemtype::USE) {
        i = new HpPotion(def.name, 10);
    } else {
        assert(false);
    }
    i->color = def.color;
    return i;
}

const Atkdef &Database::getAtkDef(std::string name) const {
    return atkdefs.at(name);
}

const Attack *Database::getAtk(Atkdef def) {

    std::unordered_map<std::string, const Attack*>::const_iterator atk = atksingletons.find(def.name);
    if (atk != atksingletons.end()) return atk->second;
    std::cout<<"bluhh";
    const Attack *a;

    if (def.atktype == Atkdef::TARGETED) {
        a = new TargetedAttack(100, 3, def.name, NOICON, 0, 2);
    } else if (def.atktype == Atkdef::SELFBUFF) {
        a = new SelfBuff(10, def.name, NOICON, new FixedHpRegen(10, "bluh", 3));
    }

    atksingletons[def.name] = a;
    return a;
}


bool Interpreter::open(std::string filename) {
    file.open(filename);
    std::cout<<"Opened "<<filename<<"\n";
    return file.is_open();
}

void Interpreter::close() {
    file.close();
}


void Interpreter::parseToDatabase(Database &d) {
    std::cout<<"Beginning parsing:\n";

    Interpreter::d = &d;

    while (file.good()) {
        std::ostringstream input;
        char c;
        do {
            file.get(c);
            input<<c;
        } while (c != '}' && file.good());
        if (file.good()) interpretDef(input.str());
    }
}

void Interpreter::interpretDef(const std::string str) {
    bool comment = false;

    char id[MAXIDCHARS] = {};
    int idlen = 0;

    unsigned int i;
    for (i = 0; i != str.size(); i++) {
        if (str[i] == '#') comment = true;
        if (str[i] == '\n') comment = false;
        if (!comment) {
            if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '{') {
                id[idlen++] = str[i];
                assert(idlen < MAXIDCHARS);
            } else if (idlen) {
                makeDef(id, str.substr(i));
                return;
            }
        }
    }
}

void Interpreter::makeDef(const char* id, const std::string str) {
    if (strcmp(id, "Mob") == 0) {
        Mobdef m = makeDef<Mobdef>(str);
        d->mobdefs[m.name] = m;
    } else if (strcmp(id, "Item") == 0) {
        Itemdef i = makeDef<Itemdef>(str);
        d->itemdefs[i.name] = i;
    } else if (strcmp(id, "Attack") == 0) {
        Atkdef i = makeDef<Atkdef>(str);
        d->atkdefs[i.name] = i;
    } else {
        assert(false);
    }
}

template <class C>
C Interpreter::makeDef(const std::string str) {
    char propertyname [MAXPROPERTYNAMECHARS] = {}; //this is a mess wee
    char property [MAXPROPERTYCHARS] = {};
    int i = 0;
    bool checkingproperty = false;

    bool comment = false;
    bool inquotes = false;

    C returnvalue;

    for (char c : str) {
        if (!inquotes) {
            if (c == '#') comment = true;
        }
        if (c == '\n') comment = false;
        if (!comment) {
            if (inquotes) goto addtoprop; //You can't stop me!
            if (c == ':') {
                checkingproperty = true;
                propertyname[i] = '\0';
                i = 0;
            } else if (c == ';') {
                checkingproperty = false;
                property[i] = '\0';
                i = 0;
                parseProperty(propertyname, property, returnvalue);
            } else if (!std::isspace(c) && c != '{') {
                addtoprop:
                if (c == '"') {
                    inquotes = false;
                    continue;
                }
                (checkingproperty ? property : propertyname)[i++] = c; //oh my god what did i do
            }
            if (c == '"') inquotes = true;
        }
    }

    return returnvalue;
    //ugh todo finish
}

/****/

void Interpreter::parseProperty(char* propertyname, char* property, Mobdef &def) {


    ACTONPROPERTY(name, std::string)
    ACTONPROPERTY(hp, atoi)
    ACTONPROPERTY(texture, parseCoordinates)
    ACTONPROPERTY(deadtexture, parseCoordinates)
    ACTONPROPERTY(swiftness, atoi)
    ACTONPROPERTY(atk, atoi)
    ACTONPROPERTY(def, atoi)
    ACTONPROPERTY(sght, atoi)
    ACTONPROPERTY(xp, atoi)
    {
        std::cout<<propertyname;
        assert(false);
    }
}

void Interpreter::parseProperty(char *propertyname, char *property, Itemdef &def) {

    thisdef = &def;


    ACTONPROPERTY(name, std::string)
    ACTONPROPERTY(color, charsToColors)
    ACTONPROPERTY(itemtype, itemtypeToInt)

    ACTONPROPERTY(data.wep.maxmana, atoi)
    ACTONPROPERTY(data.wep.atks, parseAttacks)
    {
        std::cout<<"\n----\nError: This property doesn't exist!\n-"
                    <<propertyname
                    <<"\n----\n";
        assert(false);
    }
}

void Interpreter::parseProperty(char *propertyname, char *property, Atkdef &def) {


    ACTONPROPERTY(name, std::string)
    ACTONPROPERTY(atktype, atktypeToInt)
    {
        std::cout<<"\n----\nError: This property doesn't exist!\n-"
                    <<propertyname
                    <<"\n----\n";
        assert(false);
    }
}

/****/

Pos Interpreter::parseCoordinates(char *coords) {
    Pos p;

    char *c;

    p.x = 0;
    for (c = coords; *c != ','; c++) {
        if (isdigit(*c)) {
            p.x *= 10;
            p.x += *c - '0';
        }
    }
    p.y = 0;
    for (; *c != '\0'; c++) {
        if (isdigit(*c)) {
            p.y *= 10;
            p.y += *c - '0';
        }
    }

    return p;
}

int hexchartoint(char c) {
    if (isdigit(c)) return c-'0';
    if ('a' <= c && c <= 'f') return 10 + c-'a';
    if ('A' <= c && c <= 'F') return 10 + c-'A';
    assert(false);
}

Uint32 Interpreter::charsToColors (char *hexcol) { //RGB
    char *c;

    Uint32 r = 0;

    for (c = hexcol; *c != '\0'; c++) {
        if (isalnum(*c)) {
            r *= 16;
            r += hexchartoint(*c);
        }
    }

    r = ((r%(1<<8))<<16) + (((r>>8)%(1<<8))<<8) + (r>>16); //reversing byte order from BGR to RGB
    //i'm so sorry
    return r;
}

int Interpreter::itemtypeToInt(char *type) {
    if (strcmp(type, "WEAPON") == 0) {
        return Itemtype::WEAPON;
    } else if (strcmp(type, "USE") == 0) {
        return Itemtype::USE;
    }
    assert(false);
}

int Interpreter::atktypeToInt(char *type) {
    if (strcmp(type, "TARGETED") == 0) {
        return Atkdef::TARGETED;
    } else if (strcmp(type, "SELFBUFF") == 0) {
        return Atkdef::SELFBUFF;
    } else if (strcmp(type, "HITTHRU") == 0) {
        return Atkdef::HITTHRU;
    }else if (strcmp(type, "BASHSWP") == 0) {
        return Atkdef::BASHSWP;
    }
    assert(false);
}

atkdefarr Interpreter::parseAttacks(char *atks) {
    char atkname[8];
    atkdefarr arr = thisdef->data.wep.atks;
    for (char *c = atks; *c != '\0'; c++) {
        int atk = *c++ - '1';
        assert(*c++ == '-' && atk >= 0 && atk <= 5);

        int i;
        for (i = 0; *c != '|' && *c != '\0'; c++, i++) {
            atkname[i] = *c;
        }
        atkname[i] = '\0';
        arr[atk] = &d->getAtkDef(std::string(atkname));
    }
    return arr;
}

