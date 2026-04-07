#ifndef BB_BALL_H
#define BB_BALL_H

#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"

#define N_BALLS 4

typedef struct
{
    bbScreenPoints position;
    bbScreenPoints velocity;
    char key[KEY_LENGTH];
    bool ready;
} bbBall;

///TODO:
///init ball after graphics thread is ready,
///send message to graphics thread to create ball with key
bbFlag bbBall_Init(bbBall* ball,
                    bbScreenPoints position,
                    bbScreenPoints velocity,
                    char* key);

///update ball if ball is ready
bbFlag bbBall_Update(bbBall* ball);

#endif //BB_BALL_H