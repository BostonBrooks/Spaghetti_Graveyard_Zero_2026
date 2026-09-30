#include <stdio.h>

#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"



bbTime test_time = 0;
thread_local char* thread;
thread_local bool debug_off;


int main(void) {

    char str [512];
    //str[127] = '\0';
    //I32 buffer_start = 127;
    char bounded_str[512];
    for (I32 i = 0; i < 7; i++) {
        char output[128];
        snprintf(output,128,"This is a big long text message used to test the bbStr_copyBounds operations.Testing %d\n", i);

        //bbStr_copyBack(str,&buffer_start,output,messageLength);

        bbStr_putStr(str,output,512);

        bbStr_copyBounds(bounded_str,str,11,5,512);

        bbDebug("buffer: \n%s\nbounded: \n%s\n", str, bounded_str);
    }


}
