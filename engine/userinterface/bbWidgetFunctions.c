#include "engine/userinterface/bbWidgetFunctions.h"

#include "engine/logic/bbPrime.h"
#include "engine/userinterface/bbWidgets.h"

bbFlag bbWidgetFunctions_new(bbWidgetFunctions** self)
{
    bbWidgetFunctions* functions = calloc(1, sizeof(bbWidgetFunctions));
    I32 magic_number = nextPrime(256);

    functions->constructors = calloc(magic_number, sizeof(U64));
    bbAssert(functions->constructors != NULL, "bad calloc\n");
    bbDictionary_new(&functions->constructor_dict, magic_number);
    functions->constructor_available = 0;

    *self = functions;
    return bbSuccess;
}

bbFlag bbWidgetFunctions_add(bbWidgetFunctions* functions, bbWidgetFunctionType function_type, void* function_pointer, char* key )
{

    U32 available;
    bbHandle handle;
    I32 magic_number = nextPrime(256);
    switch (function_type)
    {
    case WidgetConstructor:
        available = functions->constructor_available++;
        bbAssert(available < magic_number, "out of bounds error\n");

        functions->constructors[available] = function_pointer;
        handle.u64 = available;

        bbDictionary_add(functions->constructor_dict, key, handle);

        return bbSuccess;

    default:
        bbAssert(0, "bad widget function type\n");
    }

    return bbSuccess;
}

I32 bbWidgetFunctions_getInt(bbWidgetFunctions* functions,
                             bbWidgetFunctionType function_type, char* key){
    bbDictionary* dict;
    switch (function_type){
    case WidgetConstructor:
        dict = functions->constructor_dict;
        break;
    }

    bbHandle handle;
    bbDictionary_lookup(dict,key,&handle);
    return handle.u64;
}


bbFlag bbWidgetFunctions_getFunction(void** function, bbWidgetFunctions* functions,
                                     bbWidgetFunctionType function_type, char* key){
    bbHandle handle;

    switch (function_type)
    {
    case WidgetConstructor:
        bbDictionary_lookup(functions->constructor_dict,key,&handle);
        *function = functions->constructors[handle.u64];
        return bbSuccess;
    }
    *function = NULL;
    return bbNone;
}