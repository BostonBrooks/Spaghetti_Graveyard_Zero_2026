#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/groundsurface/bbHillShading.h"



bbFlag bbGroundSurface_init(bbGroundSurface* surface, bbSquareCoords size, char* elevation_file)
{
    bbHillShading_init(&surface->hill_shading, elevation_file);



}