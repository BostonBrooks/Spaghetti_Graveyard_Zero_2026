#include "engine/viewport/bbDrawBuffer.h"

#include "engine/ECS/bbECS.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbSystemPool.h"


I32 bbDrawBufferObject_isCloser(void* one, void* two){
    bbDrawBufferObject* object_one = one;
    bbDrawBufferObject* oject_two = two;

    I32 foo = oject_two->MC.i - object_one->MC.i
              -oject_two->MC.j + object_one->MC.j;

    if (foo > 0) return 1;
    if (foo < 0) return 0;

    if (object_one->collision > oject_two->collision) return 1;
    if (object_one->collision < oject_two->collision) return 0;

    bbNotHere()
    return 3;
}


bbFlag bbDrawBuffer_new(bbDrawBuffer** draw_buffer) {
    bbDrawBuffer* buffer = calloc(1, sizeof(bbDrawBuffer));
    bbVPool_newSystem(&buffer->pool, bbSystem_RenderBuffer,sizeof(bbDrawBufferObject),100,1000,"DRAWBUFFER" );
    bbList_init(&buffer->list,buffer->pool, NULL,offsetof(bbDrawBufferObject,list_element),bbDrawBufferObject_isCloser,bbSystem_RenderBuffer);
    *draw_buffer = buffer;
    return bbSuccess;
}
bbFlag bbDrawBufferFunctions_new(bbDrawBufferFunctions** draw_buffer) {
    bbDrawBufferFunctions* buffer = calloc(1, sizeof(bbDrawBufferFunctions));
    buffer->num = 0;
    bbDictionary_new(&buffer->dictionary,NUM_DRAWBUFFER_FUNCS);
    *draw_buffer = buffer;
    return bbSuccess;
}
bbFlag bbDrawBufferFunctions_addFunction(bbDrawBufferFunctions* buffer,char* key,bbDrawBuffer_drawFunc* function)
{
    //function(NULL,NULL);
    I32 available = buffer->num++;
    buffer->functions[available] = function;
    bbHandle function_handle;
    function_handle.u64 = available;
    bbDictionary_add(buffer->dictionary,key,function_handle);

    return bbSuccess;
}
//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
bbFlag bbDrawBuffer_draw_fn(void* node, void* cl)
{
    bbDrawBufferObject* object = node;
    drawBufferClosure* closure = (drawBufferClosure*)cl;
    bbGraphicsApp* graphics = closure->graphics;


    bbDrawBuffer_drawFunc *draw_function =
                   graphics->drawBufferFunctions->functions[object->draw_function];

    if(draw_function == NULL){bbHere();return bbNone;}
    return draw_function(node,cl);

}

bbFlag bbDrawBuffer_draw(bbDrawBuffer* buffer, drawBufferClosure* closure)
{
    bbDrawBufferObject* object;
    bbFlag flag =  bbList_popL(&buffer->list,(void**)&object);

    while(flag == bbSuccess) {
        bbDrawBuffer_draw_fn(object,closure);

        //TODO pool free crashes
        bbVPool_free(buffer->pool,object);
        flag = bbList_popL(&buffer->list,(void**)&object);
    }
    //bbDebug("number of draw buffers objects fer frame: %i\n",buffer->collision);
    buffer->collision = 0;
    return bbSuccess;
}

bbFlag bbDrawBufferObject_new(bbDrawBuffer* draw_buffer,bbDrawBufferObject** draw_buffer_object) {
    bbDrawBufferObject* object;
    bbList_alloc2(&draw_buffer->list,(void**)&object,NULL);
    object->collision = draw_buffer->collision++;
    *draw_buffer_object = object;
    bbSystemPool* pool = draw_buffer->pool->pool;

    return bbSuccess;

}

bbFlag bbDrawBufferObject_draw(bbDrawBuffer* draw_buffer,bbDrawBufferObject* draw_buffer_object)
{
    bbList_sortR(&draw_buffer->list,draw_buffer_object);
    return bbSuccess;
}
