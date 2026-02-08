
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"

#define NUM_DRAWFUNCTIONS 0
bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions){

    bbDrawfunctions* functions = malloc(sizeof(bbDrawfunctions) + NUM_DRAWFUNCTIONS * sizeof(bbDrawFunction*));
    bbAssert(functions!=NULL, "bad malloc");
    bbDictionary_new(&functions->dictionary, nextPrime(NUM_DRAWFUNCTIONS));

    functions->num = NUM_DRAWFUNCTIONS;


    *drawfunctions = functions;
    return bbSuccess;
}
