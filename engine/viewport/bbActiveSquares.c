
#include "engine/viewport/bbViewportApp.h"
#include "engine/viewport/bbActiveSquares.h"

bbFlag bbActiveSquares_update(bbViewportApp* viewport_app, bbActiveSquares* active_squares,
                        I32 new_i_min,I32 new_j_min,I32 new_i_max,I32 new_j_max) {

    active_squares->old_i_min = active_squares->new_i_min;
    active_squares->old_j_min = active_squares->new_j_min;
    active_squares->old_i_max = active_squares->new_i_max;
    active_squares->old_j_max = active_squares->new_j_max;

    active_squares->new_i_min = new_i_min;
    active_squares->new_j_min = new_j_min;
    active_squares->new_i_max = new_i_max;
    active_squares->new_j_max = new_j_max;


    for (I32 i = active_squares->old_i_min; i < active_squares->old_i_max; ++i) {
        for (I32 j = active_squares->old_j_min; j < active_squares->old_j_max; ++j) {

            if (!(i >= new_i_min) || !(i < new_i_max) || !(j >= new_j_min) || !(j < new_j_max)) {

                bbActiveSquare_deactivate(viewport_app, active_squares, i, j);
            }
        }
    }


    for (I32 i = new_i_min; i < new_i_max; ++i) {
        for (I32 j = new_j_min; j < new_j_max; ++j) {

            if (!(i >= active_squares->old_i_min) || !(i < active_squares->old_i_max)
                || !(j >= active_squares->old_j_min) || !(j < active_squares->old_j_max)) {

                bbActiveSquare_activate(viewport_app, active_squares, i, j);
            }
        }
    }

    return bbSuccess;
}



bbFlag bbActiveSquare_activate_fn(bbList* list, void* node, void* cl);
bbFlag bbActiveSquare_deactivate_fn(bbList* list, void* node, void* cl);

typedef struct {
    bbDrawables* drawables;
    bbDrawableFunctionTable* table;
} bbActiveSquares_cl;

bbFlag bbActiveSquare_activate(bbViewportApp* viewport_app, bbActiveSquares* active_squares, I32 i, I32 j) {
    bbDebug("i = %d, j = %d\n", i, j);
    bbActiveSquares_cl cl;
    cl.table = &viewport_app->table;
    cl.drawables = viewport_app->units;
    I32 index = bbDrawables_getSquareIndex(i, j, viewport_app->units->squares_i,viewport_app->units->squares_j);

    if (index>=0) {
        bbDrawableSquare* square = &viewport_app->units->squares[index];
        bbList_mapL(&square->list, bbActiveSquare_activate_fn, &cl);
    }
    return bbSuccess;
}
bbFlag bbActiveSquare_deactivate(bbViewportApp* viewport_app, bbActiveSquares* active_squares, I32 i, I32 j){
    bbDebug("i = %d, j = %d\n", i, j);
    bbActiveSquares_cl cl;
    cl.table = &viewport_app->table;
    cl.drawables = viewport_app->units;
    I32 index = bbDrawables_getSquareIndex(i, j, viewport_app->units->squares_i,viewport_app->units->squares_j);

    if (index>=0) {
        bbDrawableSquare* square = &viewport_app->units->squares[index];
        bbList_mapL(&square->list, bbActiveSquare_deactivate_fn, &cl);
    }
    return bbSuccess;
}


bbFlag bbActiveSquares_init(bbActiveSquares* active_squares) {
    active_squares->new_i_min = 0;
    active_squares->new_j_min = 0;
    active_squares->new_i_max = 0;
    active_squares->new_j_max = 0;
    active_squares->old_i_min = 0;
    active_squares->old_j_min = 0;
    active_squares->old_i_max = 0;
    active_squares->old_j_max = 0;
    return bbSuccess;

}


bbFlag bbActiveSquare_activate_fn(bbList* list, void* node, void* cl)
{
    bbDrawable* drawable = (bbDrawable*)node;
    bbActiveSquares_cl* closure = (bbActiveSquares_cl*)cl;
    bbDrawable_squareEnterVisible(closure->drawables, closure->table, drawable);
    return bbContinue;
}
bbFlag bbActiveSquare_deactivate_fn(bbList* list, void* node, void* cl)
{
    bbDrawable* drawable = (bbDrawable*)node;
    bbActiveSquares_cl* closure = (bbActiveSquares_cl*)cl;
    bbDrawable_squareLeaveVisible(closure->drawables, closure->table, drawable);
    return bbContinue;
}
/// map function to list, going left-to-right
bbFlag bbList_mapL(bbList* list, bbListFunction* myFunc, void* cl);

///