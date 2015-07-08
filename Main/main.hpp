
#define LOADFILES {"attacks.data", "items.data", "mobs.data", "level.data"}


#define NSEEALLMOBS
#define NSEEALLTILES

#define NSHOWFPS

#define NLOGGING

#define RANDSEED ::time(NULL)

#define NOICON SDL_Rect{0,0,0,0}

#define GREETINGMESSAGE \
"Hello! Welcome to the game.\nRight click on anything to know more about it!\
\nTry it now, on me!"

#include <SDL.h>
#include "../Data/interpreter.hpp"

#include "..\Misc\gddlist.hpp"
#include "..\Misc\logging.hpp"
#include "..\Misc\posstruct.hpp"
#include "..\Misc\actor.hpp"
class Mob;
class Player;
#include "state.hpp"
#include "..\Mob\Items\inv.hpp"
#include "..\Mob\destructible.hpp"
#include "..\Mob\Ai\ai.hpp"
#include "..\Mob\attack.hpp"
#include "..\Mob\Status\status.hpp"
#include "..\Mob\XP\xp.hpp"
#include "..\Mob\mob.hpp"
#include "..\Map\map.hpp"


#include "engine.hpp"

