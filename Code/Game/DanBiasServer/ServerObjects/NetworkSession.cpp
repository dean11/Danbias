#include "NetworkSession.h"


namespace DanBias
{
	NetworkSession::NetworkSession()
	{
	}
	NetworkSession::~NetworkSession()
	{
	}

	void NetworkSession::AttachClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client)
	{
	}
	void NetworkSession::DetachClient(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client)
	{
	}
	void NetworkSession::DetachClient(short ID)
	{

	}
	
	void NetworkSession::Kick(Utility::DynamicMemory::SmartPointer<Oyster::Network::NetworkClient> client)
	{
	}
	void NetworkSession::Kick()
	{

	}

	Oyster::Network::NetworkClient* NetworkSession::operator[](int Identification)
	{
		return 0;
	}
}//End namespace DanBias