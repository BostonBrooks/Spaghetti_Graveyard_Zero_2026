#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <pthread.h>
#include <math.h>

#include "engine/agents/bbAgentFunctions.h"
#include "engine/agents/bbAvoidables.h"
#include "engine/core/bbCoreDiscard.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/graphics/bbSprites.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/graphics/bbColours.h"
#include "engine/userinterface/bbInput.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/spawner/bbSpawner.h"
#include "engine/network/bbNetworkApp.h"
#include "games/game0/maps/skellychase/core/core_inputs.h"

#include <pthread.h>

pthread_barrier_t barrier1;

thread_local char* thread;
bbHome home;

U64 test_time = 0;

char test_string[KEY_LENGTH];

bbMapCoords testGoalPoint;

//test bbList_mapL()
bbFlag bbVInstruction_updateAgentsSquare_fn(bbCore* core, bbInstruction* instruction);

void* userinterface_thread(void* arg);
int main(void)
{
    thread = "MAIN";
    printf("Hello, World!\n");

    pthread_barrier_init(&barrier1, NULL, 2);

    //home.clock.clock_running = false;
    home.clock2.is_paused = true;
    home.clock2.is_running = false;
    home.UI.clock2_handle.clock_thread_index = 255;
    home.core.clock2_handle.clock_paused = true;

    home.core.viewpoint.i = 10000;
    home.core.viewpoint.j = 10000;
    home.core.viewpoint.k = 0;

    home.core.goalpoint.i = 10000;
    home.core.goalpoint.j = 10000;
    home.core.goalpoint.k = 0;





    bbCore_init(&home.core.core);

    bbCore_initVInstructions(&home.core.core);

    bbSquareCoords size; size.i = 12; size.j = 12; size.k = 0;
    bbGroundSurface_init(&home.ground_surface, size, "./maps/skellychase/graphics/HeightMap.bmp");
    bbAgentFunctions_init(&home.agents_app.functions);
    bbAgentFunctions_populate(&home.agents_app.functions);
    //bbAgents_init(&home.agents_app.agents);

    bbSpawner_init(&home.spawner, 696969);
    bbSpawner_populate(&home.spawner);



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

    bbAvoidables_new(&home.agents_app.avoidables, 12, 12);
    bbMoveables_init(&home.agents_app.movables);


    bbAgents_new(&home.agents_app.agents2, 12,12);


    bbEntities_init_core(&home.entities);

    bbSpawner_spawnCore(&home.spawner, "./maps/skellychase/spawner/spawner.csv");

    bbHandle no_handle;

    //This works, we have a bunch of skeletons going in circles!
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 11, 12,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 12, 13,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 13, 14,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 14, 15,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 15, 16,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 16, 17,bbInstructionSource_internal, no_handle);
    bbCoreInput_setGoalMoveable(&home.core.core,69696969, 17, 11,bbInstructionSource_internal, no_handle);
    bbCore_react(&home.core.core);


    pthread_barrier_wait(&barrier1);


    while (1)
    {


        bbCoreInput_updateAgentsSquare(&home.core.core, NULL,bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);


        bbCoreInput_updateAgents(&home.core.core, home.agents_app.agents2,
                                 bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);

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
                bbClock_handle_init(&home.clock2, &home.core.clock2_handle, 1, "MAIN");
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


            //bbMoveables_update(&home.agents_app.movables);
            //bbCoreInput_approachGoalpoint(&home.core.core);

            bbCoreInput_updateMoveables(&home.core.core, bbInstructionSource_input, no_handle);
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


    bbViewportApp_init(&home.viewport_app);

    bbUIApp_spawnWidgets(&home.UI);


    bbGroundSurface_setTexture(&home.ground_surface);

    home.UI.UI_time = 0;


    drawFuncClosure cl;
    cl.map_time = 0;
    cl.GUI_time = 0;
    cl.graphics = &home.UI.graphics;
    cl.target = home.UI.window;

    bbNetworkTime* network_time = (bbNetworkTime*)home.network.extra_data;

    bbMoveables_snapshot moveables_snapshot;

    U8 clock_index = 255;

    I32 counter = 0;


    bbEntities_init_graphics(&home.entities);
    bbSpawner_spawnGraphics(&home.spawner, "./maps/skellychase/spawner/spawner.csv");

    bbHere()
    bbMapCoords MC;
    MC.i = 10000;
    MC.j = 10000;
    MC.k = 1500;

    /*
    bbDrawable* sphere;
    bbDrawable_newPoint(&sphere, home.viewport_app.drawables,
                          &home.UI.graphics, MC);
    sphere->frames[0].handle.u64 = 623;*/


    pthread_barrier_wait(&barrier1);
bbHere()
    while (1)
    {

        //if (counter%360 == 0) bbClock_printQueueLengths(&home.clock2);
        counter++;

        bbInput_poll(&home.UI.input, home.UI.window);

        bbMouse_isOver(&home.UI.mouse, &home.UI.widgets);
        bbMouse_Update(&home.UI.mouse, &home.UI.widgets, &home.UI.graphics);

        bbUI_Inbox_check(&home.UI.inbox);

       /* // test bbScreenCoords_getMapCoords_k_fixed
        bbScreenPoints mouse_position = home.UI.mouse.position;
        bbViewportCoords viewport_points =
            bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, mouse_position);

        bbMapCoords vertex1, vertex2, vertex3;bbMapCoords mouseCoords;
        vertex1.i = 0;
        vertex1.j = 0;
        vertex1.k = 500;

        vertex2.i = 10000;
        vertex2.j = 0;
        vertex2.k = 500;

        vertex3.i = 0;
        vertex3.j = 10000;
        vertex3.k = 500;
        //mouseCoords =  bbViewportCoords_interpolateMapCoords (viewport_points, vertex1, vertex2, vertex3);
        //mouseCoords  = bbViewportCoords_getMapCoords_k_fixed (viewport_points, 500, &home.viewport_app.viewport);
        mouseCoords = bbViewportCoords_getMapCoords (viewport_points);
        //mouseCoords.k = 1000;
        bbDrawable_setLocation(sphere, home.viewport_app.drawables,mouseCoords);
//end test*/

        bbMoveables_copyBuffer(&home.agents_app.movables, &moveables_snapshot);
        bbUnits_consumeBuffer(home.viewport_app.units, home.viewport_app.unit_array,&moveables_snapshot);
        bbUIApp_draw(&home.UI);

        if (home.clock2.is_running){
            if (home.UI.clock2_handle.clock_thread_index == 255)
            {
                bbClock_handle_init(&home.clock2, &home.UI.clock2_handle, 0,"USER INTERFACE");
            }
            //may  not need this line:
            //bbClock_waitTick(&home.clock2,&home.UI.clock2_handle,home.UI.clock2_handle.map_tick+1);
            //bbClock_waitTick(&home.clock2,&home.UI.clock2_handle,0);
            home.UI.clock2_handle.map_tick = home.clock2.map_tick;

            test_time = home.UI.clock2_handle.map_tick;
            cl.map_time = home.UI.clock2_handle.map_tick;
            cl.GUI_time = home.UI.clock2_handle.server_tick;
        }


    }

}