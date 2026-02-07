#ifndef BB_CALLBACK_H
#define BB_CALLBACK_H

#include <stddef.h>

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef bbFlag bbCallbackFunction(void* callback, bbHandle handle);

typedef struct bbCallback
{
    bbCallbackFunction* function;
    bbHandle args;
    struct bbCallback* daisyChain;
}  bbCallback;

static bbFlag bbCallback_execute(bbCallback* callback, bbHandle handle)
{
    return callback->function(callback, handle);
}

static bbFlag bbCallback_init(bbCallback* self, bbCallbackFunction* function, bbHandle args)
{
    self->function = function;
    self->args = args;
    self->daisyChain = NULL;
    return bbSuccess;
}

#endif //BB_CALLBACK_H