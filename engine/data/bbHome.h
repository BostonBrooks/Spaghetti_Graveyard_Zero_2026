
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/interthread/bbClock.h"
#include "engine/network/bbNetwork.h"
#include "engine/userinterface/bbWidgets.h"

typedef struct
{
    bbGraphicsApp graphics;
    bbWidgets widgets;
} bbHome_UI;



typedef struct
{
    bbHome_UI UI;
    bbNetwork network;
    bbNetworkTime* network_time;
    bbClock clock;

} bbHome;


extern bbHome home;