#ifndef BB_NETWORK_TEXT_BOX_OBJECT_H
#define BB_NETWORK_TEXT_BOX_OBJECT_H

#include "bbNetworkPacket.h"
#include "engine/logic/bbFlag.h"
#include "engine/textbox/bbTextbox.h"

///Used to receive a textbox message across the network and return a bbTextbox_message object
bbFlag bbNetworkPacket_toMessage (sfPacket* packet, bbTextbox_message* message);
///Used to send a textbox message across the network, from a reference
bbFlag bbNetworkPacket_fromMessage (sfPacket* packet, bbTextbox_message* message);

#endif //BB_NETWORK_TEXT_BOX_OBJECT_H