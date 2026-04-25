
#include "engine/userinterface/bbUIApp.h"

#include "engine/data/bbHome.h"

bbFlag bbUIApp_init(bbUIApp* app)
{
    home.UI.SplashTexture = sfTexture_createFromFile("./graphics/Splash.png", NULL);
    home.UI.SplashSprite = sfSprite_create();
    sfSprite_setTexture(home.UI.SplashSprite, home.UI.SplashTexture, sfTrue);


    home.UI.widgets.selected_textbox = NULL;

    sfVideoMode mode;
    mode.width = 720;
    mode.height = 480;
    mode.bitsPerPixel = 32;

    home.UI.window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);
    sfRenderWindow_setMouseCursorVisible(home.UI.window, sfFalse);
    sfRenderWindow_setKeyRepeatEnabled(home.UI.window,sfFalse);
    sfRenderWindow_setFramerateLimit(home.UI.window, 0);


#ifdef DEFINE_PONG
    sfRenderWindow_setTitle(home.UI.window, "Test Pong");
#endif
#ifdef DEFINE_SKELLYCHASE
    sfRenderWindow_setTitle(home.UI.window, "Skelly Chase");
#endif
    sfRenderWindow_drawSprite(home.UI.window, home.UI.SplashSprite , NULL);
    sfRenderWindow_display(home.UI.window);


    bbGraphicsApp_init(&home.UI.graphics);

    bbWidgets_init(&home.UI.widgets);


    bbMouse_Init(&home.UI.mouse, &home.UI.widgets, &home.UI.graphics);

    bbMouseFunctions_init(&home.UI.mouse.functions);
    bbMouseFunctions_populate(&home.UI.mouse.functions);


    bbInput_init(&home.UI.input, home.UI.window, &home.UI.mouse, &home.UI.widgets);

    bbUI_Inbox_init(&home.UI.inbox);
}


bbFlag bbUIApp_draw(bbUIApp* app)
{
    drawFuncClosure cl;
    cl.map_time = home.UI.clock2_handle.map_tick;
    cl.GUI_time = home.UI.clock2_handle.server_tick;
    cl.graphics = &home.UI.graphics;
    cl.target = &home.viewport_app.viewport;

#ifndef DEFINE_PONG
    bbDrawablesPlus_draw( &cl, 0, 0, 12, 12);
    //bbAvoidables_draw(home.private.viewportApp.avoidables, &cl, 0, 0, 12, 12);
    //bbGroundSurface_drawVisible(&home.ground_surface, &home.viewport_app.viewport);
    bbGroundSurface_draw(&home.ground_surface, &home.viewport_app.viewport, 0, 0);


#endif
    cl.target = app->window;
    bbWidgets_draw(&app->widgets, &cl);
    bbMouse_Draw(&app->mouse, &app->widgets, &home.UI.graphics, app->window);

    sfRenderWindow_display(app->window);

#ifndef DEFINE_PONG
   bbViewport_clear(&home.viewport_app.viewport);
#endif
    return bbSuccess;
}
