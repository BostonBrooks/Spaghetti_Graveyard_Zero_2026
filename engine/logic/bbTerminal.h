/// Macros here are used to print text to the terminal

#ifndef BB_TERMINAL_H
#define BB_TERMINAL_H

#include <stdio.h>
#include "engine/logic/bbFlag.h"

extern thread_local char* thread;
static const int stringLength = 512;

/// Print current file, function, line and thread
#define bbHere() printf ("In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s\n",\
__FILE_NAME__, __func__, __LINE__, thread);

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
sprintf (string1, "In FILE: %s, FUNCTION: %s, LINE: %d, THREAD: %s,\nDEBUG: ",\
__FILE_NAME__, __func__, __LINE__, thread);\
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

#endif //BB_TERMINAL_H