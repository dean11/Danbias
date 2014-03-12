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
#define protocol_General_GameOver					102
#define protocol_Broadcast_Test						103
#define protocol_General_Timer						104
#define protocol_GeneralMAX								199


/***********************************/
/********* LOBBY PROTOCOLS ***************************************************************************************************/
/***********[ 200 - 299 ]***********/
#define protocol_LobbyMIN								200	
#define protocol_Lobby_CreateGame					200
#define protocol_Lobby_StartGame					201
#define protocol_Lobby_JoinGame						202
#define protocol_Lobby_Login						203
#define protocol_Lobby_Refresh						204
#define protocol_Lobby_ClientData					205
#define protocol_Lobby_GameData						206
#define protocol_Lobby_QuerryGameType				207
#define protocol_Lobby_ClientReadyState				208
#define protocol_Lobby_Join							209
#define protocol_LobbyMAX								299


/***********************************/
/********* GAMEPLAY PROTOCOLS ***************************************************************************************************/
/***********[ 300 - 399 ]***********/
#define protocol_GameplayMIN							300
#define protocol_Gameplay_PlayerMovementRight		300
#define protocol_Gameplay_PlayerMovementLeft		301
#define protocol_Gameplay_PlayerMovementForward		302
#define protocol_Gameplay_PlayerMovementBackward	303
#define protocol_Gameplay_PlayerLeftTurn			304
#define protocol_Gameplay_PlayerChangeWeapon		305
#define protocol_Gameplay_PlayerShot				306
#define protocol_Gameplay_PlayerJump				307
#define protocol_Gameplay_PlayerScore				308

#define protocol_Gameplay_ObjectPickup				350
#define protocol_Gameplay_ObjectDamage				351
#define protocol_Gameplay_ObjectHealthStatus		352
#define protocol_Gameplay_ObjectPosition			353
#define protocol_Gameplay_ObjectScale				354
#define protocol_Gameplay_ObjectRotation			355
#define protocol_Gameplay_ObjectPositionRotation	356
#define protocol_Gameplay_ObjectEnabled				357
#define protocol_Gameplay_ObjectDisabled			358
#define protocol_Gameplay_ObjectCreate				359
#define protocol_Gameplay_ObjectDelete				360
#define protocol_Gameplay_ObjectCreatePlayer		361
#define protocol_Gameplay_ObjectJoinTeam			362
#define protocol_Gameplay_ObjectLeaveTeam			363
#define protocol_Gameplay_ObjectWeaponCooldown		364
#define protocol_Gameplay_ObjectWeaponEnergy		365
#define protocol_Gameplay_ObjectRespawn				366
#define protocol_Gameplay_ObjectDie					367
#define protocol_Gameplay_ObjectDisconnectPlayer	368
#define protocol_Gameplay_ObjectAction				369
#define protocol_Gameplay_ObjectCollision			370
#define protocol_Gameplay_EffectBeam				371
#define protocol_GameplayMAX							399


/************************************/
/*********** PROTOCOL MACROS ***************************************************************************************************/
/************************************/

inline bool ProtocolIsLobby(short ID) { return (ID >= protocol_LobbyMIN && ID <= protocol_LobbyMAX); }
inline bool ProtocolIsGeneral(short ID) { return (ID >= protocol_GeneralMIN && ID <= protocol_GeneralMAX); }
inline bool ProtocolIsGameplay(short ID) { return (ID >= protocol_GameplayMIN && ID <= protocol_GameplayMAX); }

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
