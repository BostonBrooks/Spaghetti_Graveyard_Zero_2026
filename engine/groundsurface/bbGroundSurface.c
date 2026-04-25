#include "engine/groundsurface/bbGroundSurface.h"

#include <assert.h>

#include "engine/groundsurface/bbHillShading.h"
#include "engine/viewport/bbViewport.h"

#define HEIGHT_MAP_PADDING    16
#define ELEVATION_SCALE         32
//#define HEIGHT_MAP_SIZE       (TILES_PER_MAP + 1 + 2 * HEIGHT_MAP_PADDING)

bbFlag Create_Ground_Shaders (bbGroundSurface* surface){
//vertex_shader
    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";

    //Fragment Shader
    char fragShader[] = "\
        uniform sampler2D Base_Texture;\
        uniform sampler2D Hill_Shading_Texture;\
        uniform sampler2D Footprints_Texture;\
        uniform sampler2D Auras_Texture;\
        uniform sampler2D Circles_Texture;\
        \
        void main()\
        {\
            vec4 base = texture2D(Base_Texture, gl_TexCoord[0].xy);\
            vec4 hill_shading = texture2D(Hill_Shading_Texture, gl_TexCoord[0].xy);\
            vec4 footprints = texture2D(Footprints_Texture, gl_TexCoord[0].xy);\
            vec4 auras = texture2D(Auras_Texture, gl_TexCoord[0].xy);\
            vec4 circles = texture2D(Circles_Texture, gl_TexCoord[0].xy);\
        \
            vec4 mix1 = base * hill_shading;\
            vec4 mix2 = mix(mix1, footprints, footprints.a);\
            vec4 mix3 = mix(mix2, auras, auras.a);\
            vec4 mix4 = mix(mix3, circles, circles.a);\
            gl_FragColor = mix4;\
        }\
    ";

//            gl_FragColor =  gl_Color * Light * pixel;\

    surface->null_render_texture = sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
    sfRenderTexture_clear(surface->null_render_texture, sfGreen);
    sfRenderTexture_display(surface->null_render_texture);
    surface->null_texture = sfRenderTexture_getTexture(surface->null_render_texture);

    surface->ground_shader = sfShader_createFromMemory(vertShader, NULL, fragShader);


    surface->ground_renderer.shader = surface->ground_shader;
    surface->ground_renderer.blendMode = sfBlendAlpha;
    surface->ground_renderer.transform = sfTransform_Identity;
    surface->ground_renderer.texture = surface->null_texture;



}



bbFlag elevations_init(bbGroundSurface* surface, bbSquareCoords size, char* elevation_file)
{
    I32 height_map_padding = HEIGHT_MAP_PADDING;
    I32 height_map_i = size.i * TILES_PER_SQUARE + 2*HEIGHT_MAP_PADDING +1;
    I32 height_map_j = size.j * TILES_PER_SQUARE + 2*HEIGHT_MAP_PADDING +1;

    sfImage*   height_map;
    sfVector2u height_map_size;
    sfUint8*   height_map_values;

    height_map = sfImage_createFromFile(elevation_file);
    bbAssert(height_map != NULL, "Bad file access\n");


    height_map_size = sfImage_getSize (height_map);
    height_map_values = sfImage_getPixelsPtr(height_map);

    surface->elevations = calloc(height_map_i*height_map_j, sizeof(I32));

    for (I32 i = 0; i < height_map_i; i++)
    {
        for (I32 j = 0; j < height_map_j; j++)
        {
            if (i >= height_map_size.x || j >= height_map_size.y)
            {
                surface->elevations[i*height_map_j + j] = 0;
            } else
            {

                surface->elevations[i*height_map_j + j]
                = ELEVATION_SCALE * height_map_values[(j + i * height_map_size.x) * 4];
            }
        }
    }

    sfImage_destroy(height_map);



}

I32 bbTileCoords_getElevation(bbGroundSurface* surface, bbTileCoords coords)
{
    I32 height_map_i = surface->map_size.i * TILES_PER_SQUARE + 2*HEIGHT_MAP_PADDING +1;
    I32 height_map_j = surface->map_size.j * TILES_PER_SQUARE + 2*HEIGHT_MAP_PADDING +1;

    I32 i = coords.i;
    I32 j = coords.j;

    if (i>=height_map_i) return 0;
    if (j>=height_map_j) return 0;

    i = i + HEIGHT_MAP_PADDING;
    j = j + HEIGHT_MAP_PADDING;

    if (i<0) return 0;
    if (j<0) return 0;


    return surface->elevations[i*height_map_j + j];

}

#define ISOMETRIC_FACTOR 2.f




sfVector2f bbGetVertex(bbGroundSurface* surface, int tile_i, int tile_j, int square_i, int square_j){

    bbTileCoords TC;
    TC.i = tile_i + square_i * TILES_PER_SQUARE;
    TC.j = tile_j + square_j * TILES_PER_SQUARE;
    TC.k = 0;

    int k = bbTileCoords_getElevation(surface,TC);


    int i, j;

    i = tile_i * PIXELS_PER_TILE;
    j = tile_j * PIXELS_PER_TILE;

    sfVector2f SC;

    SC.x = i + j;
    SC.y = i / ISOMETRIC_FACTOR - j / ISOMETRIC_FACTOR - k / (1.0 * POINTS_PER_PIXEL);

    return SC;

}


