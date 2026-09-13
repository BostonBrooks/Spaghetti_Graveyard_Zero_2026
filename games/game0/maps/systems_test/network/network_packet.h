
#ifndef network_packet_h
#define network_packet_h


#include "engine/textbox/bbTextbox.h"
#include "engine/network/bbNetworkPacket.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"

bbFlag bbNetworkPacket_toMessage (sfPacket* packet, bbHandle* message_handle, bbTextbox_message** message);
bbFlag bbNetworkPacket_fromMessage (sfPacket* packet, bbHandle message_handle);

bbFlag bbNetworkApp_sendMessage(void* network, bbHandle message_handle, U64 time, U32 collision);
bbFlag bbNetworkApp_receiveMessage(void* network, void* packet);

#endif //network_packet_h