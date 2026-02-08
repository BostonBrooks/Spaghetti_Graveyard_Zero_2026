
#ifndef BB_FONTS_H
#define BB_FONTS_H


#include "engine/data/CSFML.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbTerminal.h"

typedef struct {

    I32 num_fonts;
    bbDictionary* dictionary;
    sfFont* fonts[];

} bbFonts;

bbFlag bbFonts_new(bbFonts** self, char* file_path);


#endif //BB_FONTS_H
