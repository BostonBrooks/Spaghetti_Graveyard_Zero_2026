

///draw the viewport to the screen
bbFlag bbDF_widgetViewport(void* drawable, void* frameDescriptor, void* cl){
    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbViewport* viewport = widget->extra_data;

    bbScreenPoints SP;
    SP.x = widget->rect.left;
    SP.y = widget->rect.top;
    sfVector2f pos = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(viewport->background.sprite, pos);

    bbViewport_draw(closure->target, viewport);


    return bbSuccess;
}