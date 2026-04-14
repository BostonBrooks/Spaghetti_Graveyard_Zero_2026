
#include "engine/data/bbHome.h"
#include "engine/userinterface/bbUIApp.h"

bbFlag bbUIApp_spawnWidgets(bbUIApp* app)
{
    bbWidget_newLayout(NULL, &home.UI.graphics, &home.UI.widgets, NULL);

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "NETPAUSE_BUTTON",
                     "LAYOUT",
                     "(UN)PAUSE",
                     (bbScreenPoints){13*SCREEN_PPP,13*SCREEN_PPP});

    bbWidget_constructor(NULL,
                     &home.UI.widgets,
                     "CLOCK",
                     "LAYOUT",
                     "CLOCK",
                     (bbScreenPoints){150*SCREEN_PPP,80*SCREEN_PPP});

    return bbSuccess;
}
