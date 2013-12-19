/////////////////////////////////////////////////////////////////////
// Created 2013 by:
// [Dennis Andersen], [Linda Andersson]
/////////////////////////////////////////////////////////////////////
#ifndef GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
#define GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H

/* THERE CAN ABSOLUTLEY NOT BE TWO DEFINITIONS WITH THE SAME ID!! */



/***********************************/
/********* RESERVERD PROTOCOLS *****/
/**********   [ 0 - 96 ]  *********/
#define protocol_TypeId_Lobby						97
#define protocol_TypeId_General						98
#define protocol_TypeId_Gameplay					99


/***********************************/
/********* GENERAL PROTOCOLS *******/
/***********[ 200 - 300 ]***********/
#define protocol_General_Disconnect					100
#define protocol_General_Ping						101
#define protocol_General_Text						102


/***********************************/
/********* LOBBY PROTOCOLS *********/
/***********[ 100 - 200 ]***********/
#define protocol_Lobby_CreateGame					200
#define protocol_Lobby_JoinGame						201
#define protocol_Lobby_StartGame					202
#define protocol_Lobby_JoinLobby					203
#define protocol_Lobby_LeaveLobby					204
#define protocol_Lobby_CreateGameLobby				205


/***********************************/
/********* GAMEPLAY PROTOCOLS ******/
/***********[ 300 - 400 ]***********/
#define protocol_Gamplay_PlayerNavigation			300
#define protocol_Gamplay_PlayerMouseMovement		301
#define protocol_Gamplay_PlayerPosition				302
#define protocol_Gamplay_CreateObject				303
#define protocol_Gamplay_ObjectPosition				304


/***********************************/
/*********** TEST PROTOCOLS ********/
/***********[ 1000 - x ]************/
#define PROTOCOL_TEST								1000

#endif // !GAMEPROTOCOL_PROTOCOL_DEFINITION_ID_H
