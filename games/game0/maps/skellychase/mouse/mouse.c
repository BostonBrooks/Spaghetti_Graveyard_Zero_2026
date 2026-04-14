#ifndef MOUSE
#define MOUSE

#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"

//#include "games/game0/maps/skellychase/mouse/textbox.h"
//#include "games/game0/maps/skellychase/mouse/react_button.h"
//#include "games/game0/maps/skellychase/mouse/action_button.h"
//#include "games/game0/maps/skellychase/mouse/retroaction_button.h"
//#include "games/game0/maps/skellychase/mouse/netsend_button.h"
//#include "games/game0/maps/skellychase/mouse/netcode_button.h"
//#include "games/game0/maps/skellychase/mouse/loop_button.h"
//#include "games/game0/maps/skellychase/mouse/pause_button.h"

#include "games/game0/maps/skellychase/mouse/netpause_button.h"
#include "games/game0/maps/skellychase/mouse/hover.h"
bbFlag bbMouseFunctions_populate(bbMouseFunctions* self)
{

    bbMouseFunctions_add(self, MouseIsOver, IsOver_Hover, "HOVER");
    //bbMouseFunctions_add(self, MouseIsOver,IsOver_Butterfly,"BUTTERFLY");
    //bbMouseFunctions_add(self, MouseLeftDown,Textbox_LeftDown,"TEXTBOX");
    //bbMouseFunctions_add(self, MouseLeftDown,ReactButton_LeftDown,"BUTTON");
    //bbMouseFunctions_add(self, MouseLeftDown,NetsendButton_LeftDown,"NETSEND_BUTTON");
    //bbMouseFunctions_add(self, MouseLeftDown,ActionButton_LeftDown,"ACTION_BUTTON");
   // bbMouseFunctions_add(self, MouseLeftDown,RetroactionButton_LeftDown,"RETROACTION_BUTTON");
    //bbMouseFunctions_add(self, MouseLeftDown,NetcodeButton_LeftDown,"NETCODE_BUTTON");
   // bbMouseFunctions_add(self, MouseLeftDown,LoopButton_LeftDown,"LOOP_BUTTON");
   // bbMouseFunctions_add(self, MouseLeftDown,PauseButton_LeftDown,"PAUSE_BUTTON");
    bbMouseFunctions_add(self, MouseLeftDown,NetpauseButton_LeftDown,"NETPAUSE_BUTTON");

    return bbSuccess;
}



#endif //MOUSE