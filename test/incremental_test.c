#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <pthread.h>

#include "engine/graphics/bbSprites.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/data/bbHome.h"
#include "engine/userinterface/bbInput.h"

thread_local char* thread;
bbHome home;

void* graphics_thread(void* arg);
int main(void)
{
    thread = "MAIN";
    printf("Hello, World!\n");

    pthread_t graphics_pthread;
    pthread_create(&graphics_pthread, NULL, graphics_thread, NULL);

    bbFlag flag = bbSuccess;
    bbFlag_print(flag)

    pthread_join(graphics_pthread, NULL);
}

void* graphics_thread(void* arg)
{
    thread = "GRAPHICS";
    sfTexture* splash_texture = sfTexture_createFromFile("./graphics/Splash.png", NULL);
    sfSprite* splash_sprite = sfSprite_create();
    sfSprite_setTexture(splash_sprite, splash_texture, sfTrue);

    sfVideoMode mode;
    mode.width = 720;
    mode.height = 480;
    mode.bitsPerPixel = 32;

    sfRenderWindow* window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    sfRenderWindow_drawSprite(window, splash_sprite, NULL);
    sfRenderWindow_display(window);


    bbGraphicsApp_init(&home.UI.graphics);


    bbWidgets_init(&home.UI.widgets);


    bbInput input;
    bbInput_init(&input, window, NULL, &home.UI.widgets);

    bbWidget* root;
    bbWidget_newLayout(&root, &home.UI.graphics, &home.UI.widgets, NULL);



    bbWidget_constructor(NULL,
                             &home.UI.widgets,
                             "KITTY",
                             "LAYOUT",
                             "KITTY",
                             (bbScreenPoints){200*SCREEN_PPP,200*SCREEN_PPP});

    bbWidget_constructor(&home.UI.widgets.selected_textbox,
                         &home.UI.widgets,
                         "TEXTBOX",
                         "LAYOUT",
                         "TEXTBOX",
                         (bbScreenPoints){200*SCREEN_PPP,200*SCREEN_PPP});


    drawFuncClosure cl;
    cl.map_time = 0;
    cl.GUI_time = 0;
    cl.graphics = &home.UI.graphics;
    cl.target = window;

    I32 gui_time = 0;
    while (1)
    {
        cl.GUI_time = gui_time++;

        bbInput_poll(&input, window);

        sfRenderWindow_clear(window, sfMagenta);
        bbWidgets_draw(&home.UI.widgets, &cl);
        sfRenderWindow_display(window);
    }

}