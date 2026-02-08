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

    functions->commands = calloc(magic_number, sizeof(U64));
    bbAssert(functions->commands != NULL, "bad calloc\n");
    bbDictionary_new(&functions->command_dict, magic_number);
    functions->command_available = 0;

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

    case WidgetCommand:
        available = functions->command_available++;
        bbAssert(available < magic_number, "out of bounds error\n");

        functions->commands[available] = function_pointer;
        handle.u64 = available;

        bbDictionary_add(functions->command_dict, key, handle);

        return bbSuccess;

    default:
        bbAssert(0, "bad widget function type\n");
        return bbFail;
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

    case WidgetCommand:
        dict = functions->command_dict;
        break;
    }

    bbHandle handle;
    bbFlag flag = bbDictionary_lookup(dict,key,&handle);
    if (flag == bbSuccess) return handle.u64;
    return -1;
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

    case WidgetCommand:
        bbDictionary_lookup(functions->command_dict,key,&handle);
        *function = functions->commands[handle.u64];
        return bbSuccess;
    }
    *function = NULL;
    return bbNone;
}