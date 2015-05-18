#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "../Main/main.hpp"


#define MAXIDCHARS 16
#define MAXPROPERTYNAMECHARS 16
#define MAXPROPERTYCHARS 256

struct Mobdef {
    std::string         name;
    int                 hp;
    Pos                 texture,
                        deadtexture;
    int                 swiftness = 100,
                        atk = 10,
                        def = 10,
                        sght = 10,
                        xp = 10;
};

class Database {
public:

    std::unordered_map<std::string, Mobdef>
                        mobdefs;

    Mobdef              getMobDef(std::string name) const;
    Mob                 *makeMob(Pos, Mobdef) const;

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


    Mobdef              makeMobDef(const std::string);

    void                parseProperty(char* propertyname, char* property, Mobdef &def);


    Pos                 parseCoordinates(char* coords);


    Database            *d; //so I don't have to pass it around in all functions, i store it here
};

#define ACTONPROPERTY(name, func) \
    if (strcmp(propertyname, #name) == 0) { \
        def. name = func (property); \
    } else

#endif


