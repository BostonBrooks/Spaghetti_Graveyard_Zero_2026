/**
 * @file
 * @brief the viewport is a widget that can be drawn to the screen,
 * and can be clicked by the player.
 * viewport has a bbViewport object which does the actual rendering of
 * the ground surface and bbDrawables
 **/
#ifndef BB_VIEWPORT_H
#define BB_VIEWPORT_H

#include "engine/data/bbIncludes.c"
#include "engine/logic/bbFlag.h"
#include "engine/geometry/bbCoordinates.h"

typedef struct {
	sfRenderTexture* renderTexture;
	sfTexture* texture;
	sfSprite* sprite;
} bbRenderable;



typedef struct {
	bbRenderable background;
	bbRenderable ground;
	bbRenderable main;
    bbRenderable mapicon;

    sfRenderStates renderStates;
	sfShader* shader;

    bbMapCoords viewpoint;
    int height;
    int width;

    void* widget;

} bbViewport;

bbFlag bbRenderable_init(bbRenderable* renderable, I32 height, I32 width);
bbFlag bbViewport_init(bbViewport* viewport, I32 height, I32 width);
bbFlag bbViewport_new(bbViewport** viewport,  I32 height, I32 width);
bbFlag bbViewport_draw(sfRenderWindow* window, bbViewport* viewport);
bbFlag bbViewport_clear(bbViewport* viewport);

#endif // BB_VIEWPORT_H