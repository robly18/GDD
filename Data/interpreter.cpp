#include "interpreter.hpp"


Mobdef Database::getMobDef(std::string name) const {
    return mobdefs.find(name)->second;
}

Mob *Database::makeMob(Pos p, Mobdef def) const {
    Mob* m = new Mob(p.x, p.y,
                        SDL_Rect{def.texture.x * 16, def.texture.y * 16, 16, 16},
                        engine.texture, def.name);
    m->sght = def.sght;
    m->ai = new BasicAi(def.swiftness);
    m->destructible = new MobDestructible(def.hp, SDL_Rect{def.deadtexture.x * 16,
                                                            def.deadtexture.y * 16, 16, 16}, def.xp);
    m->attack = new TargetedAttack(def.atk, 0, 0, 10, 0, 0, 0, true, false);
    m->inventory = new MobInventory;
    m->inventory->addItem(new HpPotion("HPPot", 40));

    if (def.def) {
        m->destructible->armor = new Armor("SKIN", def.def, 0);
    }

    return m;
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
    std::cout<<"MakeDef called with arguments:\nID: "<<id<<"\nString: "<<str<<"\n\n";
    if (strcmp(id, "Mob") == 0) {
        Mobdef m = makeMobDef(str);
        d->mobdefs[m.name] = m;
    } else {
        assert(false);
    }
}

Mobdef Interpreter::makeMobDef(const std::string str) {
    char propertyname [MAXPROPERTYNAMECHARS] = {}; //this is a mess wee
    char property [MAXPROPERTYCHARS] = {};
    int i = 0;
    bool checkingproperty = false;

    bool comment = false;
    bool inquotes = false;

    Mobdef returnvalue;

    for (char c : str) {
        if (!inquotes) {
            if (c == '#') comment = true;
        }
        if (c == '\n') comment = false;
        if (!comment) {
            if (isalnum(c) || c == ',' || inquotes) {
                if (c == '"') {
                    inquotes = false;
                    continue;
                }
                (checkingproperty ? property : propertyname)[i++] = c; //oh my god what did i do
            } else if (c == ':') {
                checkingproperty = true;
                propertyname[i] = '\0';
                i = 0;
                std::cout<<"Propertyname: "<<propertyname<<";\n";
            } else if (c == ';') {
                checkingproperty = false;
                property[i] = '\0';
                i = 0;
                parseProperty(propertyname, property, returnvalue);
            }
            if (c == '"') inquotes = true;
        }
    }

    return returnvalue;
    //ugh todo finish
}

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

