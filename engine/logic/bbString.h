/// String manimpulation, adds safety chacks to string.h

#ifndef BB_STRING_H
#define BB_STRING_H

#include <string.h>
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"

/// convert string to 32 bit signed integer or I32_MIN
static I32 bbStr_toI32(char* Str){
    char* str = Str;
    I64 sign = 1;
    if (str[0] == '-'){
        sign = -1;
        str = &str[1];
    }

    int len = strlen(str);
    if (len > 16) return I32_MIN;

    char digits[] = "0123456789";
    int int_len = strspn(str, digits);

    if(len != int_len) return I32_MIN;

    I64 number = strtol(str,NULL, 10);
    number *= sign;

    if (number >= I32_MAX) return I32_MIN;
    if (number <= I32_MIN) return I32_MIN;

    return (I32)number;
}

/// strcpy()
static bbFlag bbStr_setStr(char* dest, char* src, I32 max){
    for(I32 i = 0; i < max; i++){
        dest[i] = src[i];
        if (src[i] == '\0') return bbSuccess;
    }
    dest[max - 1] = '\0';
    return bbFull;
}

/// strcat()
static bbFlag bbStr_putStr(char* dest, char* src, I32 max){
    I32 i = 0;
    while(1){
        if(dest[i] == '\0') break;
        i++;
        if (i >= max){
            dest[max - 1] = '\0';
            return bbFull;
        }
    }
    I32 j = 0;

    while(1){
        dest[i] = src[j];
        if (src[j] == '\0') return bbSuccess;
        i++;
        j++;
        if (i >= max){
            dest[max - 1] = '\0';
            return bbFull;
        }
    }
}

//TODO other functions
static bbFlag bbStr_putChar(char* dest, char src, I32 max)
{
    I32 i = 0;
    //backspace character
    if (src == '\b')
    {
        //nothing to backspace
        if (dest[0] == '\0') return bbSuccess;

        while (1)
        {
            i++;
            //End Of String not found
            if (i >= max)
            {
                dest[max - 1] = '\0';
                return bbSuccess;
            }
            //End Of String found, delete character:
            if (dest[i] == '\0')
            {
                dest[i-1] = '\0';
                return bbSuccess;
            }
        }

    }

    //not a backspace character
    while (1)
    {
        if (dest[i] == '\0') break;
        i++;
        //End Of String not found
        if (i >= max)
        {
            dest[max - 1] = '\0';
            return bbNone;
        }
    }

    //EOS not found
    if (i+1 >= max) {
         dest[i] = '\0';
        return bbNone;
    }
    //add character
    dest[i] = src;
    dest[i+1] = '\0';
    return bbSuccess;
}

#endif //BB_STRING_H