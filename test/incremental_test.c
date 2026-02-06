#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <pthread.h>

#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"

thread_local char* thread;

void* graphics_thread(void* arg);
int main(void)
{
    printf("Hello, World!\n");

    pthread_t graphics_pthread;
    pthread_create(&graphics_pthread, NULL, graphics_thread, NULL);

    bbFlag flag = bbSuccess;
    bbFlag_print(flag)


    sfSleep(sfSeconds(5.f));

    return 0;
}

void* graphics_thread(void* arg)
{
    sfTexture* splash_texture = sfTexture_createFromFile("./graphics/Splash.png", NULL);
    sfSprite* splash_sprite = sfSprite_create();
    sfSprite_setTexture(splash_sprite, splash_texture, sfTrue);

    sfVideoMode mode;
    mode.width = 720;
    mode.height = 480;
    mode.bitsPerPixel = 32;

    sfRenderWindow* window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);
    sfRenderWindow_drawSprite(window, splash_sprite, NULL);
    sfRenderWindow_display(window);

    sfSleep(sfSeconds(5.f));
    return 0;
}