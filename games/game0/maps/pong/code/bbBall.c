#include "games/game0/maps/pong/code/bbBall.h"

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
    const I32 paddle_half_width = 0;
    const I32 paddle_half_height = 64;

    bool A = new_position.x < ball_size*SCREEN_PPP;
    bool B = new_position.x > (720 - ball_size)*SCREEN_PPP;

    bbScreenPoints paddle_position = home.core.paddle1.position;
    I32 horizontal_difference = new_position.x - paddle_position.x;
    I32 vertical_difference = new_position.y - paddle_position.y;

    if (horizontal_difference < 0) { horizontal_difference = -horizontal_difference; }
    if (vertical_difference < 0 ) { vertical_difference = -vertical_difference; }

    bool C = horizontal_difference < (paddle_half_width + ball_size)*SCREEN_PPP;
    bool D = vertical_difference < (paddle_half_height + ball_size)*SCREEN_PPP;

    paddle_position = home.core.paddle2.position;
    horizontal_difference = new_position.x - paddle_position.x;
    vertical_difference = new_position.y - paddle_position.y;

    if (horizontal_difference < 0) { horizontal_difference = -horizontal_difference; }
    if (vertical_difference < 0 ) { vertical_difference = -vertical_difference; }

    bool E = horizontal_difference < (paddle_half_width + ball_size)*SCREEN_PPP;
    bool F = vertical_difference < (paddle_half_height + ball_size)*SCREEN_PPP;

    if (A || B || (C && D) || (E && F))
    {
        ball->velocity.x = -ball->velocity.x;
    new_position.x = ball->position.x + ball->velocity.x;

    }

    if (C && D)
    {
        paddle_position = home.core.paddle1.position;
        ball->velocity.y += (new_position.y - paddle_position.y) / 8;
        ball->velocity.y -= ball->velocity.y / 12;
    }

    if (E && F)
    {
        paddle_position = home.core.paddle2.position;
        ball->velocity.y += (new_position.y - paddle_position.y) / 8;
        ball->velocity.y -= ball->velocity.y / 12;
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

    if (home.core.core.actual_time == home.core.core.simulation_time)
        bbUI_Inbox_SetWidgetPosition(&home.UI.inbox, ball->key, ball_position);

    return bbSuccess;
}