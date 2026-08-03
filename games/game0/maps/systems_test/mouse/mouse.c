#ifndef MOUSE
#define MOUSE

#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"

//#include "games/game0/maps/systems_test/mouse/textbox.h"
//#include "games/game0/maps/systems_test/mouse/react_button.h"
//#include "games/game0/maps/systems_test/mouse/action_button.h"
//#include "games/game0/maps/systems_test/mouse/retroaction_button.h"
//#include "games/game0/maps/systems_test/mouse/netsend_button.h"
//#include "games/game0/maps/systems_test/mouse/netcode_button.h"
//#include "games/game0/maps/systems_test/mouse/loop_button.h"
//#include "games/game0/maps/systems_test/mouse/pause_button.h"

#include "games/game0/maps/systems_test/mouse/netpause_button.h"
#include "games/game0/maps/systems_test/mouse/hover.h"
#include "games/game0/maps/systems_test/mouse/viewport.h"
#include "games/game0/maps/systems_test/mouse/switch_character_button.h"
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
    bbMouseFunctions_add(self, MouseLeftDown,SwitchCharacterButton_LeftDown,"SWITCH_CHARACTER_BUTTON");
    bbMouseFunctions_add(self, MouseLeftDown,Viewport_LeftDown,"VIEWPORT");
    bbMouseFunctions_add(self, MouseRightDown,Viewport_RightDown,"VIEWPORT");

    return bbSuccess;
}



#endif //MOUSE