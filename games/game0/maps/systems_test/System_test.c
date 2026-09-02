#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "core/instructions.h"
#include "core/core_inbox.h"
#include "core/core_inputs.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"
#include "engine/data/CSFML.h"
#include "engine/core/bbCoreDiscard.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"
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
#include "games/game0/maps/systems_test/core/core_inputs.h"
#include "games/game0/maps/systems_test/AI_system/ai_instructions.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/test_string/bbTestString.h"
#include "engine/ECS/ECS_instructions.h"


#include "engine/ECS/graphics_system/bbGraphicsSystem.h"
#include "moveables/moveables.h"

pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;
bbHome home;

U64 test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];

bbMapCoords testGoalPoint;
//bool interp_positions;

void* userinterface_thread(void* arg);
int main(void)
{
    thread = "MAIN";
    debug_off = false;
    printf("Hello, World!\n");

    pthread_barrier_init(&barrier1, NULL, 2);

    home.clock2.is_paused = true;
    home.clock2.is_running = false;
    home.UI.clock2_handle.clock_thread_index = 255;
    home.core.clock2_handle.clock_paused = true;

    null_handle.u64 = 0;

    home.core.viewpoint.i = 10000;
    home.core.viewpoint.j = 10000;
    home.core.viewpoint.k = 0;

    home.core.goalpoint.i = 10000;
    home.core.goalpoint.j = 10000;
    home.core.goalpoint.k = 0;


    bbPerformance_init(&home.performance);
    bbCore_init(&home.core.core);
    bbCore_initVInstructions(&home.core.core);
    bbCore_initInboxMessages(&home.core.core);
    bbCore_initActions(&home.core.core);
    bbCore_initDiscard(&home.core.core);

    bbCoreInbox_TestMessage(&home.core.core);
    bbCore_checkInbox(&home.core.core);

    bbECS_new(&home.core.core.ECS, bbECS_numSystems);

    home.ECS.ECS = home.core.core.ECS;
    bbServerEntities_init(&home.ECS.server_entities,home.core.core.ECS);
    bbGraphicsSystem_init(&home.ECS.graphics_system,home.core.core.ECS);
    bbMoveables_init(&home.ECS.moveables,home.core.core.ECS);
    bbAvoidables_new(&home.ECS.avoidables, 12, 12);

    bbAI_System_init(&home.ECS.AI_system,home.core.core.ECS);
    bbAI_Functions_init(&home.ECS.AI_system.functions);
    bbAI_Functions_populate(&home.ECS.AI_system.functions);

    bbHandle server_handle;
    server_handle.bloated.index = 193;
    server_handle.bloated.collision = 193;
    bbECS_entity* entity;
    // bbCoreInput_spawnServerEntity(&home.core.core,
    //                                 "TEST ENTITY",
    //                                 server_handle,
    //                                 bbInstructionSource_input,
    //                                 no_handle);


    bbCore_react(&home.core.core);

    bbHandle moveable_handle;
    bbMapCoords moveable_position;
    moveable_position.i = 10100;
    moveable_position.j = 10000;
    moveable_position.k = 0;




    // bbCoreSynchronous_spawnTestMoveable(&home.core.core,
    //                                        home.ECS.ECS->list.list.head,
    //                                        &moveable_handle,
    //                                        moveable_position,
    //                                        bbInstructionSource_norewind,
    //                                        no_handle);
    bbCore_react(&home.core.core);
    bbSquareCoords size; size.i = 12; size.j = 12; size.k = 0;
    bbGroundSurface_init(&home.ground_surface, size, "./maps/systems_test/graphics/HeightMap.bmp");
    //
    // bbSpawner_init(&home.spawner, 69, 193);
    // bbSpawner_populate(&home.spawner);

    //bbCoreInput_updateMoveables(&home.core.core,bbInstructionSource_internal, no_handle);
    //bbCore_react(&home.core.core);

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


    bbEntitySpawner_init(&home.ECS.spawner);
    bbEntitySpawner_populate(&home.ECS.spawner);


    pthread_barrier_wait(&barrier1);

    bbEntitySpawner_spawnFile(&home.ECS.spawner, "maps/systems_test/entity_spawner/spawner.csv");


    //bbMoveable* test_moveable = &home.ECS.moveables.moveables[0];

    //bbECS_entity* test_entity;

    //bbComponent_mapComponent(&home.ECS.ECS, bbECS_Moveables,(bbComponent*)test_moveable,bbECS_ECS,NULL,(bbComponent**)&test_entity);

    //bbMoveable* test_moveable2;
    //bbHandle test_handle = test_moveable->component.entity_handle;
    //bbHandle_mapComponent(&home.ECS.ECS, bbECS_ECS,test_handle,bbECS_Moveables,NULL,(bbComponent**)&test_moveable2);

    //bbDebug("moveable = %p, moveable1 = %p\n", test_moveable, test_moveable2);


    bbMapCoords MC;
    MC.i = 10100; MC.j = 10000; MC.k = 0;

    bbHandle entity_handle = home.ECS.ECS->list.list.head;
    //
    // bbCoreInput_spawnGraphicsComponent(&home.core.core,
    //                                    MC,
    //                                    entity_handle,
    //                                    moveable_handle,
    //                                    bbInstructionSource_norewind,
    //                                    no_handle);
    bbCore_react(&home.core.core);
    fflush(stdout);

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

        bbPerformance_end(&home.performance);
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
                        bbCore_react(&home.core.core);



        } else
        {

            //core_time += 3;
        }
        bbPerformance_start(&home.performance);
        //home.core.core.simulation_time = home.core.clock2_handle.map_tick;

        if (home.core.core.actual_time != home.core.clock2_handle.map_tick)
        {
            home.core.core.actual_time = home.core.clock2_handle.map_tick;

            bbCoreInput_setTime(&home.core.core,  home.core.core.actual_time, bbInstructionSource_input, no_handle);
            bbCore_react(&home.core.core);
        }
        bbCore_checkInbox(&home.core.core);
        bbCore_react(&home.core.core);



        if (home.network.send_ready && home.network.receive_ready)
        {
            bbNetworkApp_checkInbox(&home.network);
            bbCore_react(&home.core.core);
        }



        if (home.core.clock2_handle.clock_paused == false)
        {

            bbCoreInput_checkActions(&home.core.core,
                home.core.core.actual_time,
                bbInstructionSource_input, no_handle );
            bbCore_react(&home.core.core);

            //have not implemented moveables!
            bbCoreInput_updateMoveables(&home.core.core,bbInstructionSource_input, no_handle );
            bbCore_react(&home.core.core);

            //bbMovables_update(&home.agents_app.movables);
            //bbCoreInput_approachGoalpoint(&home.core.core);

            //bbCoreInput_updateMovables(&home.core.core, bbInstructionSource_input, no_handle);

            //bbCore_react(&home.core.core);
            //bbCoreInput_updateAgentsSquare(&home.core.core, NULL,bbInstructionSource_input, no_handle);
            //bbCore_react(&home.core.core);

            //updateAI is a bbCoreSynchronous function. We dont want to call bbCore_react() inside updateAI
            bbCS_updateAI(&home.core.core, bbInstructionSource_input, no_handle);
            bbCore_react(&home.core.core);

            //bbCoreInput_updateAgents(&home.core.core, home.agents_app.agents,
            //                         bbInstructionSource_input, no_handle);
            //bbCore_react(&home.core.core);
        }

        if (home.clock2.is_paused == false)
        {
        }

        bbCore_react(&home.core.core);

        if (home.core.clock2_handle.map_tick > 300)
        {
            bbCoreDiscard(&home.core.core, home.core.clock2_handle.map_tick-300);
        }
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
    debug_off = true;

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
    bbViewportSpawner_init(&home.viewport_app.viewport_spawner);
    bbViewportSpawner_populate(&home.viewport_app.viewport_spawner);
    //bbEntities_init_graphics(&home.agents_app.entities);
    //bbSpawner_spawnGraphics(&home.spawner, "./maps/systems_test/spawner/spawner.csv");

    // bbHere()
    // bbMapCoords MC;
    // MC.i = 10000;
    // MC.j = 10000;
    // MC.k = 1500;
    //
    // MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
    //
    // bbDrawable* drawable;
    // bbDrawable_newTree(&drawable, home.viewport_app.drawables, &home.UI.graphics,MC);
    // bbMapIcon* mapicon;
    // bbMapIcon_new(&mapicon, home.viewport_app.mapIcons,&home.UI.graphics, MC);
    //
    // I32 zombie_int = bbViewportSpawner_lookupInt(&home.viewport_app.viewport_spawner,
    //                             "ZOMBIE");
    //
    // MC.i = 10500;
    // MC.j = 10000;
    // MC.k = 1500;
    //
    // MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
    //
    // bbUI_Inbox_NewDrawable(&home.UI.inbox, zombie_int, MC, no_handle, no_handle);

    pthread_barrier_wait(&barrier1);
