#ifndef DANBIASSERVER_CLIENT_OBJECT_H
#define DANBIASSERVER_CLIENT_OBJECT_H


#include "NetworkSession.h"
#include "NetworkClient.h"
#include <PostBox\PostBox.h>
#include <Player.h>

namespace DanBias
{
	class ClientObject 
		:public Oyster::Network::ProtocolRecieverObject
	{
	public:
		ClientObject(Oyster::Network::NetworkClient* client);
		virtual~ClientObject();

		void SetPostbox(Oyster::IPostBox<NetworkSession::NetEvent>* box);
		void SetProtocolCallback(Oyster::Network::ProtocolRecieverObject* object);

		GameLogic::Player*				Logic_Object();
		Oyster::Network::NetworkClient* NetClient_Object();
	
	private:
		/** This method is NOT threadsafe. */
		virtual void ProtocolRecievedCallback(Oyster::Network::CustomNetProtocol& protocol) override;
	
	private:
		GameLogic::Player logicPlayer;
		Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client;
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent>* box;
	};

}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
