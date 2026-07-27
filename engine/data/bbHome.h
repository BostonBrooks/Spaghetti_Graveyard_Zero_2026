#ifndef	BB_HOME_H
#define BB_HOME_H

#include "engine/viewport/bbViewportApp.h"
#include "engine/core/bbCore.h"
#include "engine/ECS/bbEntitySpawner.h"
#include "engine/ECS/bbGraphicsSystem.h"
#include "engine/ECS/bbServerEntities.h"
#include "engine/ECS/ECS.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/interthread/bbClock.h"
#include "engine/network/bbNetwork.h"
#include "engine/userinterface/bbUI_Inbox.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbUIApp.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/spawner/bbSpawner.h"
#include "engine/ECS/bbMoveables.h"

typedef struct
{
    bbECS ECS;
    bbServerEntities* server_entities;
    bbGraphicsSystem* graphics_system;
    bbMoveables moveables;
    bbEntitySpawner spawner;
} bbECS_data;

typedef struct
{
    bbCore core;
    char quote[KEY_LENGTH];
    I64 test_int;
    bbClock_handle clock2_handle;

    ///in order to rewind the viewpoint, we mist keep a record of where it was before updating
    bbMapCoords viewpoint;
    bbMapCoords goalpoint;
} bbHome_core;

typedef struct
{
    bbUIApp UI;
    bbNetwork network;
    bbNetworkTime* network_time;
    bbClock clock2;
    bbHome_core core;
    bbViewportApp viewport_app;
    bbGroundSurface ground_surface;
    //bbSpawner spawner;
    bbECS_data ECS;
} bbHome;


extern bbHome home;

#endif // BB_HOME_H