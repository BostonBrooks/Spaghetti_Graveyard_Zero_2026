#ifndef BBACTIVESQUARES_H
#define BBACTIVESQUARES_H

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"

typedef struct bbViewportApp bbViewportApp;

typedef struct {
    I32 old_i_min,old_j_min,old_i_max,old_j_max;
    I32 new_i_min,new_j_min,new_i_max,new_j_max;
} bbActiveSquares;

bbFlag bbActiveSquares_update(bbViewportApp* viewport_app, bbActiveSquares* active_squares,
                        I32 new_i_min,I32 new_j_min,I32 new_i_max,I32 new_j_max);

bbFlag bbActiveSquare_activate(bbViewportApp* viewport_app, bbActiveSquares* active_squares, I32 i, I32 j);
bbFlag bbActiveSquare_deactivate(bbViewportApp* viewport_app, bbActiveSquares* active_squares, I32 i, I32 j);

bbFlag bbActiveSquares_init(bbActiveSquares* active_squares);

#endif // BBACTIVESQUARES_H