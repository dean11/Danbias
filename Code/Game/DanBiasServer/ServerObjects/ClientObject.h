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
		ClientObject(const Oyster::Network::NetworkClient& client);
		~ClientObject();

		void SetPostbox(Oyster::PostBox<NetworkSession::ClientEvent>* box);

		GameLogic::Player*				Logic_Object();
		Oyster::Network::NetworkClient* NetClient_Object();
	
		/** This method is NOT threadsafe. */
		virtual void ProtocolRecievedCallback(Oyster::Network::CustomNetProtocol& protocol) override;
	
	private:
		GameLogic::Player logicPlayer;
		Oyster::Network::NetworkClient client;
		Oyster::IPostBox<DanBias::NetworkSession::ClientEvent>* box;
	};

}//End namespace DanBias
#endif // !DANBIASSERVER_CLIENT_OBJECT_H
