#ifndef BB_DRAWABLE_FUNCTIONS
#define BB_DRAWABLE_FUNCTIONS
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbDictionary.h"
typedef struct bbDrawable bbDrawable;
///Notify the drawable that it has entered or left the area around the viewpoint
typedef bbFlag bbDrawable_notifyVisible_fn(struct bbDrawables* drawables, struct bbDrawable* drawable);
typedef bbFlag bbRenderUnitGroup_spawn_fn(struct bbRenderUnits* render_units, bbDrawable* drawable);

typedef struct {

    bbDrawable_notifyVisible_fn** notify_visible;
    bbDictionary*  notify_visible_dict;
    I32  notify_visible_available;


    bbRenderUnitGroup_spawn_fn** spawn_group;
    bbDictionary* spawn_group_dict;
    I32 spawn_group_available;
}bbDrawableFunctionTable;


///Notify the drawable that it has entered the area around the viewpoint
bbFlag bbDrawable_enterVisible(bbDrawableFunctionTable* table, bbDrawable* drawable);
///Notify the drawable that it has left the area around the viewpoint
bbFlag bbDrawable_leaveVisible(bbDrawableFunctionTable* table, bbDrawable* drawable);
///Notify the drawable that the square it is in has left the area around the viewpoint
bbFlag bbDrawable_squareEnterVisible(bbDrawableFunctionTable* table, bbDrawable* drawable);
///Notify the drawable that the square it is in has left the area around the viewpoint
bbFlag bbDrawable_squareLeaveVisible(bbDrawableFunctionTable* table, bbDrawable* drawable);

bbFlag bbDrawables_initFunctionTable(bbDrawableFunctionTable* table);
bbFlag bbDrawables_populateFunctionTable(bbDrawableFunctionTable* table);
bbFlag bbDrawables_addFunction(bbDrawableFunctionTable* table, bbDrawable_notifyVisible_fn* function, char* key);
bbFlag bbDrawables_addSpawner(bbDrawableFunctionTable* table, bbRenderUnitGroup_spawn_fn* function, char* key);


#endif// BB_DRAWABLE_FUNCTIONS