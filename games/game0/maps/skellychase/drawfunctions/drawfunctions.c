
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/map0/drawfunctions/textboxindicator.h"
#include "games/game0/maps/map0/drawfunctions/button_state.h"
#include "games/game0/maps/map0/drawfunctions/widget_text.h"

#define NUM_DRAWFUNCTIONS 69



bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions){

    bbDrawfunctions* functions = malloc(sizeof(bbDrawfunctions) + NUM_DRAWFUNCTIONS * sizeof(bbDrawFunction*));
    bbAssert(functions!=NULL, "bad malloc");
    bbDictionary_new(&functions->dictionary, nextPrime(NUM_DRAWFUNCTIONS));

    functions->num = NUM_DRAWFUNCTIONS;


    *drawfunctions = functions;
    return bbSuccess;
}

