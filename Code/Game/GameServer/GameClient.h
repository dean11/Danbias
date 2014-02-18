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

		inline bool operator==(const GameLogic::IPlayerData* c)				{ return (this->player) ? (c->GetID() == this->player->GetID()) : false ; }
		inline bool operator==(const Oyster::Network::NetworkClient* c)		{ return (c->GetID() == this->GetID()); }

		inline float		GetSinceLastResponse()		const { return this->secondsSinceLastResponse; }
		inline std::wstring	GetAlias()					const { return this->alias; }
		inline std::wstring	GetCharacter()				const { return this->character; }
		inline bool			IsReady()					const { return this->isReady; }
		inline GameLogic::IPlayerData* GetPlayer()		const { return this->player; }


		void SetPlayer(GameLogic::IPlayerData* player);
		void SetReadyState(bool isReady);
		void SetAlias(std::wstring alias);
		void SetCharacter(std::wstring character);
		void SetSinceLastResponse(float seconds);

	private:
		GameLogic::IPlayerData* player;
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client;
		
		bool isReady;
		float secondsSinceLastResponse;

		std::wstring alias;
		std::wstring character;
	};
}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
