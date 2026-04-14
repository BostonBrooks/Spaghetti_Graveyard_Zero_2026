#ifndef	BB_HOME_H
#define BB_HOME_H

#include "engine/viewport/bbViewportApp.h"
#include "engine/core/bbCore.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/interthread/bbClock.h"
#include "engine/interthread/bbClock2.h"
#include "engine/network/bbNetwork.h"
#include "engine/pong/bbBall.h"
#include "engine/pong/bbPaddle.h"
#include "engine/userinterface/bbUI_Inbox.h"
#include "engine/userinterface/bbWidgets.h"

#include "engine/userinterface/bbUIApp.h"




typedef struct
{
    bbCore core;
    char quote[KEY_LENGTH];
    I64 test_int;
    bbClock2_handle clock2_handle;
    bbBall balls[N_BALLS];
    bbPaddle paddle1;
    bbPaddle paddle2;
} bbHome_core;

typedef struct
{
    bbUIApp UI;
    bbNetwork network;
    bbNetworkTime* network_time;
    //bbClock clock;
    bbClock2 clock2;
    bbHome_core core;
    bbViewportApp viewport_app;
} bbHome;


extern bbHome home;

#endif // BB_HOME_H