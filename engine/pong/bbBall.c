#include "engine/pong/bbBall.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbString.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbBall_Init(bbBall* ball,
                   bbScreenPoints position,
                   bbScreenPoints velocity,
                   char* key)
{
    ball->velocity = velocity;
    ball->position = position;
    bbStr_setStr(ball->key, key, KEY_LENGTH);

    return bbSuccess;
}


bbFlag bbBall_Update(bbBall* ball)
{
    bbScreenPoints new_position;
    new_position.x = ball->position.x + ball->velocity.x;
    new_position.y = ball->position.y + ball->velocity.y;

    bbScreenPointsRect rect;

    const I32 ball_size = 13;

    if (new_position.x < ball_size*SCREEN_PPP || new_position.x > (720 - ball_size)*SCREEN_PPP)
    {
        ball->velocity.x = -ball->velocity.x;
    new_position.x = ball->position.x + ball->velocity.x;

    }


    if (new_position.y < ball_size*SCREEN_PPP || new_position.y > (480 - ball_size)*SCREEN_PPP)
    {
        ball->velocity.y = -ball->velocity.y;
        new_position.y = ball->position.y + ball->velocity.y;

    }

    ball->position.x = new_position.x;
    ball->position.y = new_position.y;


    bbHandle ball_position;
    ball_position.i32x2.x = new_position.x;
    ball_position.i32x2.y = new_position.y;
    bbUI_Inbox_SetWidgetPosition(&home.UI.inbox, "BALL", ball_position);

    return bbSuccess;
}