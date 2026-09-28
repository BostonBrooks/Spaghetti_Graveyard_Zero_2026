#include "engine/data/bbHome.h"
#include "engine/logic/bbTerminal.h"
#include "engine/render_units/bbRenderUnits.h"
#include "engine/viewport/bbDrawableFunctions.h"
#include "engine/viewport/bbDrawables.h"

///bb Drawable, notify visible enter function null
///if the drawable enters  the area around the viewport, notify by doing bbHare()
bbFlag bbDNV_nullNull(struct bbDrawables* drawables, struct bbDrawable* drawable) {
    bbDebug("Drawable notify visible not defined\n");
    return bbSuccess;
}
///bb Drawable, notify visible enter function null
///if the drawable enters  the area around the viewport, notify by doing bbHare()
bbFlag bbDNV_enterNull(struct bbDrawables* drawables, struct bbDrawable* drawable) {
    bbHere()
    return bbSuccess;
}

///bb Drawable, notify visible leave function null
///if the drawable leaves the area around the viewport, notify by doing bbHare()
bbFlag bbDNV_leaveNull(struct bbDrawables* drawables, struct bbDrawable* drawable) {
    bbHere()
    return bbSuccess;
}


///bb Drawable, notify visible square enter function null
///if the square containing the drawable enters the area around the viewport, notify by doing bbHare()
bbFlag bbDNV_squareEnterNull(struct bbDrawables* drawables, struct bbDrawable* drawable) {
    bbHere()
    return bbSuccess;
}

///bb Drawable, notify visible square leave function null
///if the square containing the drawable leaves the area around the viewport, notify by doing bbHare()
bbFlag bbDNV_squareLeaveNull(struct bbDrawables* drawables, struct bbDrawable* drawable) {
    bbHere()
    return bbSuccess;
}

///Attempt to spawn a Render Unit Group, but instead do bbHere()
bbFlag bbRUG_spawnNULL_fn(struct bbRenderUnits* render_units, bbDrawable* drawable) {
    bbHere();
    return bbSuccess;
}

bbFlag bbDNV_spawnFoxes(struct bbDrawables* drawables, struct bbDrawable* drawable) {

    bbRenderUnitGroup_spawnKey(home.viewport_app.renderUnits,
                               "FOXES",
                               drawable);
    return bbSuccess;
}

bbFlag bbDNV_unspawnFoxes(struct bbDrawables* drawables, struct bbDrawable* drawable) {

    if (drawable->group == NULL) {
        bbDebug("trying to unspawn null foxes\n")
        return bbSuccess;
    }

    bbRenderUnitGroup_delete(home.viewport_app.renderUnits,
                                      drawable->group);
    drawable->group = NULL;
    return bbSuccess;
}

bbFlag bbDrawables_populateFunctionTable(bbDrawableFunctionTable* table){
    bbDrawables_addFunction(table, bbDNV_nullNull, "NULL NULL");
    bbDrawables_addFunction(table, bbDNV_enterNull, "ENTER NULL");
    bbDrawables_addFunction(table, bbDNV_leaveNull, "LEAVE NULL");
    bbDrawables_addFunction(table, bbDNV_squareEnterNull, "SQUARE ENTER NULL");
    bbDrawables_addFunction(table, bbDNV_squareLeaveNull, "SQUARE LEAVE NULL");
    bbDrawables_addFunction(table, bbDNV_spawnFoxes, "SPAWN FOXES");
    bbDrawables_addFunction(table, bbDNV_unspawnFoxes, "UNSPAWN FOXES");
    bbDrawables_addSpawner(table, bbRUG_spawnNULL_fn, "SPAWN NULL");
    return bbSuccess;

}

//typedef bbFlag bbDrawable_notifyVisible_fn(struct bbDrawables* drawables, struct bbDrawable* drawable);

//bbFlag bbDrawables_addFunction(bbDrawables* drawables, bbDrawable_notifyVisible_fn* function, char* key);
