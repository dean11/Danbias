#include "IServer.h"

IServer::IServer()
{

}

IServer::~IServer()
{

}

bool IServer::Init(INIT_DESC& initDesc)
{

	return true;
}

bool IServer::Start()
{

	return true;
}

bool IServer::Stop()
{

	return true;
}

bool IServer::Shutdown()
{

	return true;
}

void IServer::AddSession(ISession* session)
{

}

void IServer::RemoveSession(ISession* session)
{

}