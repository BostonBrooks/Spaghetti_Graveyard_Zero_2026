#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <pthread.h>

#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/graphics/bbSprites.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/data/bbHome.h"
#include "engine/interthread/bbClock.h"
#include "engine/userinterface/bbInput.h"
#include "engine/userinterface/bbMouse.h"

#include "engine/network/bbNetworkApp.h"
thread_local char* thread;
bbHome home;

U64 test_time = 0;

void* userinterface_thread(void* arg);
int main(void)
{
    thread = "MAIN";
    printf("Hello, World!\n");

    home.clock.clock_running = false;

    bbCore_init(&home.core.core);

    pthread_t graphics_pthread;
    pthread_create(&graphics_pthread, NULL, userinterface_thread, NULL);

    bbNetworkApp_init(&home.network);

    char address[64] = "127.0.0.1";
    char port[64] = "1701";
    bbNetworkApp_connect(&home.network, address, port);
    home.network_time = (bbNetworkTime*)home.network.extra_data;



    bool once = false;
    while (1)
    {
        if (home.network.send_ready && home.network.receive_ready)
        {
            bbNetworkTime_ping(&home.network);
            bbNetworkApp_checkInbox(&home.network);
            bbNetworkTime_updateTimeDiff(home.network.extra_data);
        }


        if (home.network_time->timeCalibrated && once == false)
        {
            once = true;
            bbClock_init(&home.clock, home.network_time);
        }
        //if (test_time%60 == 0)
        //{
        //
        //   bbCore_printInteger(&home.core.core, 69, true);
        //    bbCore_printString(&home.core.core, "69", true);
        //}

        bbCore_react(&home.core.core);
        bbCore_checkLocalMessages(&home.core.core);

        sfSleep(sfSeconds(1.f/60.f));
    }

    bbFlag flag = bbSuccess;
    bbFlag_print(flag)

    pthread_join(graphics_pthread, NULL);
}

void* userinterface_thread(void* arg)
{
    thread = "USER INTERFACE";
    sfTexture* splash_texture = sfTexture_createFromFile("./graphics/Splash.png", NULL);
    sfSprite* splash_sprite = sfSprite_create();
    sfSprite_setTexture(splash_sprite, splash_texture, sfTrue);

    sfVideoMode mode;
    mode.width = 720;
    mode.height = 480;
    mode.bitsPerPixel = 32;

    sfRenderWindow* window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);
    sfRenderWindow_setMouseCursorVisible(window, sfFalse);
    sfRenderWindow_setFramerateLimit(window, 60);
    sfRenderWindow_drawSprite(window, splash_sprite, NULL);
    sfRenderWindow_display(window);


    bbGraphicsApp_init(&home.UI.graphics);


    bbWidgets_init(&home.UI.widgets);

    bbMouse mouse;
    bbMouse_Init(&mouse, &home.UI.widgets, &home.UI.graphics);

    bbMouseFunctions_init(&mouse.functions);
    bbMouseFunctions_populate(&mouse.functions);

    bbInput input;
    bbInput_init(&input, window, &mouse, &home.UI.widgets);

    bbWidget* root;
    bbWidget_newLayout(&root, &home.UI.graphics, &home.UI.widgets, NULL);



    bbWidget_constructor(NULL,
                             &home.UI.widgets,
                             "KITTY",
                             "LAYOUT",
                             "KITTY",
                             (bbScreenPoints){200*SCREEN_PPP,200*SCREEN_PPP});

    bbWidget_constructor(NULL,
                         &home.UI.widgets,
                         "BUTTON",
                         "LAYOUT",
                         "BUTTON",
                         (bbScreenPoints){300*SCREEN_PPP,200*SCREEN_PPP});


    bbWidget_constructor(NULL,
                         &home.UI.widgets,
                         "CLOCK",
                         "LAYOUT",
                         "CLOCK",
                         (bbScreenPoints){100*SCREEN_PPP,100*SCREEN_PPP});

    bbWidget_constructor(NULL,
                         &home.UI.widgets,
                         "GAME",
                         "LAYOUT",
                         "GAME",
                         (bbScreenPoints){0*SCREEN_PPP,0*SCREEN_PPP});

    drawFuncClosure cl;
    cl.map_time = 0;
    cl.GUI_time = 0;
    cl.graphics = &home.UI.graphics;
    cl.target = window;

    bbNetworkTime* network_time = (bbNetworkTime*)home.network.extra_data;

    U8 clock_index = 255;
    while (1)
    {
        test_time = cl.GUI_time++;

        bbInput_poll(&input, window);

        bbMouse_isOver(&mouse, &home.UI.widgets);
        bbMouse_Update(&mouse, &home.UI.widgets, &home.UI.graphics);

        sfRenderWindow_clear(window, sfMagenta);
        bbWidgets_draw(&home.UI.widgets, &cl);
        bbMouse_Draw(&mouse,&home.UI.widgets, &home.UI.graphics, window);
        sfRenderWindow_display(window);

        if (network_time->timeCalibrated == true)
        {
            if (clock_index == 255)
            {
                bbClock_getOutboxIndex(&home.clock, &clock_index);
                sfRenderWindow_setFramerateLimit(window, 0);
                U64 time;
                bbClock_getTick(&home.clock, &time);
                cl.map_time = time;
            }
            bbClock_waitTick(&home.clock,  ++(cl.map_time), clock_index);
        }


    }

}