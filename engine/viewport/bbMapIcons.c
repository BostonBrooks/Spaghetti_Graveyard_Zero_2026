#include "engine/viewport/bbMapIcons.h"

bbFlag bbMapIcon_new(bbMapIcon** self, bbMapIcons* mapicons,
                      bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = mapicons->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    I32 index = bbDrawables_getSquareIndex(SC.i, SC.j, mapicons->squares_i);
    bbDrawableSquare drawableSquare = mapicons->squares[index];
    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->coords = MC;

    bbPool_Handle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "MAPICONTEST",
                        &drawfunctionHandle);

    drawable->frames[0].drawfunction = drawfunctionHandle.u64;
    drawable->frames[0].handle.u64 = 141;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        drawable->frames[k].drawfunction = -1;
    }

    bbList_sortL(&drawableSquare.list, drawable);
    return Success;
}