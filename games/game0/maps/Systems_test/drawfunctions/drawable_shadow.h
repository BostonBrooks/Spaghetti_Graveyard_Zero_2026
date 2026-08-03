#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbPrime.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/viewport/bbViewport.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbMapIcons.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/data/bbHome.h"

bbFlag bbDF_drawableShadow(void* Drawable, void* frameDescriptor, void* cl)
{
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;


    I32 spriteInt = frame->handle.u64;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt];

    bbMapCoords MC = drawable->coords;

    bbMapCoords MC_offset;

    bbSquareCoords SC;
    bbGroundSurface* surface = &home.ground_surface;
    bbGroundSquare* square;
    float point_i;
    float point_j;
    sfVector2f position;
    sfRenderTexture* Render_Texture;


    MC_offset = MC;
    MC_offset.i -= POINTS_PER_SQUARE /2 ;
    MC_offset.j -= POINTS_PER_SQUARE /2 ;
    SC = bbMapCoords_getSquareCoords(MC_offset);

    if (SC.i>=0 && SC.j>=0 && SC.i < surface->map_size.i && SC.j < surface->map_size.j)
    {
        square = &surface->ground_squares[SC.i*surface->map_size.j + SC.j];
        point_i = MC.i - SC.i * POINTS_PER_SQUARE;
        point_j = MC.j - SC.j * POINTS_PER_SQUARE;
        position.x = point_j / POINTS_PER_PIXEL;
        position.y = point_i / POINTS_PER_PIXEL;
        sfSprite_setPosition(sprite, position);
        Render_Texture = square->Shadows_Render_Texture;
        sfRenderTexture_drawSprite(Render_Texture, sprite, NULL);
    }

    MC_offset = MC;
    MC_offset.i -= POINTS_PER_SQUARE /2 ;
    MC_offset.j += POINTS_PER_SQUARE /2 ;
    SC = bbMapCoords_getSquareCoords(MC_offset);

    if (SC.i>=0 && SC.j>=0 && SC.i < surface->map_size.i && SC.j < surface->map_size.j)
    {
        square = &surface->ground_squares[SC.i*surface->map_size.j + SC.j];
        point_i = MC.i - SC.i * POINTS_PER_SQUARE;
        point_j = MC.j - SC.j * POINTS_PER_SQUARE;
        position.x = point_j / POINTS_PER_PIXEL;
        position.y = point_i / POINTS_PER_PIXEL;
        sfSprite_setPosition(sprite, position);
        Render_Texture = square->Shadows_Render_Texture;
        sfRenderTexture_drawSprite(Render_Texture, sprite, NULL);
    }

    MC_offset = MC;
    MC_offset.i += POINTS_PER_SQUARE /2 ;
    MC_offset.j -= POINTS_PER_SQUARE /2 ;
    SC = bbMapCoords_getSquareCoords(MC_offset);

    if (SC.i>=0 && SC.j>=0 && SC.i < surface->map_size.i && SC.j < surface->map_size.j)
    {
        square = &surface->ground_squares[SC.i*surface->map_size.j + SC.j];
        point_i = MC.i - SC.i * POINTS_PER_SQUARE;
        point_j = MC.j - SC.j * POINTS_PER_SQUARE;
        position.x = point_j / POINTS_PER_PIXEL;
        position.y = point_i / POINTS_PER_PIXEL;
        sfSprite_setPosition(sprite, position);
        Render_Texture = square->Shadows_Render_Texture;
        sfRenderTexture_drawSprite(Render_Texture, sprite, NULL);
    }

    MC_offset = MC;
    MC_offset.i += POINTS_PER_SQUARE /2 ;
    MC_offset.j += POINTS_PER_SQUARE /2 ;
    SC = bbMapCoords_getSquareCoords(MC_offset);

    if (SC.i>=0 && SC.j>=0 && SC.i < surface->map_size.i && SC.j < surface->map_size.j)
    {
        square = &surface->ground_squares[SC.i*surface->map_size.j + SC.j];
        point_i = MC.i - SC.i * POINTS_PER_SQUARE;
        point_j = MC.j - SC.j * POINTS_PER_SQUARE;
        position.x = point_j / POINTS_PER_PIXEL;
        position.y = point_i / POINTS_PER_PIXEL;
        sfSprite_setPosition(sprite, position);
        Render_Texture = square->Shadows_Render_Texture;
        sfRenderTexture_drawSprite(Render_Texture, sprite, NULL);
    }

    return bbSuccess;
}