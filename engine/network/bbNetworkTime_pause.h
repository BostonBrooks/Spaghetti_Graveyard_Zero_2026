
#include "engine/logic/bbIntTypes.h"

//This prevents a circular dependency
#ifndef BBNETWORKTIMEPAUSEMESSAGE
#define BBNETWORKTIMEPAUSEMESSAGE

typedef struct
{
    U64 reference_server_tick;
    U64 reference_map_tick;
    bool is_paused;
} bbNetwork_pauseMessage;

#endif // BBNETWORKTIMEPAUSEMESSAGE

#ifndef BBNETWORKTIME_PAUSE_H
#define BBNETWORKTIME_PAUSE_H


#endif