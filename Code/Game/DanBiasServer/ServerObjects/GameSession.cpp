
#include <GameProtocols.h>
#include "GameSession.h"
#include <PostBox\PostBox.h>
#include "ClientObject.h"


#define ERIK

using namespace Utility::DynamicMemory;
using namespace Oyster::Network;
using namespace Oyster;

namespace DanBias
{
	GameSession::GameSession()
	{

	}
	GameSession::~GameSession()
	{

	}

	void GameSession::Run(const GameSessionDescription& desc)
	{

	}

	void GameSession::AttachClient(SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box ) 
	{
		this->Init();
	}

////private:	//overriden NetworkSession functions
	void GameSession::Close()
	{

	}
	SmartPointer<ClientObject> GameSession::DetachClient(NetworkClient* client) 
	{
		return SmartPointer<ClientObject>();
	}
	SmartPointer<ClientObject> GameSession::DetachClient(ClientObject* client) 
	{
		return SmartPointer<ClientObject>();
	}
	SmartPointer<ClientObject> GameSession::DetachClient(short ID) 
	{
		return SmartPointer<ClientObject>();
	}
	void GameSession::Send(::CustomNetProtocol& protocol) 
	{

	}
	void GameSession::Send(CustomNetProtocol& protocol, int ID) 
	{

	}
	void GameSession::SetPostbox(IPostBox<NetworkSession::NetEvent> *box) 
	{

	}
	void GameSession::CloseSession(NetworkSession* clientDestination) 
	{

	}

////private:	//overriden Threading functions
	void GameSession::ThreadEntry()
	{

	}
	void GameSession::ThreadExit()
	{

	}
	bool GameSession::DoWork	( )
	{
		this->ParseEvents();
		this->Frame();

		return true;
	}
	
////private:
	void GameSession::Init()
	{
#ifdef ERIK
		EricLogicInitFunc();
#else
		
#endif
	}
	void GameSession::Frame()
	{
#ifdef ERIK
		EricLogicFrameFunc();
#else
		
#endif
	}
	void GameSession::ParseEvents()
	{
		if(this->box && !this->box->IsEmpty())
		{
			NetEvent &e = this->box->Fetch();

#ifdef ERIK
			EricsLogicTestingProtocalRecieved(e.reciever, e.protocol);
#else
			if(e.protocol[0].type != Oyster::Network::NetAttributeType_Short) return;

			short f = e.protocol[0].value.netShort;

			switch (f)
			{
				default:
					
				break;
			}
#endif
		}
	}


#pragma region TESTING

	//VARIABLES GOES HERE
	int i = 0;
	GameLogic::Player erik;

		void GameSession::EricLogicInitFunc()
		{
		
		}
		void GameSession::EricLogicFrameFunc()
		{
			
		}
		void GameSession::EricsLogicTestingProtocalRecieved(ClientObject* reciever, CustomNetProtocol& protocol)
		{
			
		}
#pragma endregion

}//End namespace DanBias

