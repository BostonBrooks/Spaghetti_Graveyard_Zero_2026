#ifndef BB_ANIMATIONS_H
#define BB_ANIMATIONS_H

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/graphics/bbSprites.h"

typedef struct {
    char key[KEY_LENGTH];
    I32 radius;
    I32 height;
    I32 drawfunction; // default draw function for the given animation
    I32 angles;
    I32 frames;
    float framerate;
    //the container where sprites are found
    bbSprites* sprites;

    //list of length angles * sprites, integer addresses into a bbSprites container
    bbHandle Sprites[];

} bbAnimation;


typedef struct { //bbAnimations
    I32 num_animations;
    //pointer to array of pointers to variable sized animations
    bbDictionary* dictionary;
    bbAnimation* animations[];

} bbAnimations;

I32 bbAnimations_new(bbAnimations** self, bbSprites* Sprites, bbDrawfunctions* drawfunctions,  char* file_path);

I32 bbAnimations_delete(bbAnimations* animations);

I32 bbAnimations_lookupInt(bbAnimations* animations, char* key);

#endif // BB_ANIMATIONS_H