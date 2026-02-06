#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>

int main(void)
{
    printf("Hello, World!\n");



    sfVideoMode mode;
    mode.width = 720;
    mode.height = 480;
    mode.bitsPerPixel = 32;

    sfRenderWindow* window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);

    sfSleep(sfSeconds(1.f));
    return 0;
}