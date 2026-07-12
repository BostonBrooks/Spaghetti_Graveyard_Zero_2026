#include <stdio.h>
#include "engine/data/CSFML.h"
#include <pthread.h>
#include <math.h>

#include "engine/core/bbCoreDiscard.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/graphics/bbSprites.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/data/bbHome.h"
#include "engine/graphics/bbColours.h"
#include "engine/userinterface/bbInput.h"
#include "engine/userinterface/bbMouse.h"

#include "engine/network/bbNetworkApp.h"
#include "games/game0/maps/pong/code/bbBall.h"
thread_local char* thread;
bbHome home;

U64 test_time = 0;

char test_string[KEY_LENGTH];

void* userinterface_thread(void* arg);
int main(void)
{
    thread = "MAIN";
    printf("Hello, World!\n");

    //home.clock.clock_running = false;
    home.clock2.is_paused = true;
    home.clock2.is_running = false;
    home.UI.clock2_handle.clock_thread_index = 255;
    home.core.clock2_handle.clock_paused = true;

    bbCore_init(&home.core.core);
    bbCore_initVInstructions(&home.core.core);

    pthread_t graphics_pthread;
    pthread_create(&graphics_pthread, NULL, userinterface_thread, NULL);

    bbNetworkApp_init(&home.network);

    char address[64] = "127.0.0.1";
    char port[64] = "1701";
    bbNetworkApp_connect(&home.network, address, port);
    home.network_time = (bbNetworkTime*)home.network.extra_data;

    //U64 core_time = 0;
    U32 collision = 0;
    bool clock_handle_init = false;
    bool once2 = false;
    bool once = false;


    {
        bbScreenPoints position = {360*SCREEN_PPP,240*SCREEN_PPP};
        bbScreenPoints velocity = {6*SCREEN_PPP,3*SCREEN_PPP};

        char BALLN[KEY_LENGTH];
        velocity.y = -N_BALLS / 2;

        for (int i = 0; i < N_BALLS; i++)
        {
            sprintf(BALLN, "BALL%d", i);
            bbBall_Init(&home.core.balls[i],position,velocity,BALLN);
            velocity.y++;
        }


        bbScreenPoints position2 = {50*SCREEN_PPP,240*SCREEN_PPP};
        bbScreenPoints velocity2 = {0*SCREEN_PPP,7*SCREEN_PPP};
        bbPaddle_Init(&home.core.paddle1, position2,velocity2,"PADDLE1");


        bbScreenPoints position3 = {670*SCREEN_PPP,240*SCREEN_PPP};
        bbScreenPoints velocity3 = {0*SCREEN_PPP,6*SCREEN_PPP};
        bbPaddle_Init(&home.core.paddle2, position3,velocity3,"PADDLE2");
    }
    while (1)
    {

        if (home.network.send_ready && home.network.receive_ready)
        {
            bbNetworkTime_ping(&home.network);
            bbNetworkTime_updateTimeDiff(home.network.extra_data);
        }


        if (home.network_time->timeCalibrated && once == false)
        {
            once = true;
            //bbClock_init(&home.clock, home.network_time);
            bbClock_init(&home.clock2, home.network_time);
        }

        //if (home.clock.clock_running == true && home.clock2.is_running == true)
        if ( home.clock2.is_running == true)
        {
            if (clock_handle_init == false)
            {
                //bbClock_getOutboxIndex(&home.clock, &clock_index);
                //U64 time;
                //bbClock_getTick(&home.clock, &time);
                //core_time = time;
                bbClock_handle_init(&home.clock2, &home.core.clock2_handle, 1,"MAIN");
                clock_handle_init = true;
                //core_time = home.clock2.map_tick;
            }
            U64 ticks_per_frame = 6;
            U64 new_map_tick = home.core.clock2_handle.map_tick
                 - home.core.clock2_handle.map_tick % ticks_per_frame + ticks_per_frame;
            bbClock_waitTick(&home.clock2,&home.core.clock2_handle,new_map_tick);


        } else
        {

            //core_time += 3;
        }
        //home.core.core.simulation_time = home.core.clock2_handle.map_tick;
        home.core.core.actual_time = home.core.clock2_handle.map_tick;

        bbCoreInput_setTime(&home.core.core,  home.core.clock2_handle.map_tick, bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);

        bbCore_checkLocalMessages(&home.core.core);




        if (home.network.send_ready && home.network.receive_ready)
                 bbNetworkApp_checkInbox(&home.network);



        if (home.core.clock2_handle.clock_paused == false)
        {
            bbCoreInput_checkActions(&home.core.core,
                home.core.core.actual_time,
                bbInstructionSource_input, no_handle );


            for (int i = 0; i < N_BALLS; i++)
            {
                bbCoreInput_updateBall(&home.core.core, &home.core.balls[i], bbInstructionSource_input, no_handle);
            }


            bbCoreInput_updatePaddle(&home.core.core, &home.core.paddle1, bbInstructionSource_input, no_handle);
            bbCoreInput_updatePaddle(&home.core.core, &home.core.paddle2, bbInstructionSource_input, no_handle);
            bbCore_react(&home.core.core);



        }

        if (home.clock2.is_paused == false)
        {
        }

        bbCore_react(&home.core.core);

        bbCoreDiscard(&home.core.core, home.core.clock2_handle.map_tick-180);
        //bbActions_react(&home.core.core, core_time);

        if (clock_handle_init == false) sfSleep(sfSeconds(1.f/60.f));
    }

    bbFlag flag = bbSuccess;
    bbFlag_print(flag)

    pthread_join(graphics_pthread, NULL);
}

