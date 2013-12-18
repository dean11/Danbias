/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */

#define protocol_Gameplay_PlayerNavigation			0
#define protocol_Gameplay_PlayerMouseMovement		1
#define protocol_Gameplay_PlayerPosition			2
#define protocol_Gameplay_CreateObject				3
#define protocol_Gameplay_RemoveObject				4
#define protocol_Gameplay_ObjectPosition			5

#define protocol_Status					50

#define protocol_Lobby_Msg							100

#define protocol_General_Disconnect					200
#define protocol_General_Ping						201

#define PROTOCOL_TEST								1000

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
