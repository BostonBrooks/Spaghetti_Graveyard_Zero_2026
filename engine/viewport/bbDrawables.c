#include "engine/viewport/bbDrawables.h"
#include "engine/logic/bbNestedList.h"
//#include "engine/viewport/bbUnits.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

I32 bbDrawables_getSquareIndex(I32 i, I32 j, I32 squares_i, I32 squares_j){

    if (i<0 || i>=squares_i) return -1;
    if (j<0 || j>=squares_j) return -1;


    return i + squares_i * j;
}

bbDrawableSquare* bbDrawables_getSquare(bbDrawables* drawables, I32 i, I32 j, I32 squares_i, I32 squares_j)
{
    bbDrawableSquare* square;
    I32 index = bbDrawables_getSquareIndex(i, j, squares_i, squares_j);
    if (index == -1)
    {
        square = &drawables->lost;
    } else square = &drawables->squares[index];

    return square;
}

I32 bbDrawable_isCloser(void* one, void* two){
    bbDrawable* iconOne = one;
    bbDrawable* iconTwo = two;

    I32 foo = iconTwo->coords.i - iconOne->coords.i
              -iconTwo->coords.j + iconOne->coords.j;

    return (foo > 0);
}

bbFlag bbDrawables_newImpl(void** self, I32 squares_i, I32
squares_j, I32 sizeOf){
    bbDrawables* drawables = malloc(sizeof(bbDrawables) + sizeof
            (bbDrawableSquare)*squares_i*squares_j);
    bbAssert(drawables != NULL, "bad malloc\n");

    bbVPool* pool;

    bbVPool_newBloated(&pool, sizeOf, 1000, 1000,"bbDrawable_Impl");

    drawables->pool = pool;
    bbList_init(&drawables->list, pool,NULL,offsetof(bbDrawable, listElement)
                ,bbDrawable_isCloser);



    drawables->squares_i = squares_i;
    drawables->squares_j = squares_j;

    bbDrawableSquare* drawableSquare = &drawables->lost;
    drawableSquare->coords.i = -1;
    drawableSquare->coords.j = -1;
    drawableSquare->coords.k = 0;

    bbList_init(&drawableSquare->list, pool, NULL,offsetof
    (bbDrawable, listElement),bbDrawable_isCloser);

    for (I32 i = 0; i < squares_i;i++){
        for (I32 j = 0; j < squares_j; j++){
            I32 n = bbDrawables_getSquareIndex(i, j, squares_i, squares_j);
            bbDrawableSquare* drawableSquare = &drawables->squares[n];
            drawableSquare->coords.i = i;
            drawableSquare->coords.j = j;
            drawableSquare->coords.k = 0;

            bbList_init(&drawableSquare->list, pool, NULL,offsetof
            (bbDrawable, listElement),bbDrawable_isCloser);
        }
    }
    *self = drawables;
    return bbSuccess;
}



///typedef bbFlag bbNestedList_mapFunction(void* node, void* cl);
bbFlag bbDrawable_drawFunc(void* node, void* cl){
    return bbDrawable_draw(node, cl);
}

bbFlag bbDrawable_draw(bbDrawable* drawable, drawFuncClosure* cl){
    for (I32 i = 0; i < FRAMES_PER_DRAWABLE; i++){
        bbFrame* frame = &drawable->frames[i];

        bbGraphicsApp* graphics = cl->graphics;
/// the 8 in the next line refers to the number of draw functions in bbDrawfunctions
        if (frame->drawfunction >= 0 && frame->drawfunction <
        graphics->drawfunctions->num) {

            bbDrawFunction *drawFunction =
                    graphics->drawfunctions->functions[frame->drawfunction];
            if (drawFunction == NULL) continue;
            drawFunction(drawable, frame, cl);

        }
    }
}

bbFlag bbDrawables_draw(bbDrawables* drawables, drawFuncClosure* cl,
                        I32 square_i_min, I32 square_j_min,
                        I32 square_i_max, I32 square_j_max){
    bbNestedList list;
    bbNestedList_init(&list);
    I32 squares_i = drawables->squares_i;
    I32 squares_j = drawables->squares_j;

    for (int i = square_i_min; i < square_i_max; ++i) {
        for (int j = square_j_min; j < square_j_max; ++j) {
            I32 n = i + squares_i * j;

            //bbUnits* units = home.private.viewportApp.units;
            bbNestedList_attach(&list, &drawables->squares[n].list);
            //bbNestedList_attach(&list, &units->squares[n].list);
        }

    }

    bbNestedList_map(&list, bbDrawable_drawFunc, cl);

    //TODO bbNestedList_cleanup;
    return bbSuccess;
}

bbFlag bbDrawablesPlus_draw(drawFuncClosure* cl,
                            I32 square_i_min, I32 square_j_min,
                            I32 square_i_max, I32 square_j_max){
    bbDrawables* drawables = home.viewport_app.drawables;
    bbDrawables* mapicons = home.viewport_app.mapIcons;
    bbDrawables* units = home.viewport_app.units;

    bbNestedList list;
    bbNestedList_init(&list);

    I32 squares_i = drawables->squares_i;
    I32 squares_j = drawables->squares_j;

    for (I32 i = square_i_min; i < square_i_max; ++i) {
        for (I32 j = square_j_min; j < square_j_max; ++j) {
            I32 n = i + squares_i * j;

            bbNestedList_attach(&list, &drawables->squares[n].list);
            bbNestedList_attach(&list, &units->squares[n].list);
        }

    }

    for (I32 i = 0; i < squares_i; ++i) {
        for (I32 j = 0; j < squares_j; ++j) {
            I32 n = i + squares_i * j;

            bbNestedList_attach(&list, &mapicons->squares[n].list);
        }

    }

    bbNestedList_map(&list, bbDrawable_drawFunc, cl);


    return bbSuccess;
}