bbFlag bbGroundSquare_initVertexArray(bbGroundSurface* surface, bbSquareCoords SC)
{
    bbGroundSquare* square = &surface->ground_squares[SC.i*surface->map_size.j + SC.j];
    square->vertex_array = sfVertexArray_create();

    sfVertex top_vertex, bottom_vertex, left_vertex, right_vertex;

    sfVector2f V2F;

    for (I32 i = 0; i < TILES_PER_SQUARE; i++)
    {
        for (I32 j = 0; j < TILES_PER_SQUARE; j++)
        {
            V2F = bbGetVertex(surface, i, j+1, SC.i, SC.j);
            top_vertex.position.x = V2F.x;
            top_vertex.position.y = V2F.y;
            top_vertex.texCoords.y = i * PIXELS_PER_TILE;
            top_vertex.texCoords.x = (j+1) * PIXELS_PER_TILE;

            V2F = bbGetVertex(surface, i+1, j, SC.i, SC.j);
            bottom_vertex.position.x = V2F.x;
            bottom_vertex.position.y = V2F.y;
            bottom_vertex.texCoords.y = (i+1) * PIXELS_PER_TILE;
            bottom_vertex.texCoords.x = j* PIXELS_PER_TILE;

            V2F = bbGetVertex(surface, i, j, SC.i, SC.j);
            left_vertex.position.x = V2F.x;
            left_vertex.position.y = V2F.y;
            left_vertex.texCoords.y = i * PIXELS_PER_TILE;
            left_vertex.texCoords.x = j * PIXELS_PER_TILE;

            V2F = bbGetVertex(surface, i+1, j+1, SC.i, SC.j);
            right_vertex.position.x = V2F.x;
            right_vertex.position.y = V2F.y;
            right_vertex.texCoords.y = (i+1) * PIXELS_PER_TILE;
            right_vertex.texCoords.x = (j+1) * PIXELS_PER_TILE;


            top_vertex.color = sfWhite;
            bottom_vertex.color = sfWhite;
            left_vertex.color = sfWhite;
            right_vertex.color = sfWhite;


            sfVertexArray_append(square->vertex_array, top_vertex);
            sfVertexArray_append(square->vertex_array, right_vertex);
            sfVertexArray_append(square->vertex_array, left_vertex);

            sfVertexArray_append(square->vertex_array, left_vertex);
            sfVertexArray_append(square->vertex_array, right_vertex);
            sfVertexArray_append(square->vertex_array, bottom_vertex);

        }
    }
    return bbSuccess;
}

bbFlag bbGroundSurface_init(bbGroundSurface* surface, bbSquareCoords size, char* elevation_file)
{
    bbHillShading_init(&surface->hill_shading, elevation_file);

    surface->map_size = size;

    surface->ground_squares = calloc(size.i * size.j, sizeof(bbGroundSquare*));

    Create_Ground_Shaders(surface);

    elevations_init(surface, size, elevation_file);

    for (I32 i = 0; i < size.i; i++)
    {
        for (I32 j = 0; j < size.j; j++)
        {
            bbSquareCoords coords; coords.i = i; coords.j = j; coords.k = 0;

            bbGroundSquare* square = &surface->ground_squares[i*size.j + j];

            square->Base_Render_Texture
            = sfRenderTexture_create(
                PIXELS_PER_SQUARE,PIXELS_PER_SQUARE,sfFalse);
            square->Base_Texture
            = sfRenderTexture_getTexture(square->Base_Render_Texture);

            square->Hill_Shading_Render_Texture
            = sfRenderTexture_create(
                PIXELS_PER_SQUARE,PIXELS_PER_SQUARE,sfFalse);
            square->Hill_Shading_Texture
            = sfRenderTexture_getTexture(square->Hill_Shading_Render_Texture);

            square->Footprints_Render_Texture
            = sfRenderTexture_create(
                PIXELS_PER_SQUARE,PIXELS_PER_SQUARE,sfFalse);
            square->Footprints_Texture
            = sfRenderTexture_getTexture(square->Footprints_Render_Texture);

            square->Auras_Render_Texture
            = sfRenderTexture_create(
                PIXELS_PER_SQUARE,PIXELS_PER_SQUARE,sfFalse);
            square->Auras_Texture
            = sfRenderTexture_getTexture(square->Auras_Render_Texture);

            square->Circles_Render_Texture
            = sfRenderTexture_create(
                PIXELS_PER_SQUARE,PIXELS_PER_SQUARE,sfFalse);
            square->Circles_Texture
            = sfRenderTexture_getTexture(square->Circles_Render_Texture);


            bbHillshading_calculate(&surface->hill_shading,
                                   square->Hill_Shading_Render_Texture, coords);



            bbGroundSquare_initVertexArray(surface, coords);

        }
    }


    return bbNone;
}


