
#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbGraphicsApp.h"

bbFlag bbDF_composition(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->asset_handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;

    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    for (int i = 0; i < composition->num_frames; i++){
        input_frame = &composition->frame[i];

        output_frame.type = input_frame->type;
        output_frame.asset_handle = input_frame->asset_handle;
        output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
        output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
        output_frame.framerate = input_frame->framerate * self_frame->framerate;
        output_frame.start_time = input_frame->start_time + self_frame->start_time;
        output_frame.draw_function = input_frame->draw_function;


        if (output_frame.draw_function <0){
            //bbDebug ("drawfunction == %d, type = %d\n",
            //		 output_frame.drawfunction, output_frame.type);
        } else {

            bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.draw_function];
            drawFunction(drawable, &output_frame, cl);
        }
    }
    return bbSuccess;
}


#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbGraphicsApp.h"

bbFlag bbDF_compositionState(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->asset_handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;

    bbDrawable* Drawable = drawable;
    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    //for (int i = 0; i < composition->num_frames; i++){
    I32 i = Drawable->md.state;
        input_frame = &composition->frame[i];

        output_frame.type = input_frame->type;
        output_frame.asset_handle = input_frame->asset_handle;
        output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
        output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
        output_frame.framerate = input_frame->framerate * self_frame->framerate;
        output_frame.start_time = input_frame->start_time + self_frame->start_time;
        output_frame.draw_function = input_frame->draw_function;


        if (output_frame.draw_function <0){
            //bbDebug ("drawfunction == %d, type = %d\n",
            //		 output_frame.drawfunction, output_frame.type);
        } else {

            bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.draw_function];
            drawFunction(drawable, &output_frame, cl);
        }
    //}
    return bbSuccess;
}

bbFlag bbDF_compositionParentState(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->asset_handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;

    bbDrawable* Drawable = drawable;
    bbRenderUnit* render_unit = drawable;
    bbDrawable* parent_drawable = render_unit->owner;
    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    //for (int i = 0; i < composition->num_frames; i++){
    I32 i = parent_drawable->md.state;
    input_frame = &composition->frame[i];

    output_frame.type = input_frame->type;
    output_frame.asset_handle = input_frame->asset_handle;
    output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
    output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
    output_frame.framerate = input_frame->framerate * self_frame->framerate;
    output_frame.start_time = input_frame->start_time + self_frame->start_time;
    output_frame.draw_function = input_frame->draw_function;


    if (output_frame.draw_function <0){
        //bbDebug ("drawfunction == %d, type = %d\n",
        //		 output_frame.drawfunction, output_frame.type);
    } else {

        bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.draw_function];
        drawFunction(drawable, &output_frame, cl);
    }
    //}
    return bbSuccess;
}

///Draw a group of 12 units. unit state = parent unit state, except only the front row plays the attack animation
bbFlag bbDF_frontRow(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->asset_handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;

    bbDrawable* Drawable = drawable;
    bbRenderUnit* render_unit = drawable;
    bbDrawable* parent_drawable = render_unit->owner;

    I32 row = render_unit->index / 4;
    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    //for (int i = 0; i < composition->num_frames; i++){
    I32 state = parent_drawable->md.state;

    if (row != 0 && state == bbDrawableState_attacking)
    {
        state = bbDrawableState_idle;
    }

    input_frame = &composition->frame[state];

    output_frame.type = input_frame->type;
    output_frame.asset_handle = input_frame->asset_handle;
    output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
    output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
    output_frame.framerate = input_frame->framerate * self_frame->framerate;
    output_frame.start_time = input_frame->start_time + self_frame->start_time;
    output_frame.draw_function = input_frame->draw_function;

    if (state == bbDrawableState_dead)
    {
        //bbHack()
        output_frame.start_time +=  parent_drawable->last_state_change;
       // bbDebug("output start_time = %lu, input start_time = %lu, of 12 start time = %lu, parent_drawable last sate change %lu\n", output_frame.start_time, input_frame->start_time, self_frame->start_time, parent_drawable->last_state_change);
    }

    if (output_frame.draw_function <0){
        //bbDebug ("drawfunction == %d, type = %d\n",
        //		 output_frame.drawfunction, output_frame.type);
    } else {

        bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.draw_function];
        drawFunction(drawable, &output_frame, cl);
    }
    //}
    return bbSuccess;
}
