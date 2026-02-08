
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/userinterface/bbWidgets.h"

typedef struct
{
    bbGraphicsApp graphics;
    bbWidgets widgets;
} bbHome_UI;

typedef struct
{
    bbHome_UI UI;
} bbHome;


extern bbHome home;