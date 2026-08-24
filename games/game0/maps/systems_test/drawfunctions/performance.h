#include "engine/data/CSFML.h"
#include "engine/geometry/bbCoordinates.h"


bbFlag bbDF_widgetPerformance(void* drawable, void* frameDescriptor, void* cl)
{
    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;


    static sfRectangleShape* framerate_rect = 0;
    static sfRectangleShape* uptime_rect = 0;
    static sfRectangleShape* downtime_rect = 0;

    if(framerate_rect == 0)
    {
        sfVector2f vec2;

        framerate_rect = sfRectangleShape_create();
        sfRectangleShape_setFillColor(framerate_rect, sfGreen);


        uptime_rect = sfRectangleShape_create();
        sfRectangleShape_setFillColor(uptime_rect, sfRed);

        sfRectangleShape_setSize(uptime_rect, vec2);

        downtime_rect = sfRectangleShape_create();
        sfRectangleShape_setFillColor(downtime_rect, sfBlue);

        sfRectangleShape_setSize(downtime_rect, vec2);

    }

    bbScreenPoints SP;
    sfVector2f position;
    sfVector2f size;
    SP.x = widget->rect.left;
    SP.y = widget->rect.top;
    sfVector2f start_position = bbScreenPoints_getV2f(SP);


    float framerate = home.performance.framerate / 60.f / 2.f;
    float height = framerate * widget->rect.height;
    size.x = 12;
    size.y = height;
    position = start_position;
    position.y -= height;
    sfRectangleShape_setSize(framerate_rect, size);
    sfRectangleShape_setPosition(framerate_rect, position);
    sfRenderWindow_drawRectangleShape(closure->target, framerate_rect, NULL);


    height = home.performance.uptime * widget->rect.height / 2.f;
    size.x = 12;
    size.y = height;
    position = start_position;
    position.y -= height;
    position.x += 12;
    sfRectangleShape_setSize(uptime_rect, size);
    sfRectangleShape_setPosition(uptime_rect, position);
    sfRenderWindow_drawRectangleShape(closure->target, uptime_rect, NULL);


    height = home.performance.downtime * widget->rect.height / 2.f;
    size.x = 12;
    size.y = height;
    position = start_position;
    position.y -= height;
    position.x += 24;
    sfRectangleShape_setSize(downtime_rect, size);
    sfRectangleShape_setPosition(downtime_rect, position);
    sfRenderWindow_drawRectangleShape(closure->target, downtime_rect, NULL);

    return bbSuccess;
}
