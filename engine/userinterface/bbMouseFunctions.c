#include "engine/userinterface/bbMouseFunctions.h"

#include <stdlib.h>

#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbMouseFunctions_init(bbMouseFunctions* functions)
{
    I32 magic_number = nextPrime(256);

    functions->IsOver = calloc(magic_number, sizeof(bbMouse_IsOver));
    bbAssert(functions->IsOver != NULL, "bad calloc\n");
    bbDictionary_new(&functions->IsOver_dict, magic_number);
    functions->IsOver_available = 0;


    functions->Enter = calloc(magic_number, sizeof(bbMouse_Enter));
    bbAssert(functions->Enter != NULL, "bad calloc\n");
    bbDictionary_new(&functions->Enter_dict, magic_number);
    functions->Enter_available = 0;


    functions->Leave = calloc(magic_number, sizeof(bbMouse_Leave));
    bbAssert(functions->Leave != NULL, "bad calloc\n");
    bbDictionary_new(&functions->Leave_dict, magic_number);
    functions->Leave_available = 0;


    functions->LeftDown = calloc(magic_number, sizeof(bbMouse_LeftDown ));
    bbAssert(functions->LeftDown != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftDown_dict, magic_number);
    functions->LeftDown_available = 0;


    functions->LeftUp = calloc(magic_number, sizeof(bbMouse_LeftUp ));
    bbAssert(functions->LeftUp != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftUp_dict, magic_number);
    functions->LeftUp_available = 0;


    functions->LeftDrag = calloc(magic_number, sizeof(bbMouse_LeftDrag ));
    bbAssert(functions->LeftDrag != NULL, "bad calloc\n");
    bbDictionary_new(&functions->LeftDrag_dict, magic_number);
    functions->LeftDrag_available = 0;


    functions->RightDown = calloc(magic_number, sizeof(bbMouse_RightDown ));
    bbAssert(functions->RightDown != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightDown_dict, magic_number);
    functions->RightDown_available = 0;


    functions->RightUp = calloc(magic_number, sizeof(bbMouse_RightUp ));
    bbAssert(functions->RightUp != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightUp_dict, magic_number);
    functions->RightUp_available = 0;


    functions->RightDrag = calloc(magic_number, sizeof(bbMouse_RightDrag ));
    bbAssert(functions->RightDrag != NULL, "bad calloc\n");
    bbDictionary_new(&functions->RightDrag_dict, magic_number);
    functions->RightDrag_available = 0;

    return bbSuccess;
}

bbFlag bbMouseFunctions_add(bbMouseFunctions* functions, MouseFunctionType fnType, void* fnPointer, char* key )
{


    I32 available;
    bbHandle handle;
    I32 magic_number = 256;
    switch (fnType)
    {
    case MouseIsOver:

        available = functions->IsOver_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->IsOver[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->IsOver_dict, key, handle);
        return bbSuccess;

    case MouseEnter:

        available = functions->Enter_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->Enter[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->Enter_dict, key, handle);
        return bbSuccess;

    case MouseLeave:

        available = functions->Leave_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->Leave[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->Leave_dict, key, handle);
        return bbSuccess;

        case MouseLeftDown:

            available = functions->LeftDown_available++;
            bbAssert(available < magic_number, "out of bounds error\n");
            functions->LeftDown[available] = fnPointer;
            handle.u64 = available;
            bbDictionary_add(functions->LeftDown_dict, key, handle);
            return bbSuccess;

        case MouseLeftUp:

            available = functions->LeftUp_available++;
            bbAssert(available < magic_number, "out of bounds error\n");
            functions->LeftUp[available] = fnPointer;
            handle.u64 = available;
            bbDictionary_add(functions->LeftUp_dict, key, handle);
            return bbSuccess;

        case MouseLeftDrag:

            available = functions->LeftDrag_available++;
            bbAssert(available < magic_number, "out of bounds error\n");
            functions->LeftDrag[available] = fnPointer;
            handle.u64 = available;
            bbDictionary_add(functions->LeftDrag_dict, key, handle);
            return bbSuccess;

    case MouseRightDown:

        available = functions->RightDown_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->RightDown[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->RightDown_dict, key, handle);
        return bbSuccess;

    case MouseRightUp:

        available = functions->RightUp_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->RightUp[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->RightUp_dict, key, handle);
        return bbSuccess;

    case MouseRightDrag:

        available = functions->RightDrag_available++;
        bbAssert(available < magic_number, "out of bounds error\n");
        functions->RightDrag[available] = fnPointer;
        handle.u64 = available;
        bbDictionary_add(functions->RightDrag_dict, key, handle);
        return bbSuccess;

    default:
        bbAssert(0, "bad widget function type\n");
    }

}

I32 bbMouseFunctions_getInt(bbMouseFunctions* functions,
                                MouseFunctionType fnType, char* key){
    bbDictionary* dict;
    switch (fnType){
    case MouseIsOver:
        dict = functions->IsOver_dict;
        break;
    case MouseEnter:
        dict = functions->Enter_dict;
        break;
        case MouseLeave:
            dict = functions->Leave_dict;
            break;
        case MouseLeftDown:
            dict = functions->LeftDown_dict;
            break;
        case MouseLeftUp:
            dict = functions->LeftUp_dict;
            break;
        case MouseLeftDrag:
            dict = functions->LeftDrag_dict;
        break;
    case MouseRightDown:
        dict = functions->RightDown_dict;
        break;
    case MouseRightUp:
        dict = functions->RightUp_dict;
        break;
    case MouseRightDrag:
        dict = functions->RightDrag_dict;
        break;

    default:
        bbAssert(0, "bad widget function type\n");
    }

    bbHandle handle;
    bbDictionary_lookup(dict,key,&handle);
    return handle.u64;
}

bbFlag bbMouseFunctions_getFunction(void** function, bbMouseFunctions* functions,
                                     MouseFunctionType fnType, char* key){
    bbHandle handle;

    switch (fnType){
    case MouseIsOver:
        bbDictionary_lookup(functions->IsOver_dict,key,&handle);
        *function = functions->IsOver[handle.u64];
        return bbSuccess;
    default:
        bbAssert(0, "bad widget function type\n");

    }

}