sfVector2f bbTileCoords_getScreenCoords_centre (bbViewport* viewport, bbTileCoords TC){



    int mci = TC.i * POINTS_PER_TILE;
    int mcj = TC.j * POINTS_PER_TILE;
    int mck = TC.k;

    sfVector2f sc;
    sc.x = viewport->width/2.0
           + (mci - viewport->viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
           + (mcj - viewport->viewpoint.j) / (1.0 * POINTS_PER_PIXEL);

    sc.y = viewport->height/2.0
           + (mci - viewport->viewpoint.i) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mcj - viewport->viewpoint.j) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mck - viewport->viewpoint.k) / (1.0 * POINTS_PER_PIXEL);

    return sc;
}


bbFlag bbGroundSurface_draw(bbGroundSurface* surface, bbViewport* viewport, I32 Square_i, I32 Square_j){

    bbGroundSquare* square = &surface->ground_squares[Square_i*surface->map_size.j + Square_j];

    sfRenderTexture_display(square->Base_Render_Texture);
    sfRenderTexture_display(square->Hill_Shading_Render_Texture);
    sfRenderTexture_display(square->Footprints_Render_Texture);
    sfRenderTexture_display(square->Auras_Render_Texture);
    sfRenderTexture_display(square->Circles_Render_Texture);

    bbTileCoords TC;
    TC.i = Square_i * TILES_PER_SQUARE;
    TC.j = Square_j * TILES_PER_SQUARE;
    TC.k = 0;

    sfVector2f sf_left_corner = bbTileCoords_getScreenCoords_centre(viewport, TC);

    sfVertexArray* VA = square->vertex_array;

    sfShader_setVec2Uniform (surface->ground_shader, "offset", sf_left_corner);


    sfShader_setTextureUniform(surface->ground_shader,"Base_Texture", square->Base_Texture);
    sfShader_setTextureUniform(surface->ground_shader,"Hill_Shading_Texture", square->Hill_Shading_Texture);
    sfShader_setTextureUniform(surface->ground_shader,"Footprints_Texture", square->Footprints_Texture);
    sfShader_setTextureUniform(surface->ground_shader,"Auras_Texture", square->Auras_Texture);
    sfShader_setTextureUniform(surface->ground_shader,"Circles_Texture", square->Circles_Texture);

    sfRenderTexture_drawVertexArray (viewport->ground.renderTexture, VA, &surface->ground_renderer);

    sfRenderTexture_clear(square->Footprints_Render_Texture, sfTransparent);
    sfRenderTexture_clear(square->Auras_Render_Texture, sfTransparent);
    sfRenderTexture_clear(square->Circles_Render_Texture, sfTransparent);

    return bbSuccess;


}

#define ELEVATION_MAX       (ELEVATION_SCALE * PIXEL_VALUE_MAX)

int bbGroundSurface_drawVisible(bbGroundSurface* surface, bbViewport* viewport){

    bbScreenCoords sc;
    bbMapCoords TopLeft, TopRight, BottomLeft, BottomRight;


    //not sure if I should add VIEWPORT_TOP and VIEWPORT_LEFT
    //Im going to say no
    sc.x = 0;
    sc.y = -viewpoint.k;

    TopLeft = bbScreenCoords_getMapCoords_k0(sc);

    sc.x = VIEWPORT_WIDTH;
    sc.y = -viewpoint.k;

    TopRight = bbScreenCoords_getMapCoords_k0(sc);

    sc.x = 0;
    sc.y = VIEWPORT_HEIGHT + ELEVATION_MAX -viewpoint.k;

    BottomLeft = bbScreenCoords_getMapCoords_k0(sc);

    sc.x = VIEWPORT_WIDTH;
    sc.y = VIEWPORT_HEIGHT + ELEVATION_MAX -viewpoint.k;

    BottomRight = bbScreenCoords_getMapCoords_k0(sc);


    bbSquareCoords LeftCorner;
    bbSquareCoords RightCorner;

    LeftCorner.i = floordiv(TopLeft.i,POINTS_PER_SQUARE);
    LeftCorner.j = floordiv(BottomLeft.j,POINTS_PER_SQUARE);

    if (LeftCorner.i < 0) LeftCorner.i = 0;
    if (LeftCorner.j < 0) LeftCorner.j = 0;

    RightCorner.i = floordiv(BottomRight.i,POINTS_PER_SQUARE);
    RightCorner.j = floordiv(TopRight.j,POINTS_PER_SQUARE);

    if (RightCorner.i > SQUARES_PER_MAP-1) RightCorner.i = SQUARES_PER_MAP-1;
    if (RightCorner.j > SQUARES_PER_MAP-1) RightCorner.j = SQUARES_PER_MAP-1;
    //indexing starts at zero

    int m,n;

    for (n = RightCorner.j; n >= LeftCorner.j; n--){
        for (m = LeftCorner.i; m <= RightCorner.i; m++){
            bbGroundSurface_draw(m,n);
        }
    }
}
