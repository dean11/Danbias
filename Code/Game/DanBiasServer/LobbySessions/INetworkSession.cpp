#include "INetworkSession.h"
#include <GID.h>

INetworkSession::INetworkSession()
	:id(GID())
{}
INetworkSession::INetworkSession(const INetworkSession& orig)
{
	id = orig.id;
}
const INetworkSession& INetworkSession::operator=(const INetworkSession& orig)
{
	id = orig.id;
	return *this;
}
INetworkSession::~INetworkSession()	
{}

