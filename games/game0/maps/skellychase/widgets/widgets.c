#ifndef WIDGET_FUNCTIONS
#define WIDGET_FUNCTIONS
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/pong/widgets/textbox.h"
#include "games/game0/maps/pong/widgets/gamewidget.h"
#include "games/game0/maps/pong/widgets/react_button.h"
#include "games/game0/maps/pong/widgets/netsend_button.h"
#include "games/game0/maps/pong/widgets/action_button.h"
#include "games/game0/maps/pong/widgets/retroaction_button.h"
#include "games/game0/maps/pong/widgets/netcode_button.h"
#include "games/game0/maps/pong/widgets/netpause_button.h"
#include "games/game0/maps/pong/widgets/loop_button.h"
#include "games/game0/maps/pong/widgets/pause_button.h"
#include "games/game0/maps/pong/widgets/composition.h"
#include "games/game0/maps/pong/widgets/ball.h"
#include "games/game0/maps/pong/widgets/paddle.h"


bbFlag bbWidgetFunctions_populate(bbWidgetFunctions* self)
{

    return bbSuccess;
}



#endif //WIDGET_FUNCTIONS