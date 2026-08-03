
bbFlag bbDF_widgetGraphicsTimeAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;

    double time = home.UI.clock2_handle.map_tick;

    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(time - frame_descriptor->start_time) *
                      (double)animation->framerate * frame_descriptor->framerate) % animation->frames;
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}

bbFlag bbDF_widgetCoreTimeAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;

    double time = home.core.clock2_handle.map_tick;

    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(time - frame_descriptor->start_time) *
                      (double)animation->framerate * frame_descriptor->framerate) % animation->frames;
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}

bbFlag bbDF_widgetServerTimeAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;


    I64 itime;
    bbNetworkTime* network_time = home.network_time;
    bbNetworkTime_get(network_time, &itime);


    double time = itime  / (1000000.f / 60.f);

    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(time - frame_descriptor->start_time) *
                      (double)animation->framerate * frame_descriptor->framerate) % animation->frames;
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}
