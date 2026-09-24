#include "engine/viewport/bbDrawBuffer.h"

bbFlag bbDrawBuffer_bbHere(void* node, void* cl) {
    printf("###################3\nbbHere()\n");
}

bbFlag bbDrawBufferFunctions_populate(bbDrawBufferFunctions* draw_buffer) {
    bbDrawBufferFunctions_addFunction(draw_buffer,"DRAWBUFFER_HERE", bbDrawBuffer_bbHere);
    return bbSuccess;
}
