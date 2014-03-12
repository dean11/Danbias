/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_CLIENT_OBJECT_H
#define DANBIASSERVER_CLIENT_OBJECT_H

#include <NetworkClient.h>
#include <NetworkSession.h>
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
		enum ClientState
		{
			ClientState_CreatingGame,
			ClientState_Ready,
			ClientState_Invalid,
		};

	public:
		GameClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> nwClient);
		virtual~GameClient();

		inline bool operator==(const GameLogic::IPlayerData* c)				{ return (this->player) ? (c->GetID() == this->player->GetID()) : false ; }
		inline bool operator==(const Oyster::Network::NetworkClient* c)		{ return (c->GetID() == this->client->GetID()); }

		inline bool Equals(const GameLogic::IPlayerData* c)					{ return (this->player) ? (c->GetID() == this->player->GetID()) : false ; }
		inline bool Equals(const Oyster::Network::NetworkClient* c)			{ return (c->GetID() == this->client->GetID()); }

		inline float					GetSinceLastResponse()		const { return this->secondsSinceLastResponse; }
		inline std::wstring				GetAlias()					const { return this->alias; }
		inline std::wstring				GetCharacter()				const { return this->character; }
		inline bool						IsReady()					const { return this->isReady; }
		inline GameLogic::IPlayerData*	GetPlayer()					const { return this->player; }
		Oyster::Network::NetClient		GetClient()					const { return this->client; }
		ClientState						GetState()					const { return this->state; }
		int								GetFailedProtocolCount()	const { return this->failedPackagesCount; }


		void SetPlayer(GameLogic::IPlayerData* player);
		void SetReadyState(bool isReady);
		void SetAlias(std::wstring alias);
		void SetCharacter(std::wstring character);
		void SetSinceLastResponse(float seconds);
		void SetState(ClientState state);

		void ReleasePlayer();
		Oyster::Network::NetClient ReleaseClient();

		bool IsInvalid();
		void Invalidate();
		int IncrementFailedProtocol();
		void ResetFailedProtocolCount();

		//NetworkSpecific
		void SetOwner(Oyster::Network::NetworkSession* owner);
		void UpdateClient();

	private:
		GameLogic::IPlayerData* player;
		Oyster::Network::NetClient client;
		
		bool isReady;
		bool isInvalid;
		float secondsSinceLastResponse;
		int failedPackagesCount;

		std::wstring alias;
		std::wstring character;

		ClientState state;
	};
}//End namespace DanBias

typedef Utility::DynamicMemory::SmartPointer<DanBias::GameClient> gClient;

#endif // !DANBIASSERVER_CLIENT_OBJECT_H
