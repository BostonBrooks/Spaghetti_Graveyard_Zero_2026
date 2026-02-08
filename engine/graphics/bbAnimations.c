
#include "engine/graphics/bbAnimations.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine/graphics/bbSprites.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"


I32 bbAnimations_new(bbAnimations** self, bbSprites* Sprites, bbDrawfunctions* drawfunctions,  char* file_path) {
	FILE *file = fopen(file_path, "r");
	bbAssert(file != NULL, "bad fopen\n");

	I32 num;
	fscanf(file, "Number of Animations:,%d%*[^\n]\n", &num);

	bbAnimations *animations = malloc(
			sizeof(bbAnimations) + num * sizeof(bbHandle));
	animations->num_animations = num;
    bbDictionary_new(&animations->dictionary, nextPrime(num));

	fscanf(file, "%*[^\n]\n");

	char key[KEY_LENGTH];
	I32 address;
	bbHandle handle;
	I32 radius;
	I32 height;
	char drawfunction_str[KEY_LENGTH];
	bbHandle drawfunction_handle;
	I32 angles;
	I32 frames;
	float framerate;
	I32 sprite_int;
	bbHandle sprite_handle;

	while (fscanf(file, "%[^,],%d,%d,%d,%[^,],%d,%d,%f",
				 key,&address,&radius,&height,drawfunction_str,&angles,&frames,&framerate) == 8){

		bbAnimation* animation = malloc(sizeof(*animation) + angles*frames*sizeof(bbHandle));
		bbAssert(animation!=NULL, "bad malloc\n");
		bbStr_setStr(animation->key, key, KEY_LENGTH);
		animation->radius = radius;
		animation->height = height;

		bbDictionary_lookup(drawfunctions->dictionary, drawfunction_str, &drawfunction_handle);

		animation->drawfunction = drawfunction_handle.u64;
		animation->angles = angles;
		animation->sprites = Sprites;
		animation->frames = frames;
		animation->framerate = framerate;

		//bbDebug("label = %s, angles = %d, frames = %d, framerate = %f\n",
		//		animation->key, animation->angles, animation->frames, animation->framerate);
		for(I32 i = 0; i < angles*frames; i++){

			fscanf(file, ",%[^,]", key);
			bbSprites_lookupInt(Sprites, &sprite_int, key );
			sprite_handle.u64 = sprite_int;
			animation->Sprites[i] = sprite_handle;
		}
		fscanf(file, "%*[^\n]\n");

		handle.u64 = address;

		animations->animations[address] = animation;
		bbDictionary_add(animations->dictionary, animation->key, handle);
	}

	*self = animations;

    // bbDictionary_print(animations->dictionary);
	return bbSuccess;
}