void* userinterface_thread(void* arg)
{
    thread = "USER INTERFACE";

    bbUIApp_init(&home.UI);

    bbWidget* root;
    bbWidget_newLayoutPong(&root, &home.UI.graphics, &home.UI.widgets, NULL);

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "NETPAUSE_BUTTON",
                     "LAYOUT",
                     "(UN)PAUSE",
                     (bbScreenPoints){6*SCREEN_PPP,6*SCREEN_PPP});

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "CLOCK",
                     "LAYOUT",
                     "CLOCK",
                     (bbScreenPoints){150*SCREEN_PPP,80*SCREEN_PPP});


    bbWidget* ball;

    char BALLN[KEY_LENGTH];

    for (int i = 0; i < N_BALLS; i++)
    {
        sprintf(BALLN, "BALL%d", i);
        bbWidget_constructor(&ball,
                         &home.UI.widgets,
                         "BALL",
                         "LAYOUT",
                         BALLN,
                         (bbScreenPoints){100*SCREEN_PPP,100*SCREEN_PPP});
    }

    bbWidget_constructor(&ball,
                 &home.UI.widgets,
                 "PADDLE",
                 "LAYOUT",
                 "PADDLE1",
                 (bbScreenPoints){50*SCREEN_PPP,150*SCREEN_PPP});


    bbWidget_constructor(&ball,
                 &home.UI.widgets,
                 "PADDLE",
                 "LAYOUT",
                 "PADDLE2",
                 (bbScreenPoints){670*SCREEN_PPP,150*SCREEN_PPP});

    home.UI.UI_time = 0;


    drawFuncClosure cl;
    cl.map_time = 0;
    cl.GUI_time = 0;
    cl.graphics = &home.UI.graphics;
    cl.target = home.UI.window;

    bbNetworkTime* network_time = (bbNetworkTime*)home.network.extra_data;

    U8 clock_index = 255;
    while (1)
    {



        bbInput_poll(&home.UI.input, home.UI.window);

        bbMouse_isOver(&home.UI.mouse, &home.UI.widgets);
        bbMouse_Update(&home.UI.mouse, &home.UI.widgets, &home.UI.graphics);

        bbUI_Inbox_check(&home.UI.inbox);
        sfRenderWindow_clear(home.UI.window, bbLight);
        bbWidgets_draw(&home.UI.widgets, &cl);
        bbMouse_Draw(&home.UI.mouse,&home.UI.widgets, &home.UI.graphics, home.UI.window);
        sfRenderWindow_display(home.UI.window);

        if (home.clock2.is_running){
            if (home.UI.clock2_handle.clock_thread_index == 255)
            {
                bbClock_handle_init(&home.clock2, &home.UI.clock2_handle, 1, "USER INTERFACE");
            }
            //may  not need this line:
            bbClock_waitTick(&home.clock2,&home.UI.clock2_handle,home.UI.clock2_handle.map_tick+1);


            test_time = home.UI.clock2_handle.map_tick;
            cl.map_time = home.UI.clock2_handle.map_tick;
            cl.GUI_time = home.UI.clock2_handle.server_tick;
        }


    }

}