#ifndef BB_VIEWPORTAPP_H
#define BB_VIEWPORTAPP_H

#include "bbActiveSquares.h"
#include "bbVP_ProjectileSpawner.h"
#include "engine/viewport/bbDrawableFunctions.h"
#include "engine/viewport/bbViewportMouse.h"
#include "engine/viewport/bbMapIcons.h"
#include "engine/viewport/bbUnits.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewport.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbLookupTable.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/viewport/bbViewportSpawner.h"
#include "engine/render_units/bbRenderUnits.h"
#include "engine/viewport/bbDrawBuffer.h"

typedef struct bbViewportApp
{
    bbDrawables* drawables;
    bbMapIcons* mapIcons;
    bbUnits* units;
    bbList projectiles;
    bbDrawableFunctionTable table;
    struct bbRenderUnits* renderUnits;
    bbDrawBuffer* drawbuffer;
    bbViewport viewport;
    bbWidget* viewport_widget;

    bbHandle viewport_focus;
    bbActiveSquares active_squares;

    bbLookupTable* entity_units2;
    bbLookupTable* entity_projectiles;

    bbViewportSpawner viewport_spawner;
    bbVP_ProjectileSpawner projectile_spawner;

    bbVPMouse mouse;



} bbViewportApp;

bbFlag bbViewportApp_init(bbViewportApp* app);

bbFlag spawnDrawables(void);

bbFlag bbViewportApp_updateViewpoint(bbViewportApp* app);

#endif //BB_VIEWPORTAPP_H