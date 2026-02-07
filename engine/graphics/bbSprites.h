#ifndef BB_SPRITES_H
#define BB_SPRITES_H


#include "engine/logic/bbDictionary.h"
//#include "engine/graphics/bbTextures.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/graphics/CSFML.h"
#include "engine/graphics/bbTextures.h"

typedef struct { //bbSprites
    I32 num_sprites;
    bbDictionary* dictionary;
    sfSprite* sprites[];

} bbSprites;

bbFlag bbSprites_new(bbSprites** self, bbTextures* textures, char* file_path,
                  float widget_scale, float drawable_scale, float ground_scale);

///when closing one map before opening another
bbFlag bbSprites_delete(bbSprites* sprites);

bbFlag bbSprites_lookup (sfSprite** self, bbSprites* sprites, char* key);


bbFlag bbSprites_lookupInt(bbSprites* sprites, I32* address, char* key);


#endif //BB_SPRITES_H