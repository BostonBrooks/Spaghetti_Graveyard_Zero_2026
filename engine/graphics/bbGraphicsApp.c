#include "engine/graphics/bbGraphicsApp.h"
#include "engine/graphics/bbColours.h"

bbFlag bbGraphicsApp_init(bbGraphicsApp* app)
{
    bbColours_init();

    bbTextures_new(&app->textures,"./maps/map0/graphics/textures.csv");
    bbSprites_new(&app->sprites, app->textures,"./maps/map0/graphics/sprites.csv",
        1.f,1.f,1.f,1.f);

    bbDrawfunctions_new(&app->drawfunctions);
    bbAnimations_new(&app->animations, app->sprites, app->drawfunctions,
        "./maps/map0/graphics/animations.csv");

    bbCompositions_new(&app->compositions, app->sprites, app->animations, app->drawfunctions,
        "./maps/map0/graphics/compositions.csv");

    bbFonts_new(&app->fonts, "./maps/map0/graphics/fonts.csv");

        return bbSuccess;
}
