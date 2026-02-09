
#include "engine/graphics/bbGraphicsApp.h"
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
} bbHome;


extern bbHome home;