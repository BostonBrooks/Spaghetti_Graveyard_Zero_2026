#ifndef BB_TEXTURES_H
#define BB_TEXTURES_H

#include "engine/graphics/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbDictionary.h"

typedef struct { //bbTextures
    I32 num_textures;
    bbDictionary* dictionary;
    sfTexture* textures[];

} bbTextures;

bbFlag bbTextures_new(bbTextures** self, char* file_path);
bbFlag bbTextures_delete(bbTextures* textures);

//if key is int, return texture at that address
//otherwise, look up key in dictionary and return the texture pointed to.
bbFlag bbTextures_lookup (sfTexture** self, bbTextures* textures, char* key);


#endif // BB_TEXTURES_H