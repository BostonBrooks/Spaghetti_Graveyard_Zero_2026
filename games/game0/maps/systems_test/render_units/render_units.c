#include "engine/render_units/bbRenderUnits.h"

bbFlag bbRenderUnits_populateFunctions (bbRenderUnits* render_units){

    bbRenderUnits_addSpawnFunction(render_units,
                                          bbRenderUnitGroup_spawnFoxes_fn,
                                          "FOXES");

    return bbSuccess;
}