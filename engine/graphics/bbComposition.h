/**
 * @file
 * @brief a composition is a collection of frames to be drawn to the screen
 * a frame contains data needed to draw a sprite, animation, or another frame
 *
 * compositions.csv has the following fields:
 * Label: used to lookup composition in dictionary
 * Virtual Address: where to store composition
 * up to FRAMES_PER_COMPOSITION of: {
 *     type: SPRITE/ANIMATION/COMPOSITION //which dictionary to look up identifier
 *     identifier: used to look up SPRITE/ANIMATION/COMPOSITION
 *     framerate:
 *     drawfunction:
 **/
#ifndef BB_COMPOSITION_H
#define BB_COMPOSITION_H


#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbDictionary.h"
#include "engine/graphics/bbSprites.h"
#include "engine/graphics/bbAnimations.h"
#include "engine/logic/bbHandle.h"
#include "engine/geometry/bbCoordinates.h"



typedef enum {
	Sprite,
	Animation,
	Composition,
} bbType;

// a bbFrame contains the data used to draw a sprite, animation or composition
typedef struct {
	//handle is usually an integer address of the sprite/animation
    //TODO do we need this or let drawfunction decide?
	bbType type;
	bbHandle handle;
	//I32 angle - angle supplied in radians by drawable/widget
	//I32 frame - calculated based on framerate (times framerate of animation)
	//            and startTime
	I32 drawfunction;
	U32 start_time;
	bbScreenPoints offset;
	//multiply by the framerate of the animation
	float framerate;
} bbFrame;

//A bbAnimation should be able to have variable number of frames, so must be dynaically allocated
typedef struct {
	I32 num_frames;
	bbFrame frame[];
	//U8 is_dynamically_allocated; - all compositions are statically allocated
} bbComposition;

typedef struct {
	I32 num;
	bbDictionary* dictionary;
	bbComposition* compositions[];
} bbCompositions;

I32 bbCompositions_new(bbCompositions** self,
	                    bbSprites* sprites,
	                    bbAnimations* animations,
	                    bbDrawfunctions* drawfunctions,
	                    char* file_path);

I32 bbCompositions_delete(bbCompositions* compositions);

I32 bbCompositions_lookupInt(bbCompositions* compositions, char* key);

#endif //BB_COMPOSITION_H