bbHere()
    while (1)
    {

        counter++;
        bbPerformance_newFrame(&home.performance);

        bbInput_poll(&home.UI.input, home.UI.window);

        bbMouse_isOver(&home.UI.mouse, &home.UI.widgets);
        bbMouse_Update(&home.UI.mouse, &home.UI.widgets, &home.UI.graphics);

        bbUI_Inbox_check(&home.UI.inbox);

        bbMoveables_copyBuffer(&home.ECS.moveables, &moveables_snapshot);
        bbUnits_consumeBuffer(home.viewport_app.units, home.viewport_app.entity_units, &moveables_snapshot);

        bbUIApp_draw(&home.UI);






        sfRenderWindow_display(home.UI.window);

        if (home.clock2.is_running){
            if (home.UI.clock2_handle.clock_thread_index == 255)
            {
                bbClock_handle_init(&home.clock2, &home.UI.clock2_handle, 0,"USER INTERFACE");
            }

            if (home.clock2.map_tick >= 6)
            {
                home.UI.clock2_handle.map_tick = home.clock2.map_tick-6;
            }
            test_time = home.UI.clock2_handle.map_tick;
            cl.map_time = home.UI.clock2_handle.map_tick;
            cl.GUI_time = home.UI.clock2_handle.server_tick;
        }


    }

}