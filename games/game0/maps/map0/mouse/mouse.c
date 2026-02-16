#ifndef MOUSE
#define MOUSE

#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"

#include "games/game0/maps/map0/mouse/textbox.h"
#include "games/game0/maps/map0/mouse/react_button.h"
#include "games/game0/maps/map0/mouse/action_button.h"
#include "games/game0/maps/map0/mouse/netsend_button.h"
#include "games/game0/maps/map0/mouse/netcode_button.h"

//mouse hovers over widget, uses pointer defined by widget
bbFlag IsOver_Hover(bbMouse* mouse, bbWidgets* widgets, bbWidget* widget)
{

    bbScreenPointsRect rect = widget->rect;
    bbScreenPoints point = mouse->position;

    if (bbScreenPoints_inScreenPointsRect(point, rect)){
        bbVPool* pool = widgets->pool;
        bbHandle handle;
        bbVPool_reverseLookup(pool,widget,&handle);

        if (!bbVPool_handleIsEqual(pool,handle,mouse->is_over))
        {
            mouse->was_over = mouse->is_over;
            mouse->is_over = handle;
        }
        return bbBreak;
    }
    return bbContinue;
}

//if the mouse is over the widget, it teleports to somewhere else, execution continues
bbFlag IsOver_Butterfly(bbMouse* mouse, bbWidgets* widgets, bbWidget* widget)
{

    bbScreenPointsRect rect = widget->rect;
    bbScreenPoints point = mouse->position;

    if (bbScreenPoints_inScreenPointsRect(point, rect)){
        widget->rect.left = rand()%(720*SCREEN_PPP - 80*SCREEN_PPP);
        widget->rect.top = rand()%(480*SCREEN_PPP - 80*SCREEN_PPP);

        widget->angle = rand()%8;
    }


    return bbContinue;
}

bbFlag bbMouseFunctions_populate(bbMouseFunctions* self)
{

    bbMouseFunctions_add(self, MouseIsOver, IsOver_Hover, "HOVER");
    bbMouseFunctions_add(self, MouseIsOver,IsOver_Butterfly,"BUTTERFLY");
    bbMouseFunctions_add(self, MouseLeftDown,Textbox_LeftDown,"TEXTBOX");
    bbMouseFunctions_add(self, MouseLeftDown,ReactButton_LeftDown,"BUTTON");
    bbMouseFunctions_add(self, MouseLeftDown,NetsendButton_LeftDown,"NETSEND_BUTTON");
    bbMouseFunctions_add(self, MouseLeftDown,ActionButton_LeftDown,"ACTION_BUTTON");
    bbMouseFunctions_add(self, MouseLeftDown,NetcodeButton_LeftDown,"NETCODE_BUTTON");

    return bbSuccess;
}




#endif //MOUSE