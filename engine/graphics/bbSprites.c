#include "engine/logic/bbIntTypes.h"
#include "engine/data/CSFML.h"
#include "engine/graphics/bbSprites.h"

#include <stdlib.h>
#include <string.h>

#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"

typedef struct {
    I32 left;
    I32 top;
    I32 width;
    I32 height;
    float origin_x;
    float origin_y;
    float scale_x;
    float scale_y;
} sprite_dimensions;

bbFlag bbSprite_new(bbSprites* sprites, char* key, I32 address, sfTexture* texture, sprite_dimensions* dimensions){

    sfSprite* sprite;
#ifdef CSFML3
    sprite = sfSprite_create(texture);
    bbAssert(sprite != NULL, "sfSprite_create(NULL) returned NULL\n");
#else CSFML3
    sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);

#endif
    sfIntRect rect;

#ifdef CSFML3
    rect.position.x = dimensions->left;
    rect.position.y = dimensions->top;
    rect.size.x = dimensions->width;
    rect.size.y = dimensions->height;
#else CSFML3

    rect.left = dimensions->left;
    rect.top = dimensions->top;
    rect.width = dimensions->width;
    rect.height = dimensions->height;

#endif

    sfSprite_setTextureRect(sprite, rect);

    sfVector2f origin;
    origin.x = dimensions->origin_x;
    origin.y = dimensions->origin_y;
    sfSprite_setOrigin(sprite, origin);

    sfVector2f scale;
    scale.x = dimensions->scale_x;
    scale.y = dimensions->scale_y;
    sfSprite_setScale(sprite, scale);

    sprites->sprites[address] = sprite;
    bbHandle handle;
    handle.u64 = address;

    bbDictionary_add(sprites->dictionary, key, handle);

    return bbSuccess;
}

bbFlag bbSprites_new(bbSprites** self, bbTextures* textures, char* file_path,
                  float widget_scale,
                  float drawable_scale,
                  float ground_scale,
                  float mapicon_scale)
{

    FILE* file = fopen(file_path, "r");
    bbAssert(file!= NULL, "bad fopen\n");

    I32 num;
    fscanf(file, "Number of Sprites:,%d%*[^\n]\n", &num);

    bbSprites* sprites = malloc(sizeof(bbSprites) + num * sizeof (sfSprite*));


    sprites->num_sprites = num;
    bbDictionary_new(&sprites->dictionary, nextPrime(num));



    fscanf(file, "%*[^\n]\n");


    char key[KEY_LENGTH];
    I32 address;
    char texture[KEY_LENGTH];
    sfTexture* texture_ptr;
    sprite_dimensions dimensions;
    char scale_by[KEY_LENGTH];

    while(fscanf(file, "%[^,],%d,%[^,],%d,%d,%d,%d,%f,%f,%f,%f,%[^,],%*[^\n]\n",
                 key, &address, texture, &dimensions.left, &dimensions.top, &dimensions.width,
                 &dimensions.height, &dimensions.origin_x, &dimensions.origin_y, &dimensions.scale_x,
                 &dimensions.scale_y, scale_by) == 12)
    {
        if(0 == strcmp(scale_by, "Widget")){
            dimensions.scale_x *= widget_scale;
            dimensions.scale_y *= widget_scale;
        } else if(0 == strcmp(scale_by, "Drawable")){
            dimensions.scale_x *= drawable_scale;
            dimensions.scale_y *= drawable_scale;
        } else if(0 == strcmp(scale_by, "Ground")){
            dimensions.scale_x *= ground_scale;
            dimensions.scale_y *= ground_scale;
        } else if(0 == strcmp(scale_by, "MapIcon")){
            dimensions.scale_x *= mapicon_scale;
            dimensions.scale_y *= mapicon_scale;
        } else {

            dimensions.scale_x *= 1.f;
            dimensions.scale_y *= 1.f;
        }


        bbTextures_lookup(&texture_ptr, textures, texture);
        bbSprite_new(sprites, key, address, texture_ptr, &dimensions);
    }

    fclose(file);
    *self = sprites;

    return bbSuccess;
}


bbFlag bbSprites_lookup (sfSprite** self, bbSprites * sprites, char* key){

    I32 len = strlen(key);
    char digits[] = "0123456789";
    I32 int_len = strspn(key, digits);
    I32 address;
    if(len == int_len){
        address = atoi(key);

    } else {
        bbHandle handle;
        bbDictionary_lookup(sprites->dictionary, key, &handle);
        address = handle.u64;
    }
    bbAssert(address < sprites->num_sprites, "address out of bounds\n");

    *self = sprites->sprites[address];

    return bbSuccess;
}

bbFlag bbSprites_lookupInt(bbSprites* sprites, I32* address, char* key){

    I32 len = strlen(key);
    char digits[] = "0123456789";
    I32 int_len = strspn(key, digits);
    I32 address1;
    if(len == int_len){
        address1 = atoi(key);

    } else {
        bbHandle handle;
        bbDictionary_lookup(sprites->dictionary, key, &handle);
        address1 = handle.u64;
    }
    bbAssert(address1 < sprites->num_sprites,
             "address (%d) out of bounds (%d)\n", address1,sprites->num_sprites);

    *address = address1;

    return bbSuccess;
}
