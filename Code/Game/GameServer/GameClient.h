/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_CLIENT_OBJECT_H
#define DANBIASSERVER_CLIENT_OBJECT_H

#include <NetworkClient.h>
#include <PostBox\PostBox.h>
#include <GameAPI.h>
#include <Utilities.h>

namespace DanBias
{
	/**
	*	Container to keep logic player and network client together as a unit.
	*/
	class GameClient
	{
	public:
		GameClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client, GameLogic::IPlayerData* player);
		virtual~GameClient();

		GameLogic::IPlayerData* GetPlayer();
		GameLogic::IPlayerData* ReleasePlayer();
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> GetClient();
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> ReleaseClient();

		float GetSinceLastResponse() const;
		bool IsReady() const;
		bool Equals(const Oyster::Network::NetworkClient* c);

		void SetReadyState(bool isReady);
		void SetSinceLastResponse(float seconds);

	private:
		GameLogic::IPlayerData* player;
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client;
		bool isReady;
		float secondsSinceLastResponse;
	};

}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
