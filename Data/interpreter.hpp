#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <array>

#include "../Main/main.hpp"


#define MAXIDCHARS 16
#define MAXPROPERTYNAMECHARS 16
#define MAXPROPERTYCHARS 256

struct Mobdef {
    std::string         name;
    int                 hp;
    Pos                 texture,
                        deadtexture;
    int                 swiftness = 0,
                        atk = 0,
                        def = 0,
                        sght = 0,
                        xp = 0;
};

struct Atkdef {
};

struct Itemdef {
    std::string         name;
    int                 itemtype;
    Uint32              color;

    union data {
        struct pot {
            int         heals; //subject to change in the future
        };
        struct wep {
            int         weapontype;
            std::array<Atkdef*, 6>
                        atks;
            int         atk,
                        maxmana;
        };
    };
};

class Database {
public:

    std::unordered_map<std::string, Mobdef>
                        mobdefs;

    Mobdef              getMobDef(std::string name) const;
    Mob                 *makeMob(Pos, Mobdef) const;


    std::unordered_map<std::string, Itemdef>
                        itemdefs;

    Itemdef             getItemDef(std::string name) const;
    Item                *makeItem(Itemdef) const;

};

class Interpreter {
public:
    bool                open(std::string filename);
    void                close();

    std::fstream        file;

    void                parseToDatabase(Database&);

private:
    void                interpretDef(const std::string);
    void                makeDef(const char* def, const std::string);

    template <class C>
    C                   makeDef(const std::string);

    void                parseProperty(char* propertyname, char* property, Mobdef &def);
    void                parseProperty(char* pname, char* p, Itemdef &def);

    Pos                 parseCoordinates(char* coords);


    Database            *d; //so I don't have to pass it around in all functions, i store it here
};

#define ACTONPROPERTY(n, func) \
    if (strcmp(propertyname, #n) == 0) { \
        def. n = func (property); \
        std::cout<<"Got "<<#n<<": "<<property<<"\n"; \
    } else

#endif


