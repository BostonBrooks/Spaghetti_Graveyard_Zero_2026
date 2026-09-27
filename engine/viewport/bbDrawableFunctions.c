#include "bbDrawableFunctions.h"

#include "bbDrawables.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbDrawables_addFunction(bbDrawableFunctionTable* table, bbDrawable_notifyVisible_fn* function, char* key)
{
    bbHandle handle;
    I32 available = table->notify_visible_available++;
    bbAssert(available < 193, "out of space error\n");
    table->notify_visible[available] = function;
    handle.u64 = available;
    bbDictionary_add(table->notify_visible_dict, key, handle);
    return bbSuccess;
}
bbFlag bbDrawables_addSpawner(bbDrawableFunctionTable* table, bbRenderUnitGroup_spawn_fn* function, char* key)
{
    bbHandle handle;
    I32 available = table->spawn_group_available++;
    bbAssert(available < 193, "out of space error\n");
    table->spawn_group[available] = function;
    handle.u64 = available;
    bbDictionary_add(table->spawn_group_dict, key, handle);
    return bbSuccess;
}


bbFlag bbDrawable_enterVisible(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
{
    I32 index = drawable->functions.on_enter;
    bbDrawable_notifyVisible_fn* function = table->notify_visible[index];
    return function(drawables, drawable);
}

bbFlag bbDrawable_leaveVisible(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
{
    I32 index = drawable->functions.on_leave;
    bbDrawable_notifyVisible_fn* function = table->notify_visible[index];
    return function(drawables, drawable);
}
bbFlag bbDrawable_squareEnterVisible(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
{
    I32 index = drawable->functions.on_square_enter;
    bbDrawable_notifyVisible_fn* function = table->notify_visible[index];
    return function(drawables, drawable);
}
bbFlag bbDrawable_squareLeaveVisible(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
{
    I32 index = drawable->functions.on_square_leave;
    bbDrawable_notifyVisible_fn* function = table->notify_visible[index];
    return function(drawables, drawable);
}


// bbFlag bbDrawable_spawnRenderUnits(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
// {
//     bbNotImplemented()
// }
// bbFlag bbDrawable_unspawnRenderUnits(bbDrawables* drawables, bbDrawableFunctionTable* table, bbDrawable* drawable)
// {
//     bbNotImplemented()
// }

bbFlag bbDrawables_initFunctionTable(bbDrawableFunctionTable* table) {
    I32 magic_number = 193;
    table->notify_visible = calloc(magic_number, sizeof(bbDrawable_notifyVisible_fn*));
    bbAssert(table->notify_visible != NULL, "bad calloc\n");
    bbDictionary_new(&table->notify_visible_dict, magic_number);
    table->notify_visible_available = 0;

    table->spawn_group = calloc(magic_number, sizeof(bbRenderUnitGroup_spawn_fn*));
    bbAssert(table->spawn_group != NULL, "bad calloc\n");
    bbDictionary_new(&table->spawn_group_dict, magic_number);
    table->spawn_group_available = 0;

    return bbSuccess;
}