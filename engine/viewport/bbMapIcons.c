#include "engine/viewport/bbMapIcons.h"

bbFlag bbMapIcon_new(bbMapIcon** self, bbMapIcons* mapicons,
                      bbGraphicsApp* graphics, bbMapCoords MC)
{
    bbVPool* pool = mapicons->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare = bbDrawables_getSquare(mapicons,SC.i, SC.j, mapicons->squares_i, mapicons->squares_j);
    bbDrawable* drawable;
    bbVPool_alloc(pool, (void**)&drawable);
    drawable->md.coords = MC;
    drawable->md.SC = SC;

    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "MAPICON_TEST",
                        &drawfunctionHandle);

    drawable->md.frames[0].draw_function = drawfunctionHandle.u64;
    drawable->md.frames[0].asset_handle.u64 = 141;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        drawable->md.frames[k].draw_function = -1;
    }

    bbList_sortL(&drawableSquare->list, drawable);
    return bbSuccess;
}