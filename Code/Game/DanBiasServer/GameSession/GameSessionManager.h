/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAME_SEESION_MANAGER_H
#define DANBIASSERVER_GAME_SEESION_MANAGER_H

#include <string>
#include <Utilities.h>
#include <DynamicArray.h>

namespace DanBias
{
	class LobbyClient;
	class NetworkSession;

	struct GameSessionDescription
	{
		std::wstring mapName;
		Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<LobbyClient>> clients;
		NetworkSession* exitDestionation;	//The new owner when session dies
	};
	struct GameSessionInfo
	{
		std::wstring mapName;
		unsigned int numberOfPlayers;
		float gametime;
	};

	class GameSessionManager
	{
	public:
		/**
		*	Add a new game session.
		*	On success, the function returns the game instance id number greater than 0.
		*/
		static int AddSession(GameSessionDescription& instance, bool run);

		/**
		*	Starts an existing game session
		*	@param session The session id recieved when created.
		*	@param run Indicates if the game session should start imidiatly when created.
		*	@return Returns false if session is not found.
		*/
		static bool StartSession(int session);

		/**
		*	Join an exiting session
		*	@param session The session id recieved when created.
		*	@param client The client that is to join a game session
		*	@return Returns false on failure.
		*/
		static bool JoinSession(int session, Utility::DynamicMemory::SmartPointer<LobbyClient> client);
		
		/**
		*	Gets information about a given session
		*	@param session The session id recieved when created.
		*	@param sessionInformation The output parameter that will be filled.
		*/
		static void GetSessionInfo(int session, GameSessionInfo& sessionInformation);
	
		/**
		*	Close a session.
		*	@param session The session id recieved when created a session.
		*/
		static void CloseSession(int session);

		/**
		*	Get total sessions running
		*	@return Returns the total sessions curently running.
		*/
		static int GetSessionSize();

	private:
		friend class AdminInterface;
	};
}
#endif // !DANBIASSERVER_GAME_SEESION_MANAGER_H
