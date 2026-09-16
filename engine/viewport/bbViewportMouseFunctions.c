
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/viewport/bbViewportMouseFunctions.h"

#include "bbUnits.h"
#include "bbViewportApp.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"

bbFlag bbVPMouseFunctions_init(bbVPMouseFunctions* functions)
{
    I32 magic_number = nextPrime(256);

    functions->IsOver = calloc(magic_number, sizeof(bbVPMouse_IsOver));
    bbAssert(functions->IsOver != NULL, "bad calloc\n");
    bbDictionary_new(&functions->IsOver_dict, magic_number);
    functions->IsOver_available = 0;

    functions->Enter = calloc(magic_number, sizeof(bbVPMouse_Enter));
    bbAssert(functions->Enter != NULL, "bad calloc\n");
    bbDictionary_new(&functions->Enter_dict, magic_number);
    functions->Enter_available = 0;

    functions->Leave = calloc(magic_number, sizeof(bbVPMouse_Leave));
    bbAssert(functions->Leave != NULL, "bad calloc\n");
    bbDictionary_new(&functions->Leave_dict, magic_number);
    functions->Leave_available = 0;

    functions->LeftDown = calloc(magic_number, sizeof(bbVPMouse_LeftDown ));
    bbAssert(functions->LeftDown != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftDown_dict, magic_number);
    functions->LeftDown_available = 0;

    functions->LeftUp = calloc(magic_number, sizeof(bbVPMouse_LeftUp ));
    bbAssert(functions->LeftUp != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftUp_dict, magic_number);
    functions->LeftUp_available = 0;

    functions->LeftDrag = calloc(magic_number, sizeof(bbVPMouse_LeftDrag ));
    bbAssert(functions->LeftDrag != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftDrag_dict, magic_number);
    functions->LeftDrag_available = 0;

    functions->RightDown = calloc(magic_number, sizeof(bbVPMouse_RightDown ));
    bbAssert(functions->RightDown != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightDown_dict, magic_number);
    functions->RightDown_available = 0;

    functions->RightUp = calloc(magic_number, sizeof(bbVPMouse_RightUp ));
    bbAssert(functions->RightUp != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightUp_dict, magic_number);
    functions->RightUp_available = 0;

    functions->RightDrag = calloc(magic_number, sizeof(bbVPMouse_RightDrag ));
    bbAssert(functions->RightDrag != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightDrag_dict, magic_number);
    functions->RightDrag_available = 0;

    bbVPool_newBloated(&functions->mouse_tables, sizeof (bbVPMouseTable), 10,100, "bbVPMouseTable");
    bbDictionary_new(&functions->mouse_table_dict, magic_number);

}

