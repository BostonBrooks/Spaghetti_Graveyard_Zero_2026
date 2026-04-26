#ifndef BB_GROUNDSURFACE_H
#define BB_GROUNDSURFACE_H


#include "engine/groundsurface/bbHillShading.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbViewport.h"

typedef struct
{
    bbSquareCoords SC;

    sfRenderTexture*  Base_Render_Texture;
    sfTexture*        Base_Texture;


    sfImage*          Hill_Shading_CPU;
    sfUint8*          Hill_Shading_Data;
    sfRenderTexture*  Hill_Shading_Render_Texture;
    sfTexture*        Hill_Shading_Texture;

    sfRenderTexture*  Shadows_Render_Texture;
    sfTexture*        Shadows_Texture;

    sfRenderTexture*  Footprints_Render_Texture;
    sfTexture*        Footprints_Texture;

    sfRenderTexture*  Auras_Render_Texture;
    sfTexture*        Auras_Texture;

    sfRenderTexture*  Circles_Render_Texture;
    sfTexture*        Circles_Texture;

    sfVertexArray*    vertex_array;


} bbGroundSquare;

typedef struct
{
    bbSquareCoords        map_size;
    bbGroundSquare*       ground_squares;

    sfRenderStates       ground_renderer;
    sfShader*             ground_shader;

    sfTexture*            null_texture;
    sfRenderTexture*      null_render_texture;

    bbHillShading         hill_shading;

    I32* elevations;

} bbGroundSurface;

bbFlag bbGroundSurface_getSquare(bbGroundSquare** square, bbGroundSurface* surface, bbSquareCoords SC);

/** Initialise vertex array for one map square */
bbFlag bbGroundSquare_initVertexArray(bbGroundSurface* surface, bbSquareCoords SC);

/** Initialise hill shading for one map square */
bbFlag bbGroundSquare_calcHillShading(int Square_i, int Square_j);

bbFlag bbGroundSurface_init(bbGroundSurface* surface, bbSquareCoords size, char* elevation_file);

I32 bbTileCoords_getElevation(bbGroundSurface* surface, bbTileCoords coords);
I32 bbMapCoords_getElevation(bbGroundSurface* ground_surface, bbMapCoords coords);

bbFlag bbGroundSurface_drawVisible(bbGroundSurface* surface, bbViewport* viewport);


bbFlag bbGroundSurface_draw(bbGroundSurface* surface, bbViewport* viewport, I32 Square_i, I32 Square_j);
bbFlag bbGroundSurface_setTexture(bbGroundSurface* surface);
#endif //BB_GROUNDSURFACE_H