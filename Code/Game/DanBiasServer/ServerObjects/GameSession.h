#ifndef DANBIASSERVER_GAME_SESSION_H
#define DANBIASSERVER_GAME_SESSION_H

#include "NetworkSession.h"
#include <PostBox\IPostBox.h>
#include <Thread\OysterThread.h>

namespace DanBias
{
	class ClientObject;
	class GameSession :private NetworkSession, public Oyster::Thread::IThreadObject
	{
	public:
		struct GameSessionDescription
		{
			NetworkSession* owner;

		};

	public:
		GameSession();
		virtual~GameSession();

		void Run(const GameSessionDescription& desc);

		void AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box = 0) override;

	private:	//overriden NetworkSession functions
		void Close();
		Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(Oyster::Network::NetworkClient* client) override;
		Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(ClientObject* client) override;
		Utility::DynamicMemory::SmartPointer<ClientObject> DetachClient(short ID) override;
		void Send(Oyster::Network::CustomNetProtocol& protocol) override;
		void Send(Oyster::Network::CustomNetProtocol& protocol, int ID) override;
		void SetPostbox(Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box) override;
		void CloseSession(NetworkSession* clientDestination) override;

	private:	//overriden Threading functions
		void ThreadEntry() override;
		void ThreadExit() override;
		bool DoWork	( ) override;
		
	private:
		void Init();
		void Frame();
		void ParseEvents();

	private:
		NetworkSession* owner;
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box;
		Oyster::Thread::OysterThread worker;

#pragma region TESTING
		void EricLogicInitFunc();
		void EricLogicFrameFunc();
		void EricsLogicTestingProtocalRecieved(ClientObject* reciever, Oyster::Network::CustomNetProtocol& protocol);
#pragma endregion


	};//End GameSession
}//End namespace DanBias
#endif // !DANBIASSERVER_GAME_SESSION_H