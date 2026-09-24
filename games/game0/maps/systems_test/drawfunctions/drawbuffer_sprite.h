bbFlag bbDF_drawBufferSprite(void* Drawable, void* frameDescriptor, void* cl){


    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    bbDrawBufferObject* draw_buffer_object;
    bbDrawBufferObject_new(home.viewport_app.drawbuffer,&draw_buffer_object) ;
    bbHandle draw_function_handle;
    bbFlag flag = bbDictionary_lookup(home.UI.graphics.drawBufferFunctions->dictionary,"DRAWBUFFER_SPRITE",&draw_function_handle);
    draw_buffer_object->draw_function = draw_function_handle.u64;
    draw_buffer_object->MC = drawable->md.coords;
    draw_buffer_object->asset_handle = frame->asset_handle;

    bbDrawBufferObject_draw(home.viewport_app.drawbuffer,draw_buffer_object);

    return bbSuccess;


}

bbFlag bbDF_drawBufferUnitSprite(void* Drawable, void* frameDescriptor, void* cl){


    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    bbDrawBufferObject* draw_buffer_object;
    bbDrawBufferObject_new(home.viewport_app.drawbuffer,&draw_buffer_object) ;
    bbHandle draw_function_handle;
    bbFlag flag = bbDictionary_lookup(home.UI.graphics.drawBufferFunctions->dictionary,"DRAWBUFFER_UNITSPRITE",&draw_function_handle);
    draw_buffer_object->draw_function = draw_function_handle.u64;
    draw_buffer_object->MC = drawable->md.coords;
    draw_buffer_object->asset_handle = frame->asset_handle;

    bbDrawBufferObject_draw(home.viewport_app.drawbuffer,draw_buffer_object);

    return bbSuccess;


}