//bbFlag bbVPMouseFunctions_populate(bbVPMouseFunctions* self); // Not part of engine
bbFlag bbVPMouseFunctions_add(bbVPMouseFunctions* functions, bbVPMouseFunctionType fnType, void* fnPointer, char* key )
{

    I32 available_entries;
    bbHandle dict_entry;
    I32 max_entries = 256;
    switch (fnType)
    {
    case VPMouseIsOver:

        available_entries = functions->IsOver_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->IsOver[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->IsOver_dict, key, dict_entry);
        return bbSuccess;

    case VPMouseEnter:

        available_entries = functions->Enter_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->Enter[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->Enter_dict, key, dict_entry);
        return bbSuccess;

    case VPMouseLeave:

        available_entries = functions->Leave_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->Leave[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->Leave_dict, key, dict_entry);
        return bbSuccess;

        case VPMouseLeftDown:

            available_entries = functions->LeftDown_available++;
            bbAssert(available_entries < max_entries, "out of bounds error\n");
            functions->LeftDown[available_entries] = fnPointer;
            dict_entry.u64 = available_entries;
            bbDictionary_add(functions->LeftDown_dict, key, dict_entry);
            return bbSuccess;

        case VPMouseLeftUp:

            available_entries = functions->LeftUp_available++;
            bbAssert(available_entries < max_entries, "out of bounds error\n");
            functions->LeftUp[available_entries] = fnPointer;
            dict_entry.u64 = available_entries;
            bbDictionary_add(functions->LeftUp_dict, key, dict_entry);
            return bbSuccess;

        case VPMouseLeftDrag:

            available_entries = functions->LeftDrag_available++;
            bbAssert(available_entries < max_entries, "out of bounds error\n");
            functions->LeftDrag[available_entries] = fnPointer;
            dict_entry.u64 = available_entries;
            bbDictionary_add(functions->LeftDrag_dict, key, dict_entry);
            return bbSuccess;

    case VPMouseRightDown:

        available_entries = functions->RightDown_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->RightDown[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->RightDown_dict, key, dict_entry);
        return bbSuccess;

    case VPMouseRightUp:

        available_entries = functions->RightUp_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->RightUp[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->RightUp_dict, key, dict_entry);
        return bbSuccess;

    case VPMouseRightDrag:

        available_entries = functions->RightDrag_available++;
        bbAssert(available_entries < max_entries, "out of bounds error\n");
        functions->RightDrag[available_entries] = fnPointer;
        dict_entry.u64 = available_entries;
        bbDictionary_add(functions->RightDrag_dict, key, dict_entry);
        return bbSuccess;

    default:
        bbAssert(0, "bad widget function type\n");
    }
}
bbFlag bbVPMouseFunctions_addTable(bbVPMouseFunctions* functions, bbVPMouseTable* table, bbHandle* table_handle, char* key)
{
    bbVPMouseTable* new_table;
    bbHandle new_handle;
    bbVPool_alloc2(functions->mouse_tables, (void**)&new_table,&new_handle);
    *new_table = *table;
    bbDictionary_add(functions->mouse_table_dict, key, new_handle);
    bbStr_putStr(new_table->key, key, KEY_LENGTH);
    if (table_handle!=NULL) *table_handle = new_handle;
    return bbSuccess;
}
bbFlag bbVPMouseFunctions_getTable(bbVPMouseFunctions* functions, bbVPMouseTable** table, bbHandle table_handle)
{
    bbVPMouseTable* new_table;
    bbVPool_lookup(functions->mouse_tables, (void**)&new_table,table_handle);
    *table = new_table;
    return bbSuccess;
}
bbFlag bbVPMouseFunctions_getTableHandle(bbVPMouseFunctions* functions, bbHandle* table_handle, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(functions->mouse_table_dict, key, &handle);
    *table_handle = handle;
    return bbSuccess;
}
bbFlag bbVPMouseFunctions_getFunction(bbVPMouseFunctions* functions, void** function, bbVPMouseFunctionType fnType, I32 index)
{   void* function_ptr;
    switch (fnType)
    {
    case VPMouseIsOver:
        function_ptr = functions->IsOver[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseEnter:
        function_ptr = functions->Enter[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseLeave:
        function_ptr = functions->Leave[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseLeftDown:
        function_ptr = functions->LeftDown[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseLeftUp:
        function_ptr = functions->LeftUp[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseLeftDrag:
        function_ptr = functions->LeftDrag[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseRightDown:
        function_ptr = functions->RightDown[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseRightUp:
        function_ptr = functions->RightUp[index];
        *function = function_ptr;
        return bbSuccess;
    case VPMouseRightDrag:
        function_ptr = functions->RightDrag[index];
        *function = function_ptr;
        return bbSuccess;
    default:
        bbNotHere()
        *function = NULL;
        return bbNone;
    }
}
I32 bbVPMouseFunctions_getInt(bbVPMouseFunctions* functions, bbVPMouseFunctionType fnType, char* key)
{
    bbDictionary* dict;
    switch (fnType){
    case VPMouseIsOver:
        dict = functions->IsOver_dict;
        break;
    case VPMouseEnter:
        dict = functions->Enter_dict;
        break;
    case VPMouseLeave:
        dict = functions->Leave_dict;
        break;
    case VPMouseLeftDown:
        dict = functions->LeftDown_dict;
        break;
    case VPMouseLeftUp:
        dict = functions->LeftUp_dict;
        break;
    case VPMouseLeftDrag:
        dict = functions->LeftDrag_dict;
        break;
    case VPMouseRightDown:
        dict = functions->RightDown_dict;
        break;
    case VPMouseRightUp:
        dict = functions->RightUp_dict;
        break;
    case VPMouseRightDrag:
        dict = functions->RightDrag_dict;
        break;

    default:
        bbNotHere() //bad function type
    }

    bbHandle handle;
    bbFlag flag = bbDictionary_lookup(dict,key,&handle);
    bbAssert(flag == bbSuccess, "function not found\n");
    return handle.u64;
}

bbFlag bbVPMouse_IsOver_NULL (void* Mouse, void* units, void* Unit)
{


    bbUnit* unit = Unit;
    I32 closeness = unit->drawable.coords.i - unit->drawable.coords.j;

    bbVPMouse* mouse = (bbVPMouse*)Mouse;
    bbViewportApp* app = (bbViewportApp*)mouse->viewportApp;
    bbMapCoords mouse_coords = bbViewportCoords_getMapCoords_k_fixed(mouse->position,0,&app->viewport);

    //bbFlag flag = bbMapCoords_withinCircle(mouse_coords, unit->drawable.coords, POINTS_PER_TILE);
    //bbDebug("the unit's closeness is %d\n", closeness);

    bbGameWorldRect rect;

    rect.top = 4*POINTS_PER_TILE;
    rect.left = POINTS_PER_TILE;
    rect.height = 4*POINTS_PER_TILE;
    rect.width = 2*POINTS_PER_TILE;
    bbFlag flag  = bbMapCoords_withinRect(mouse_coords, unit->drawable.coords, rect);

    if (flag == bbSuccess) return bbBreak;

    return bbContinue;
}