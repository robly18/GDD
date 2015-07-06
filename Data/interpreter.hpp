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
#define MAXPROPERTYCHARS 512

struct Mobdef {
    std::string         name;
    std::string         desc;

    int                 hp;
    Pos                 texture,
                        deadtexture;
    int                 swiftness = 0,
                        atk = 0,
                        def = 0,
                        sght = 0,
                        xp = 0;
};

struct Statusdef {
    enum {
        BLOCK,
        THORN,
        FROZEN,
        REGEN,
        POISON,
    };
    std::string         id;

    std::string         name;

    int                 statustype;
    Pos                 pos;
    Pos                 spritepos;
    int                 time;

    int                 str;
};
struct StatusChance;

struct Atkdef {
    enum {
        TARGETED,
        SELFBUFF,
        HITTHRU,
        BASHSWP,
    };
    std::string         id;

    int                 atktype = TARGETED;
    std::string         name = "";

    int                 damage, cost;

    int                 minrange = 0, maxrange = 1;
    int                 minaccy = 0, maxaccy = 0;
    int                 radius = 0;
    bool                hurtself = false, physical = true;

    const Status        *buff;

    std::vector
        <StatusChance>  addstatus = {}; //This varname may not seem to make much sense,
};                                      //but it does for the interpreter

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

struct Leveldef {
    std::vector<std::pair<Mobdef, int>>
                        mobs;
    //a pair of a mobdef and the number of times that mob should appear

    int                 roomnum;

    int                 flags = 0;
    enum {
        FIRSTLEVEL = 0x01,
        LASTLEVEL = 0x02,
    };
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
    Item                *makeItem(Itemdef) const;


    std::unordered_map<std::string, Atkdef>
                        atkdefs;
    mutable std::unordered_map<std::string, const Attack*>
                        atksingletons;

    const Atkdef        &getAtkDef(std::string name) const;
    const Attack        *getAtk(Atkdef) const;

    std::unordered_map<std::string, Statusdef>
                        statusdefs; //todo add singletons

    const Statusdef     &getStatusdef(std::string name) const;
    const Status        *getStatus(Statusdef) const;
    StatusChance        getStatusChance(std::string name, int chance);

    std::vector<Leveldef>
                        levels;
    const Leveldef      &getLeveldef(int lvl) const;
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

    void                parseProperty(char* pname, char* p, Mobdef &def);
    void                parseProperty(char* pname, char* p, Itemdef &def);
    void                parseProperty(char* pname, char* p, Atkdef &def);
    void                parseProperty(char* pname, char* p, Statusdef &def);
    void                parseProperty(char* pname, char* p, Leveldef &def);

    Pos                 parseCoordinates(char* coords);
    Uint32              charsToColors(char* colors);
    bool                parseBool(char* b);

    int                 itemtypeToInt(char *type);
    int                 atktypeToInt(char *type);
    int                 statustypeToInt(char *type);

    atkdefarr           parseAttacks(char *atks);
    std::vector
        <StatusChance>  addChance(char *c);
    const Status        *parseStatus(char *c);

    void                addEnemy(char *id, Leveldef &lvl);
    void                toggleFlag(char *id, Leveldef &lvl);

    Database            *d; //so I don't have to pass it around in all functions, i store it here
    Itemdef             *thisitemdef; //Okay this is gonna come back to bite me someday u_u
    Atkdef              *thisatkdef;
};

#define ACTONPROPERTY(n, func) \
    if (strcmp(propertyname, #n) == 0) { \
        def. n = func (property); \
    } else

#define MODIFYPROPERTYWITHDEF(n, func) \
    if (strcmp(propertyname, #n) == 0) { \
        func (property, def); \
    } else

#define IFRETURNENUM(var, nspace, num) \
    if (strcmp(var, #num) == 0) return nspace num;

int hexchartoint(char c);

#endif


