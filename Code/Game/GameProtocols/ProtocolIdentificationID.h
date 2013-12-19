/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */

#define protocol_ID_INDEX							0

#define protocol_Gamplay_PlayerNavigation			300
#define protocol_Gamplay_PlayerMouseMovement		301
#define protocol_Gamplay_PlayerPosition				302
#define protocol_Gamplay_CreateObject				303
#define protocol_Gamplay_ObjectPosition				304


#define protocol_Lobby_Msg							100

#define protocol_General_Disconnect					200
#define protocol_General_Ping						201

#define PROTOCOL_TEST								1000

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
