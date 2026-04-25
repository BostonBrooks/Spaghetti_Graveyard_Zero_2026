#include "engine/geometry/bbCoordinates.h"
#include "engine/graphics/bbTextures.h"

typedef struct
{
    sfTexture* height_texture;
    sfRenderStates* render_states;
    sfShader* hill_shader;
    sfSprite* null_sprite;
    sfTexture* null_texture;


} bbHillShading;

//
bbFlag bbHillShading_init( bbHillShading* hill_shading, char* file);

bbFlag bbHillshading_calculate(bbHillShading* hill_shading,
    sfRenderTexture* target, bbSquareCoords* square_coords);