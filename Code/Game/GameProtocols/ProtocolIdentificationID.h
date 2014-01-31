/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */


/***********************************/
/********* RESERVERD PROTOCOLS ***************************************************************************************************/
/**********   [ 0 - 99 ]  *********/
#define protocol_RESERVED_MIN							0
#define protocol_RESERVED_MAX							99

/***********************************/
/********* GENERAL PROTOCOLS ***************************************************************************************************/
/***********[ 100 - 199 ]***********/
#define protocol_GeneralMIN								100
#define protocol_General_Status						100
#define protocol_General_Text						101
#define protocol_GeneralMAX								199


/***********************************/
/********* LOBBY PROTOCOLS ***************************************************************************************************/
/***********[ 200 - 299 ]***********/
#define protocol_LobbyMIN								200	
#define protocol_Lobby_Create						200
#define protocol_Lobby_Start						201
#define protocol_Lobby_Join							202
#define protocol_Lobby_Login						203
#define protocol_Lobby_Refresh						204
#define protocol_Lobby_ClientData					205
#define protocol_Lobby_GameData						206
#define protocol_LobbyMAX								299


/***********************************/
/********* GAMEPLAY PROTOCOLS ***************************************************************************************************/
/***********[ 300 - 399 ]***********/
#define protocol_GameplayMIN							300
#define protocol_Gameplay_PlayerMovement			300
#define protocol_Gameplay_PlayerLookDir		301
#define protocol_Gameplay_PlayerChangeWeapon		302
#define protocol_Gameplay_PlayerShot				303
#define protocol_Gameplay_PlayerJump				304
#define protocol_Gameplay_ObjectPickup				305
#define protocol_Gameplay_ObjectDamage				306
#define protocol_Gameplay_ObjectPosition			307
#define protocol_Gameplay_ObjectEnabled				308
#define protocol_Gameplay_ObjectDisabled			309
#define protocol_Gameplay_ObjectCreate				310
#define protocol_GameplayMAX							399


/************************************/
/*********** PROTOCOL MACROS ***************************************************************************************************/
/************************************/
inline bool ProtocolIsLobby(short ID) { return (ID >= protocol_LobbyMIN && ID <= protocol_LobbyMAX); }
inline bool ProtocolIsGeneral(short ID) { return (ID >= protocol_GeneralMIN && ID <= protocol_GeneralMAX); }
inline bool ProtocolIsGameplay(short ID) { return (ID >= protocol_GameplayMIN && ID <= protocol_GameplayMAX); }

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
