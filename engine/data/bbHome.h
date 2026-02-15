#ifndef	BB_HOME_H
#define BB_HOME_H

#include "engine/core/bbCore.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/interthread/bbClock.h"
#include "engine/network/bbNetwork.h"
#include "engine/userinterface/bbUI_Inbox.h"
#include "engine/userinterface/bbWidgets.h"

typedef struct
{
    bbGraphicsApp graphics;
    bbWidgets widgets;
    bbUI_Inbox inbox;
} bbHome_UI;

typedef struct
{
    bbCore core;
    char quote[KEY_LENGTH];
} bbHome_core;

typedef struct
{
    bbHome_UI UI;
    bbNetwork network;
    bbNetworkTime* network_time;
    bbClock clock;

    bbHome_core core;

} bbHome;


extern bbHome home;

#endif // BB_HOME_H