

bbFlag bbDF_healthPointsTest(void* drawable, void* frameDescriptor, void* cl){

    static sfRectangleShape* red_rect = NULL;
    static sfRectangleShape* green_rect = NULL;

    if (red_rect == NULL)
    {
        bbAssert(green_rect == NULL, "bad init in health bar drawfunction\n");
        red_rect = sfRectangleShape_create();
        green_rect = sfRectangleShape_create();
        sfRectangleShape_setFillColor(red_rect, sfRed);
        sfRectangleShape_setFillColor(green_rect, sfGreen);

        sfVector2f V2F;
        V2F.x = 50;
        V2F.y = 5;
        sfRectangleShape_setSize(red_rect, V2F);

        V2F.x = 30;
        V2F.y = 5;
        sfRectangleShape_setSize(green_rect, V2F);

        V2F.x = 25;
        V2F.y = 100;

        sfRectangleShape_setOrigin(red_rect, V2F);
        sfRectangleShape_setOrigin(green_rect, V2F);

    }




    bbDrawable* mapicon = drawable;
    drawFuncClosure* foo = cl;
    bbFrame* frame_descriptor = frameDescriptor;

    bbUnit* unit = drawable;

    if (unit->display_health_until > home.UI.clock2_handle.map_tick
        && unit->percent_health > 0)

    {
        sfVector2f V2F;
        V2F.x = unit->percent_health / 2.0;
        V2F.y = 5;
        sfRectangleShape_setSize(green_rect, V2F);


        bbViewport* VP = foo->target;

        sfRenderTexture* renderTexture = VP->mapicon.renderTexture;

        V2F = bbMapCoords_getV2f(mapicon->coords, VP);



        sfRectangleShape_setPosition(red_rect, V2F);
        sfRectangleShape_setPosition(green_rect, V2F);
        sfRenderTexture_drawRectangleShape(renderTexture, red_rect, NULL);
        sfRenderTexture_drawRectangleShape(renderTexture, green_rect, NULL);
    }
    return bbSuccess;

}