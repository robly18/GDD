#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <array>
#include <cctype>

#include "../Main/main.hpp"


#define MAXIDCHARS 16
#define MAXPROPERTYNAMECHARS 64
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
    enum {
        TARGETED,
        SELFBUFF,
        HITTHRU,
        BASHSWP,
    };

    int                 atktype;
    std::string         name;
    int                 minrange, maxrange;
    int                 minaccy = 0, maxaccy = 0;
    int                 radius = 0;
    bool                hurtself = false, physical = true;
};

typedef std::array<const Atkdef*, 6> atkdefarr;

struct Itemdef {
    std::string         name;
    int                 itemtype;
    Uint32              color;

    union {
        struct {
            int         heals; //subject to change in the future
        } use;
        struct {
            int         weapontype;
            atkdefarr   atks;
            int         atk,
                        maxmana;
        } wep;
    } data;
};

class Database {
public:

    std::unordered_map<std::string, Mobdef>
                        mobdefs;

    const Mobdef        &getMobDef(std::string name) const;
    Mob                 *makeMob(Pos, Mobdef) const;


    std::unordered_map<std::string, Itemdef>
                        itemdefs;

    const Itemdef       &getItemDef(const std::string name) const;
    Item                *makeItem(Itemdef);


    std::unordered_map<std::string, Atkdef>
                        atkdefs;
    std::unordered_map<std::string, const Attack*>
                        atksingletons;

    const Atkdef        &getAtkDef(std::string name) const;
    const Attack        *getAtk(Atkdef);

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
    void                parseProperty(char* pname, char* p, Atkdef &def);

    Pos                 parseCoordinates(char* coords);
    Uint32              charsToColors(char* colors);
    int                 itemtypeToInt(char *type);
    int                 atktypeToInt(char *type);

    atkdefarr           parseAttacks(char *atks);


    Database            *d; //so I don't have to pass it around in all functions, i store it here
    Itemdef             *thisdef; //Okay this is gonna come back to bite me someday u_u
};

#define ACTONPROPERTY(n, func) \
    if (strcmp(propertyname, #n) == 0) { \
        def. n = func (property); \
        /*std::cout<<"Got "<<#n<<": "<<property<<"\n";*/ \
    } else

int hexchartoint(char c);

#endif


