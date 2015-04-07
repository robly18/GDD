#ifndef DEBUGMSG_H
#define DEBUGMSG_H

#include <iostream>

//Remember to recompile the project...

#ifndef NLOGGING
#define DEBUGMSG(msg)           \
        std::cout<<msg<<'\n'
#else
#define DEBUGMSG(msg)
#endif

#endif