//TODO what if MC is out of bounds
bbFlag bbDrawable_newTree(bbDrawable** self, bbDrawables* drawables,
                          bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = drawables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare = bbDrawables_getSquare(drawables,SC.i, SC.j, drawables->squares_i, drawables->squares_j);

    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->coords = MC;

    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_SPRITE",
                        &drawfunctionHandle);

    drawable->frames[0].drawfunction = drawfunctionHandle.u64;
    drawable->frames[0].handle.u64 = 134;
    drawable->frames[0].offset.x = 0;
    drawable->frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_ANIMATION",
                        &drawfunctionHandle);

    drawable->frames[1].drawfunction = drawfunctionHandle.u64;
    drawable->frames[1].handle.u64 = 8;
    drawable->frames[1].start_time =  -(rand()%6);
    drawable->frames[1].framerate = 1;
    drawable->frames[1].offset.x = 0;
    drawable->frames[1].offset.y = 0;


    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_SHADOW",
                        &drawfunctionHandle);

    drawable->frames[2].drawfunction = drawfunctionHandle.u64;
    drawable->frames[2].handle.u64 = 611;

    for (I32 k = 3; k < FRAMES_PER_DRAWABLE; k++){
        drawable->frames[k].drawfunction = -1;
    }

    bbList_sortL(&drawableSquare->list, drawable);

    //bbAvoidable_newCircle(home.agents_app.avoidables, MC, 193);


    *self = drawable;
    return bbSuccess;
}
bbFlag bbDrawable_newCat(bbDrawable** self, bbDrawables* drawables,
                          bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = drawables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare =  bbDrawables_getSquare(drawables,SC.i, SC.j, drawables->squares_i, drawables->squares_j);

    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->coords = MC;

    bbHandle drawfunctionHandle;



    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "EYE_CANDY",
                        &drawfunctionHandle);

    drawable->frames[0].drawfunction = drawfunctionHandle.u64;
    drawable->frames[0].handle.u64 = 7;
    drawable->frames[0].start_time =  -(rand()%6);
    drawable->frames[0].framerate = 1;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        drawable->frames[k].drawfunction = -1;
    }

    bbList_sortL(&drawableSquare->list, drawable);
    *self = drawable;
    return bbSuccess;
}

bbFlag bbDrawable_newSkeleton(bbDrawable** self, bbDrawables* drawables,
                          bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = drawables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare = bbDrawables_getSquare(drawables,SC.i, SC.j, drawables->squares_i, drawables->squares_j);

    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->coords = MC;

    bbHandle drawfunctionHandle;



    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_ANIMATION",
                        &drawfunctionHandle);

    drawable->frames[0].drawfunction = drawfunctionHandle.u64;
    drawable->frames[0].handle.u64 = 9;
    drawable->frames[0].start_time =  -(rand()%6);
    drawable->frames[0].framerate = 1;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        drawable->frames[k].drawfunction = -1;
    }

    bbList_sortL(&drawableSquare->list, drawable);
    *self = drawable;
    return bbSuccess;
}


//TODO what if MC or drawable is out of bounds?
//TODO it might be faster, if the drawable stays in the same square,
//that we move the drawable up or down in the list instead of removing it
// then re-inserting it
bbFlag bbDrawable_setLocation(bbDrawable* drawable, bbDrawables* drawables,
                              bbMapCoords MC){
    bbSquareCoords newSC = bbMapCoords_getSquareCoords(MC);
    bbSquareCoords oldSC = bbMapCoords_getSquareCoords(drawable->coords);
    I32 newIndex = bbDrawables_getSquareIndex(newSC.i,
                                           newSC.j,
                                           drawables->squares_i, drawables->squares_j);
    I32 oldIndex = bbDrawables_getSquareIndex(oldSC.i,
                                              oldSC.j,
                                              drawables->squares_i, drawables->squares_j);
    bbDrawableSquare* newSquare= bbDrawables_getSquare(drawables,newSC.i, newSC.j, drawables->squares_i, drawables->squares_j);


    bbDrawableSquare* oldSquare= bbDrawables_getSquare(drawables,oldSC.i, oldSC.j, drawables->squares_i, drawables->squares_j);


    bbList_remove(&oldSquare->list, drawable);
    drawable->coords = MC;
    bbList_sortL(&newSquare->list, drawable);

    return bbSuccess;
}

bbFlag bbDrawable_newSphere(bbDrawable** self, bbDrawables* drawables,
                          bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = drawables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare = bbDrawables_getSquare(drawables,SC.i, SC.j, drawables->squares_i, drawables->squares_j);

    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->coords = MC;

    bbHandle drawfunctionHandle;



    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_SPRITE",
                        &drawfunctionHandle);

    drawable->frames[0].drawfunction = drawfunctionHandle.u64;
    drawable->frames[0].handle.u64 = 615;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    drawable->frames[1].drawfunction = drawfunctionHandle.u64;
    drawable->frames[1].handle.u64 = 612;

    for (I32 k = 2; k < FRAMES_PER_DRAWABLE; k++){
        drawable->frames[k].drawfunction = -1;
    }

    bbList_sortL(&drawableSquare->list, drawable);
    *self = drawable;
    return bbSuccess;
}

