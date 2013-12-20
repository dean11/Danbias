/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */


/** Index where the identifier is located(aka protocol identification index) **/
#define protocol_INDEX_ID							0


/***********************************/
/********* RESERVERD PROTOCOLS *****/
/**********   [ 0 - 100 ]  *********/


/***********************************/
/********* GENERAL PROTOCOLS *******/
/***********[ 100 - 200 ]***********/
#define protocol_GeneralMIN								100		/* This defines lower bounds of general protocols (okay to have same value as the first since this should not realy be sent). */
#define protocol_General_Disconnect					100
#define protocol_General_Ping						102
#define protocol_General_Text						103
#define protocol_General_Status						104
#define protocol_General_DisconnectKick				105
#define protocol_GeneralMAX								199


/***********************************/
/********* LOBBY PROTOCOLS *********/
/***********[ 200 - 300 ]***********/
#define protocol_LobbyMIN								200
#define protocol_Lobby_CreateGame					200
#define protocol_Lobby_JoinGame						201
#define protocol_Lobby_StartGame					202
#define protocol_Lobby_JoinLobby					203
#define protocol_Lobby_LeaveLobby					204
#define protocol_Lobby_CreateGameLobby				205
#define protocol_LobbyMAX								299


/***********************************/
/********* GAMEPLAY PROTOCOLS ******/
/***********[ 300 - 400 ]***********/
#define protocol_GameplayMIN							300
#define protocol_Gameplay_PlayerNavigation			300
#define protocol_Gameplay_PlayerMouseMovement		301
#define protocol_Gameplay_PlayerPosition			302
#define protocol_Gameplay_CreateObject				303
#define protocol_Gameplay_RemoveObject				304
#define protocol_Gameplay_ObjectPosition			305
#define protocol_GameplayMAX							399


/************************************/
/*********** PROTOCOL MACROS ********/
/************************************/
inline bool ProtocolIsLobby(short ID) { return (ID >= protocol_LobbyMIN && ID <= protocol_LobbyMAX); }
inline bool ProtocolIsGeneral(short ID) { return (ID >= protocol_GeneralMIN && ID <= protocol_GeneralMAX); }
inline bool ProtocolIsGameplay(short ID) { return (ID >= protocol_GameplayMIN && ID <= protocol_GameplayMAX); }



/***********************************/
/*********** TEST PROTOCOLS ********/
/***********[ 1000 - x ]************/
#define PROTOCOL_TEST								1000




#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
