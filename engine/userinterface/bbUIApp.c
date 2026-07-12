
#include "engine/userinterface/bbUIApp.h"

#include "engine/data/bbHome.h"

bbFlag bbUIApp_init(bbUIApp* app)
{
    home.UI.SplashTexture = sfTexture_createFromFile("./graphics/Splash.png", NULL);

#ifdef CSFML3
    home.UI.SplashSprite = sfSprite_create(home.UI.SplashTexture);
#endif
#ifndef CSFML3
    home.UI.SplashSprite = sfSprite_create();

    sfSprite_setTexture(home.UI.SplashSprite, home.UI.SplashTexture, sfTrue);
#endif

    home.UI.widgets.selected_textbox = NULL;

    sfVideoMode mode;
#ifdef CSFML3
    mode.size.x = 720;
    mode.size.y = 480;
    mode.bitsPerPixel = 32;
#endif
#ifndef CSFML3
    mode.height = 480;
    mode.width = 720;
    mode.bitsPerPixel = 32;
#endif

#ifdef CSFML3
    home.UI.window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose,sfWindowed, NULL);
#endif
#ifndef CSFML3
    home.UI.window = sfRenderWindow_create(mode, "early demo", sfResize | sfClose, NULL);
#endif



    sfRenderWindow_setMouseCursorVisible(home.UI.window, sfFalse);
    sfRenderWindow_setKeyRepeatEnabled(home.UI.window,sfFalse);
    sfRenderWindow_setFramerateLimit(home.UI.window, 60);


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

    bbSquareCoords SC = bbMapCoords_getSquareCoords(home.viewport_app.viewport.viewpoint);

    I32 i_min = SC.i - 1;
    I32 j_min = SC.j - 1;

    I32 i_max = SC.i + 2;
    I32 j_max = SC.j + 2;

    I32 squares_i = home.viewport_app.drawables->squares_i;
    I32 squares_j = home.viewport_app.drawables->squares_j;

    if (i_min < 0) i_min = 0;
    if (j_min < 0) j_min = 0;
    if (i_max > squares_i) i_max = squares_i;
    if (j_max > squares_j) j_max = squares_j;

    bbDrawablesPlus_draw( &cl, i_min, j_min, i_max, j_max);
    //bbAvoidables_draw(home.private.viewportApp.avoidables, &cl, 0, 0, 12, 12);
    bbGroundSurface_drawVisible(&home.ground_surface, &home.viewport_app.viewport);
    //bbGroundSurface_draw(&home.ground_surface, &home.viewport_app.viewport, 0, 0);


#endif
    cl.target = app->window;
    bbWidgets_draw(&app->widgets, &cl);
    bbMouse_Draw(&app->mouse, &app->widgets, &home.UI.graphics, app->window);


#ifndef DEFINE_PONG
   bbViewport_clear(&home.viewport_app.viewport);
#endif
    return bbSuccess;
}
