

#ifndef BB_INPUT_H
#define BB_INPUT_H

#include "engine/logic/bbFlag.h"
#include "engine/data/CSFML.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"

typedef struct {
    bbMouse* mouse;
    bbWidgets* widgets;

    char keymap_lowercase[sfKeyCount];
    char keymap_uppercase[sfKeyCount];
} bbInput;

bbFlag bbInput_init(bbInput* input, sfRenderWindow* window, bbMouse* mouse, bbWidgets* widgets);
bbFlag bbInput_poll(bbInput* input, sfRenderWindow* window);


#endif //BB_INPUT_H

