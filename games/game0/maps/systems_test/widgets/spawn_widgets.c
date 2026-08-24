
#include "engine/data/bbHome.h"
#include "engine/userinterface/bbUIApp.h"

bbFlag bbUIApp_spawnWidgets(bbUIApp* app)
{
    bbWidget* layout;
    bbWidget_newLayout(&layout, &home.UI.graphics, &home.UI.widgets, NULL);


    bbWidget_newViewport(&home.viewport_app.viewport_widget, &home.UI.graphics, &app->widgets, layout,
                         &home.viewport_app.viewport);

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "NETPAUSE_BUTTON",
                     "LAYOUT",
                     "(UN)PAUSE",
                     (bbScreenPoints){12*SCREEN_PPP,13*SCREEN_PPP});

    bbWidget_constructor(NULL,
                 &home.UI.widgets,
                 "SWITCH_CHARACTER_BUTTON",
                 "LAYOUT",
                 "CHARACTER",
                 (bbScreenPoints){12*SCREEN_PPP,35*SCREEN_PPP});

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "CLOCK",
                     "LAYOUT",
                     "CLOCK",
                     (bbScreenPoints){150*SCREEN_PPP,80*SCREEN_PPP});

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "PERFORMANCE",
                     "LAYOUT",
                     "PERFORMANCE",
                     (bbScreenPoints){12*SCREEN_PPP,468*SCREEN_PPP});

    bbWidget_constructor(NULL,
                 &home.UI.widgets,
                 "SOCKET_NUMBER",
                 "LAYOUT",
                 "SOCKET_NUMBER",
                 (bbScreenPoints){200*SCREEN_PPP,13*SCREEN_PPP});

    return bbSuccess;
}
