
#ifndef network_packet_h
#define network_packet_h


#include "engine/textbox/bbTextbox.h"
#include "engine/network/bbNetworkPacket.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"

bbFlag bbNetworkPacket_toMessageHandle (sfPacket* packet, bbHandle* message_handle, bbTextbox_message** message);
bbFlag bbNetworkPacket_fromMessageHandle (sfPacket* packet, bbHandle message_handle);

bbFlag bbNetworkApp_sendMessage(void* network, bbHandle message_handle, bbTime time, U32 collision);
bbFlag bbNetworkApp_receiveMessage(void* network, void* packet);


///Used to receive a textbox message across the network and return a bbTextbox_message handle
bbFlag bbNetworkPacket_toMessage (sfPacket* packet, bbTextbox_message* message);
///Used to send a textbox message across the network, from a handle
bbFlag bbNetworkPacket_fromMessage (sfPacket* packet, bbTextbox_message* message);

#endif //network_packet_h