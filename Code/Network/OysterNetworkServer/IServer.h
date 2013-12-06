#ifndef OYSTER_NETWORK_SERVER_I_SERVER_H
#define OYSTER_NETWORK_SERVER_I_SERVER_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

class IServer
{
	class ISession;

public:
	struct INIT_DESC
	{

	};

	IServer();
	virtual ~IServer();

	virtual bool Init(INIT_DESC& initDesc);
	virtual bool Start();
	virtual bool Stop();
	virtual bool Shutdown();

	virtual void AddSession(ISession* session);
	virtual void RemoveSession(ISession* session);

private:


};

#endif