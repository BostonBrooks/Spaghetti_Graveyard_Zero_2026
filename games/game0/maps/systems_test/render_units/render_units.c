#include "tree_group.h"
#include "engine/render_units/bbRenderUnits.h"


bbFlag bbRenderUnitGroup_unspawnNULL_fn(struct bbRenderUnits* render_units, bbDrawable* drawable)
{
    bbHere()
}

bbFlag bbRenderUnits_populateFunctions (bbRenderUnits* render_units){

    bbRenderUnits_addSpawnFunction(render_units,
                                          bbRenderUnitGroup_spawnFoxes_fn,
                                          "FOXES");
    bbRenderUnits_addSpawnFunction(render_units,
                                          bbRenderUnitGroup_unspawnNULL_fn,
                                          "UNSPAWN NULL");
    bbRenderUnits_addSpawnFunction(render_units,
                                          bbRenderUnitGroup_spawnTrees_fn,
                                          "SPAWN TREES");

    return bbSuccess;
}