#include "engine/graphics/bbFonts.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine/data/CSFML.h"
#include "engine/logic/bbPrime.h"


bbFlag bbFonts_new(bbFonts** self, char* filepath)
{
    FILE* file = fopen(filepath, "r");
    bbAssert(file != NULL, "bad fopen\n");

    I32 num;
    fscanf(file, "Number of Fonts:,%d%*[^\n]\n", &num);

    bbFonts* fonts = (bbFonts*)malloc(sizeof(bbFonts) + num * sizeof(sfFont*));
    bbAssert(fonts != NULL, "bad malloc\n");
    fonts->num_fonts = num;

    bbDictionary_new(&fonts->dictionary, nextPrime(num));

    fscanf(file, "%*[^\n]\n");

    char key[KEY_LENGTH];
    int address;
    bbHandle handle;
    char file_path[256];
    sfFont* font;

    while (fscanf(file, "%[^,],%d,%[^,],%*[^\n]\n", key, &address, file_path) == 3)
    {
        font = sfFont_createFromFile(file_path);
        bbAssert(font != NULL, "bad sfFont_createFromFile\n");
        handle.u64 = address;

        bbDictionary_add(fonts->dictionary, key, handle);
        fonts->fonts[address] = font;
    }
    fclose(file);
    *self = fonts;
    return bbSuccess;
}
