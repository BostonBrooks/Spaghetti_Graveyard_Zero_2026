#ifndef BBMAPICONS_H
#define BBMAPICONS_H

#include "engine/viewport/bbDrawables.h"

typedef struct {
    bbDrawable drawable;
    bool sighted;
} bbMapIcon;

#define bbMapIconSquare bbDrawableSquare

#define bbMapIcons bbDrawables

#define bbMapIcons_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, squares_i, squares_j, sizeof(bbMapIcon));

#define bbMapIcons_draw bbDrawables_draw

bbFlag bbMapIcon_new(bbMapIcon** self, bbMapIcons* mapicons,
                      bbGraphicsApp* graphics, bbMapCoords MC);

#endif //BBMAPICONS_H