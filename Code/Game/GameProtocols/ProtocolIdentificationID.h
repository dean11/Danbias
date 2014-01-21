/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */


/** Index where the identifier is located(aka protocol identification index) **/
/* Use this as id accesser since it may change in the future. */
#define protocol_INDEX_ID							0


/***********************************/
/********* RESERVERD PROTOCOLS ***************************************************************************************************/
/**********   [ 0 - 100 ]  *********/
#define protocol_RESERVED_MIN							0
#define protocol_RESERVED_MAX							99

/***********************************/
/********* GENERAL PROTOCOLS ***************************************************************************************************/
/***********[ 100 - 200 ]***********/
#define protocol_GeneralMIN								100
#define protocol_General_Status						100
#define protocol_General_Text						101
#define protocol_GeneralMAX								199


/***********************************/
/********* LOBBY PROTOCOLS ***************************************************************************************************/
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
/********* GAMEPLAY PROTOCOLS ***************************************************************************************************/
/***********[ 300 - 400 ]***********/
#define protocol_GameplayMIN							300
#define protocol_Gameplay_PlayerMovement			300
#define protocol_Gameplay_PlayerMouseMovement		301
#define protocol_Gameplay_PlayerChangeWeapon		302
#define protocol_Gameplay_PlayerDamage				303
#define protocol_Gameplay_PlayerPickup				304
#define protocol_Gameplay_ObjectPosition			305
#define protocol_Gameplay_ObjectEnabled				306
#define protocol_Gameplay_ObjectDisabled			307
#define protocol_Gameplay_ObjectCreate				308
#define protocol_Gameplay_Initiate					309
#define protocol_GameplayMAX							399


/************************************/
/*********** PROTOCOL MACROS ***************************************************************************************************/
/************************************/
inline bool ProtocolIsLobby(short ID) { return (ID >= protocol_LobbyMIN && ID <= protocol_LobbyMAX); }
inline bool ProtocolIsGeneral(short ID) { return (ID >= protocol_GeneralMIN && ID <= protocol_GeneralMAX); }
inline bool ProtocolIsGameplay(short ID) { return (ID >= protocol_GameplayMIN && ID <= protocol_GameplayMAX); }

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
