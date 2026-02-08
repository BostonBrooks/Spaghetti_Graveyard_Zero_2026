
#include <stdio.h>
#include <string.h>
#include "engine/graphics/bbCompositions.h"

#include <stdlib.h>

#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbDictionary.h"
#include "engine/graphics/bbDrawfunctions.h"

I32 bbCompositions_new(bbCompositions** self,
	                      bbSprites* sprites,
	                      bbAnimations* animations,
	                      bbDrawfunctions* drawfunctions,
	                      char* file_path){

	FILE* file = fopen(file_path, "r");
	bbAssert(file!= NULL, "bad fopen\n");

	I32 num_compositions;
	fscanf(file, "Number of Compositions:,%d%*[^\n]\n%*[^\n]\n", &num_compositions);

	bbCompositions* compositions = malloc(sizeof(bbCompositions) + num_compositions * sizeof(bbComposition*));
	compositions->num = num_compositions;
	bbDictionary_new(&compositions->dictionary, nextPrime(num_compositions));

	char label[KEY_LENGTH];
	I32 num_frames;
	I32 address;
	bbComposition* composition;
	bbHandle handle1;

	for (int i = 0; i < num_compositions; i++){

		fscanf(file, "Label:,%[^,],Integer Address:,%d,num:,%d%*[^\n]\n", label,&address, &num_frames);
		composition = malloc(sizeof(bbComposition)+num_frames*sizeof(bbFrame));
		//bbDebug("Label:,%s,Integer Address:,%d,num:,%d\n", label,address, num_frames);
		bbAssert(composition != NULL, "bad malloc\n");



		handle1.u64 = address;
		bbDictionary_add(compositions->dictionary, label, handle1);
		composition->num_frames = num_frames;
		//bbDebug("composition->num_frames = %d\n",composition->num_frames);
	/*	{
			char string[64];
			fscanf(file, "%[^\n]\n]", string);
			bbAssert(strcmp(string, "Type:,Key:,Drawfunction:,startTime,Offest X:,Offset Y:,Frame Rate:,") == 0,
					 "bad file\n");
		}*/ fscanf(file, "%*[^\n]\n]");



		char type[KEY_LENGTH];
		char asset[KEY_LENGTH];
		char drawfunction[KEY_LENGTH];
		I32 start_time;
		I32 offsetX, offsetY;
		float framerate;

		bbHandle handle, handle2;

		for(I32 j = 0; j < num_frames; j++) {
			fscanf(file, "%[^,],%[^,],%[^,],%d,%d,%d,%f%*[^\n]\n",
				   type, asset, drawfunction, &start_time, &offsetX, &offsetY, &framerate);

			if (strcmp(type, "SPRITE") == 0) {
				//look up asset in sprites
				bbDictionary_lookup(sprites->dictionary, asset, &handle);
				composition->frame[j].handle = handle;
				composition->frame[j].type = Sprite;

			} else if (strcmp(type, "ANIMATION") == 0) {
				//look up asset in animations
				bbDictionary_lookup(animations->dictionary, asset, &handle);
				composition->frame[j].handle = handle;
				composition->frame[j].type = Animation;

			} else if (strcmp(type, "COMPOSITION") == 0) {
				//look up asset in compositions
				bbDictionary_lookup(compositions->dictionary, asset, &handle);
				composition->frame[j].handle = handle;
				composition->frame[j].type = Composition;
			} else {
				bbAssert(0 == 1, "bad type in compositions.csv\n");
			}
			bbDictionary_lookup(drawfunctions->dictionary, drawfunction, &handle2);
			composition->frame[j].drawfunction = handle2.u64;
			//bbDebug("composition drawfunction = %" PRIu64 "\n", handle2.u64);
			composition->frame[j].start_time = start_time;
			composition->frame[j].offset.x = offsetX;
			composition->frame[j].offset.y = offsetY;
			composition->frame[j].framerate = framerate;

		}
		{
			char line1[1048];
			char line2[1048];
			fscanf(file,"%[^\n]\n", line1);
			//bbDebug("%s\n", line1);
		}
		compositions->compositions[address] = composition;
	}

	*self = compositions;
	return bbSuccess;
}
