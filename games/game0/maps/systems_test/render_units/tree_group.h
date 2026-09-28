#ifndef TREE_GROUP_H
#define TREE_GROUP_H

#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/render_units/bbRenderUnits.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewportApp.h"


bbFlag bbViewportSpawnTrees(bbViewportApp *viewport_app,
                            bbMapCoords MC,
                            U64 random_seed);



bbFlag bbRenderUnitGroup_spawn_trees(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics);


bbFlag bbRenderUnitGroup_spawnTrees_fn(struct bbRenderUnits* render_units, bbDrawable* drawable);
// bbFlag bbRenderUnitGroup_delete(bbRenderUnits* render_units,
//                                       bbRenderUnitGroup* group);

#endif //TREE_GROUP_H
