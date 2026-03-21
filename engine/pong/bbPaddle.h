#ifndef BB_PADDLE_H
#define BB_PADDLE_H

#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"

typedef struct
{
    bbScreenPoints position;
    bbScreenPoints velocity;
    char key[KEY_LENGTH];
    bool ready;
} bbPaddle;

///TODO:
///init paddle after graphics thread is ready,
///send message to graphics thread to create paddle with key
bbFlag bbPaddle_Init(bbPaddle* paddle,
                    bbScreenPoints position,
                    bbScreenPoints velocity,
                    char* key);

///update paddle if paddle is ready
bbFlag bbPaddle_Update(bbPaddle* ball);

#endif //BB_PADDLE_H