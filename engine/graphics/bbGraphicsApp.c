#include "engine/graphics/bbGraphicsApp.h"
#include "engine/graphics/bbColours.h"

#include "graphics_paths.h"

//char textures_path[] = "./maps/map0/graphics/textures.csv";
//char sprites_path[] = "./maps/map0/graphics/sprites.csv";
//char animations_path[] = "./maps/map0/graphics/animations.csv";
//char compositions_path[] = "./maps/map0/graphics/compositions.csv";
//char fonts_path[] = "./maps/map0/graphics/fonts.csv";

bbFlag bbGraphicsApp_init(bbGraphicsApp* app)
{
    bbColours_init();

    bbTextures_new(&app->textures,textures_path);
    bbSprites_new(&app->sprites, app->textures,sprites_path,
        1.f,1.f,1.f,1.f);

    bbDrawfunctions_new(&app->drawfunctions);
    bbAnimations_new(&app->animations, app->sprites, app->drawfunctions,
         animations_path);

    bbCompositions_new(&app->compositions, app->sprites, app->animations, app->drawfunctions,
        compositions_path);

    bbFonts_new(&app->fonts, fonts_path);

        return bbSuccess;
}
