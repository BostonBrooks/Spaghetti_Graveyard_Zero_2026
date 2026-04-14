#ifndef MOUSE
#define MOUSE

#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"

#include "games/game0/maps/map0/mouse/textbox.h"
#include "games/game0/maps/map0/mouse/react_button.h"
#include "games/game0/maps/map0/mouse/action_button.h"
#include "games/game0/maps/map0/mouse/retroaction_button.h"
#include "games/game0/maps/map0/mouse/netsend_button.h"
#include "games/game0/maps/map0/mouse/netcode_button.h"
#include "games/game0/maps/map0/mouse/loop_button.h"
#include "games/game0/maps/map0/mouse/pause_button.h"
#include "games/game0/maps/map0/mouse/netpause_button.h"
bbFlag bbMouseFunctions_populate(bbMouseFunctions* self)
{

    return bbSuccess;
}




#endif //MOUSE