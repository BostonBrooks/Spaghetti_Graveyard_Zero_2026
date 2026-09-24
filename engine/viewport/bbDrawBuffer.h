#ifndef BBDRAWBUFFER_H
#define BBDRAWBUFFER_H
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbList.h"

//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
typedef bbFlag bbDrawBuffer_drawFunc(void* node, void* cl);
#define NUM_DRAWBUFFER_FUNCS 193
typedef struct {
    void* graphics;
    void* target;
    I32 GUI_time;
    I32 map_time;
} drawBufferClosure;

typedef struct {
    bbMapCoords MC;
    I32 collision;
    I32 draw_function;
    bbHandle asset_handle;
    bbListElement_Handle list_element;
} bbDrawBufferObject;

I32 bbDrawBufferObject_isCloser(void* one, void* two);

typedef struct {
    bbVPool* pool;
    bbList list;
    I32 collision;


} bbDrawBuffer;

typedef struct {
    I32 num;
    bbDictionary* dictionary;
    bbDrawBuffer_drawFunc* functions[NUM_DRAWBUFFER_FUNCS];
} bbDrawBufferFunctions;

I32 num;
bbDictionary* dictionary;
bbDrawBuffer_drawFunc* functions[NUM_DRAWBUFFER_FUNCS];


bbFlag bbDrawBufferFunctions_new(bbDrawBufferFunctions** draw_buffer);
bbFlag bbDrawBufferFunctions_addFunction(bbDrawBufferFunctions* draw_buffer,char* key,bbDrawBuffer_drawFunc* function);
bbFlag bbDrawBufferFunctions_populate(bbDrawBufferFunctions* draw_buffer);

bbFlag bbDrawBuffer_new(bbDrawBuffer** draw_buffer);
bbFlag bbDrawBuffer_draw(bbDrawBuffer* buffer, drawBufferClosure* closure);

bbFlag bbDrawBufferObject_new(bbDrawBuffer* draw_buffer,bbDrawBufferObject** draw_buffer_object);
bbFlag bbDrawBufferObject_draw(bbDrawBuffer* draw_buffer,bbDrawBufferObject* draw_buffer_object);


#endif