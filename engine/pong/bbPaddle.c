#include "engine/pong/bbPaddle.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbString.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbPaddle_Init(bbPaddle* paddle,
                   bbScreenPoints position,
                   bbScreenPoints velocity,
                   char* key)
{
    paddle->velocity = velocity;
    paddle->position = position;
    bbStr_setStr(paddle->key, key, KEY_LENGTH);

    return bbSuccess;
}


bbFlag bbPaddle_Update(bbPaddle* paddle)
{
    bbScreenPoints new_position;
    new_position.x = paddle->position.x + paddle->velocity.x;
    new_position.y = paddle->position.y + paddle->velocity.y;

    bbScreenPointsRect rect;

    const I32 paddle_half_width = 11;
    const I32 paddle_half_height = 64;

    if (new_position.x < paddle_half_width*SCREEN_PPP || new_position.x > (720 - paddle_half_width)*SCREEN_PPP)
    {
        paddle->velocity.x = -paddle->velocity.x;
    new_position.x = paddle->position.x + paddle->velocity.x;

    }


    if (new_position.y < paddle_half_height*SCREEN_PPP || new_position.y > (480 - paddle_half_height)*SCREEN_PPP)
    {
        paddle->velocity.y = -paddle->velocity.y;
        new_position.y = paddle->position.y + paddle->velocity.y;

    }

    paddle->position.x = new_position.x;
    paddle->position.y = new_position.y;


    bbHandle ball_position;
    ball_position.i32x2.x = new_position.x;
    ball_position.i32x2.y = new_position.y;


    if (home.core.core.actual_time == home.core.core.simulation_time)
    bbUI_Inbox_SetWidgetPosition(&home.UI.inbox, paddle->key, ball_position);

    return bbSuccess;
}