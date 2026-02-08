#ifndef BB_DRAWFUNCTIONS_H
#define BB_DRAWFUNCTIONS_H

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbDictionary.h"

typedef bbFlag bbDrawFunction(void* drawable, void* frame_descriptor, void* cl);

typedef struct {
    void* graphics;
    void* target;
    I32 GUI_time;
    I32 map_time;
} drawFuncClosure;

typedef struct {
    I32 num;
    bbDictionary* dictionary;
    bbDrawFunction* functions[];
} bbDrawfunctions;

bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions);

#endif // BB_DRAWFUNCTIONS_H