#include "engine/data/bbHome.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewport.h"
#include "engine/logic/bbBubbleSort.h"

typedef struct
{
    I32 row_N;
    I32 column_M;

    bbMapCoords delta_coords;

} unit_position;

static bbFlag compare_closeness(void* one, void* two)
{
    unit_position* position_one = one;
    unit_position* position_two = two;

    I32 foo = position_two->delta_coords.i - position_one->delta_coords.i
              - position_two->delta_coords.j + position_one->delta_coords.j;

    if(foo < 0) return bbSuccess;
    return bbFail;
}

bbFlag bbDF_unitGroup(void* Drawable, void* frameDescriptor, void* cl)
{bbHere()
    bbDrawable* drawable = Drawable;
    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    bbComposition* composition = graphics->compositions->compositions[self_frame->handle.u64];
    bbFrame* input_frame = &composition->frame[0];
    bbAssert(input_frame->drawfunction >= 0, "bad draw function\n");

    bbDebug("drawfunction = %d\n", input_frame->drawfunction);
    float theta = drawable->rotation;
    float spacing = POINTS_PER_TILE;

    unit_position pos[12];

    bbMapCoords delta_coords, new_coords;

    float c_theta = cos(theta);
    float s_theta = sin(theta);

    I32 row_N, column_M;
    for (I32 i = 0; i < 12; i++)
    {
        row_N = i % 3;
        column_M = i / 3;

        delta_coords.i = (1.5-column_M)*spacing*c_theta - (-1+row_N)*spacing*s_theta;
        delta_coords.j = -(1.5-column_M)*spacing*s_theta - (-1+row_N)*spacing*c_theta;
        delta_coords.k = 0;

        pos[i].row_N = row_N;
        pos[i].column_M = column_M;
        pos[i].delta_coords = delta_coords;

    }

    bbBubbleSort((U8*)pos, 12, sizeof(unit_position), compare_closeness);

    for (I32 i = 0; i < 12; i++)
    {
        row_N = pos[i].row_N;
        column_M = pos[i].column_M;

        bbDebug("row_N = %d, column_M = %d\n", row_N, column_M);

        delta_coords = pos[i].delta_coords;
        delta_coords.k = 0;

        new_coords.i = drawable->coords.i + delta_coords.i;
        new_coords.j = drawable->coords.j + delta_coords.j;
        new_coords.k = 0;

        new_coords.k = bbMapCoords_getElevation(&home.ground_surface, new_coords);

        delta_coords.k = new_coords.k - drawable->coords.k;

        bbScreenPoints SP = bbMapCoords_getScreenPoints_relative(drawable->coords, new_coords);

        bbDebug("SP.x = %d, SP.y = %d\n", SP.x, SP.y);

        bbFrame output_frame;
        output_frame.type = input_frame->type;
        output_frame.handle = input_frame->handle;
        output_frame.offset.x = input_frame->offset.x + self_frame->offset.x + SP.x;
        output_frame.offset.y = input_frame->offset.y + self_frame->offset.y + SP.y;
        output_frame.framerate = input_frame->framerate * self_frame->framerate;
        output_frame.start_time = input_frame->start_time + self_frame->start_time;
        output_frame.drawfunction = input_frame->drawfunction;

        bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.drawfunction];

        bbHere()

        drawFunction(drawable, &output_frame, cl);

        bbHere()

    }
bbHere()
    return bbSuccess;
}
