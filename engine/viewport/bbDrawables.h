/** a drawable is (a base class for) any object drawn to the viewport */

#ifndef BBDRAWABLES_H
#define BBDRAWABLES_H

#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbFlag.h"
#include "engine/graphics/bbCompositions.h"
#include "engine/logic/bbList.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbNestedList.h"

#define FRAMES_PER_DRAWABLE 8

typedef struct
{
    bbMapCoords coords;
    float rotation;
    bbList_Handle listElement;
    bbFrame frames[FRAMES_PER_DRAWABLE];
} bbDrawable;

typedef struct
{
    bbSquareCoords coords;
    bbList list;
} bbDrawableSquare;

typedef struct
{

    bbVPool* pool;
    //Drawables outside the usual map grid
    bbList list;
    bbNestedList nestedList;

    //We cant extend bbDrawables because it ends in a flexible array member,
    //so we use a pointer to extra data
    void* extra_data;

    I32 squares_i;
    I32 squares_j;
    bbDrawableSquare squares[];
} bbDrawables;

bbFlag bbDrawables_newImpl(void** self, I32 squares_i, I32
squares_j, I32 sizeOf);

#define bbDrawables_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, squares_i, squares_j, sizeof(bbDrawable));

I32 bbDrawable_isCloser(void* one, void* two);

/** bbDrawables_draw maps bbDrawables_drawFunc tp each bbDrawable */
bbFlag bbDrawables_draw(bbDrawables* drawables, drawFuncClosure* cl,
                        I32 square_i_min, I32 square_j_min,
                        I32 square_i_max, I32 square_j_max);

bbFlag bbDrawablesPlus_draw(drawFuncClosure* cl,
                            I32 square_i_min, I32 square_j_min,
                            I32 square_i_max, I32 square_j_max);

/** bbDrawables_drawFunc calls bbDrawable_draw */
bbFlag bbDrawables_drawFunc(void* node, void* cl);
/** bbbDrawable_draw draws a drawable to the screen*/
bbFlag bbDrawable_draw(bbDrawable* drawable, drawFuncClosure* cl);
I32 bbDrawables_getSquareIndex(I32 i, I32 j, I32 squares_i);

bbFlag bbDrawable_newTree(bbDrawable** self, bbDrawables* drawables,
                          bbGraphicsApp* graphics, bbMapCoords MC);
bbFlag bbDrawable_newCat(bbDrawable** self, bbDrawables* drawables,
                         bbGraphicsApp* graphics, bbMapCoords MC);
bbFlag bbDrawable_newFire(bbDrawable** self, bbDrawables* drawables,
                         bbGraphicsApp* graphics, bbMapCoords MC);
bbFlag bbDrawable_setLocation(bbDrawable* self, bbDrawables* drawables,
                              bbMapCoords MC);


#endif //BBDRAWABLES_H

