/// Macros here are used to print text to the terminal

#ifndef BB_TERMINAL_H
#define BB_TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"

extern thread_local char* thread;
static const int stringLength = 512;
extern U64 test_time;

/// Print current file, function, line and thread
#define bbHere() printf ("In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s, TIME: %lu\n",\
__FILE_NAME__, __func__, __LINE__, thread, test_time);

#define bbNotHere() {\
printf ("In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s, TIME: %lu\n"\
"BB_NOT_HERE\n",\
__FILE_NAME__, __func__, __LINE__, thread, test_time);\
int x = 1/0;\
}

/// Print warning and exit, but preserve the stack for gdb
#define bbAssert(expression, ...)\
{\
if (!(expression)){\
char string1[stringLength];\
char string2[stringLength];\
sprintf (string1, "In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s,"\
"\nASSERTION: %s, ", __FILE_NAME__, __func__, __LINE__, thread,\
#expression );\
\
sprintf(string2, __VA_ARGS__);\
\
printf("%s%s", string1, string2);\
int x = 1/0;\
}\
}

#define bbDebug(...) {\
char string1[stringLength];\
char string2[stringLength];\
sprintf (string1, "In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s, TIME: %lu\nDEBUG: ",\
__FILE_NAME__, __func__, __LINE__, thread, test_time);\
sprintf (string2, __VA_ARGS__);\
printf("%s%s", string1, string2);\
}

#define bbPrintf(...) {\
char string[stringLength];\
sprintf (string, __VA_ARGS__);\
printf("bbPrintf: %s", string);\
}

#define bbFlag_print(flag)\
{\
switch ((bbFlag)flag)\
{\
case bbSuccess:\
bbDebug ("Flag = Success\n");\
break;\
case bbFail:\
bbDebug ("Flag = Fail\n");\
break;\
case bbNone:\
bbDebug ("Flag = None\n");\
break;\
case bbFull:\
bbDebug ("Flag = Full\n");\
break;\
case bbEmpty:\
bbDebug ("Flag = Empty\n");\
break;\
case bbBreak:\
bbDebug ("Flag = Break\n");\
break;\
case bbRepeat:\
bbDebug ("Flag = Repeat\n");\
break;\
case bbContinue:\
bbDebug ("Flag = Continue\n");\
break;\
case bbDelete:\
bbDebug ("Flag = Delete\n");\
break;\
case bbHead:\
bbDebug ("Flag = Head\n");\
break;\
case bbTail:\
bbDebug ("Flag = Tail\n");\
break;            \
\
}\
}\

#define sfSocketStatus_print(status)\
{\
switch((sfSocketStatus)status)\
{\
case sfSocketDone:\
bbDebug("status = sfSocketDone\n");\
break;\
case sfSocketNotReady:\
bbDebug("status = sfSocketNotReady\n");\
break;\
case sfSocketPartial:\
bbDebug("status = sfSocketPartial\n");\
break;\
case sfSocketDisconnected:\
bbDebug("status = sfSocketDisconnected\n");\
break;\
case sfSocketError:\
bbDebug("status = sfSocketError\n");\
break;\
default:\
bbDebug("status = unknown\n");\
\
}\
}\

//Similar function found in https://github.com/orichalcink/chatroom
static void bbClearLine(I32 lines)
{
    //    \r     #go to the start of the current line
    //    \033[F #back to previous line
    //    \033[K #delete everything from the cursor to the end of the line

    for(I32 i = 0; i <= lines;i++)
    {
        printf("\r\033[K");
        if (i<lines) printf("\033[F");
    }
    fflush(stdout);
}

//Similar function found in https://github.com/orichalcink/chatroom
static I32 bbGetLine(char* string, I32 bufferlength, FILE* fp)
{
    I32 numchars = 0;
    I32 c = '\0';

    while (1)
    {
        c = fgetc(fp);

        if (c == EOF || c == '\n') break;

        string[numchars] = c;

        if (numchars >= bufferlength - 1) break;
        numchars++;
    }

    string[numchars + 1] = '\0';

    return numchars + 1;
}

//Similar function found in https://github.com/orichalcink/chatroom
static I32 bbGetInt(char* prompt, int defaultValue)
{
    I32 number;
    char answer[64];
    while(1){
        printf("%s", prompt);

        for (I32 j = 0; j < 64; j++)
        {
            answer[j] = 0;
        }

        bbGetLine(answer,64,stdin);

        I32 len = strlen(answer);

        if (len == 0) return defaultValue;

        char digits[] = "0123456789";
        I32 int_len = strspn(answer, digits);

        if(len == int_len) {

            number = atoi(answer);
            return (number);
        }
        bbClearLine(1);

        printf("Invalid input. Please enter a valid integer.\n");


    }
}


//#define MUTEX_DEBUG
#ifdef MUTEX_DEBUG

#define bbMutexLock(mutex){\
bbDebug("Mutex Lock attempted in thread %s!\n", thread);\
pthread_mutex_lock(mutex);}\
bbPrintf("Mutex Lock aquired in thread %s!\n", thread);\


#define bbMutexUnlock(mutex){\
bbDebug("Mutex Unlock in thread %s!\n", thread);\
pthread_mutex_unlock(mutex);}\



#else //#ifdef MUTEX_DEBUG


#define bbMutexLock(mutex){\
pthread_mutex_lock(mutex);}\


#define bbMutexUnlock(mutex){\
pthread_mutex_unlock(mutex);}\

#endif //#ifdef MUTEX_DEBUG

#endif //BB_TERMINAL_H