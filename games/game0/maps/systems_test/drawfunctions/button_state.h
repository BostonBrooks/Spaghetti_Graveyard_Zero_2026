#include "engine/userinterface/bbWidgets.h"

bbFlag bbDF_buttonState(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;
    bbWidget* widget = drawable;
    I32 i = (I32)widget->state;
    if (widget->is_frozen){ i = bbWidgetState_Frozen; }
    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    {
        input_frame = &composition->frame[i];

        output_frame.type = input_frame->type;
        output_frame.handle = input_frame->handle;
        output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
        output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
        output_frame.framerate = input_frame->framerate * self_frame->framerate;
        output_frame.start_time = input_frame->start_time + self_frame->start_time;
        output_frame.drawfunction = input_frame->drawfunction;


        if (output_frame.drawfunction <0 || output_frame.drawfunction > graphics->drawfunctions->num){
            //bbDebug ("drawfunction == %d, type = %d\n",
            //		 output_frame.drawfunction, output_frame.type);
        } else {

            bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.drawfunction];
            drawFunction(drawable, &output_frame, cl);
        }
    }
    return bbSuccess;
}
