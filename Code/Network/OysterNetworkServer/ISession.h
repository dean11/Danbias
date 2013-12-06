#ifndef OYSTER_NETWORK_SERVER_I_SESSION_H
#define OYSTER_NETWORK_SERVER_I_SESSION_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

class ISession
{
	class IClient;
public:
	struct INIT_DESC
	{

	};

	ISession();
	virtual ~ISession();

	virtual bool Init();
	virtual bool Start();
	virtual bool Stop();
	virtual bool Shutdown();

	virtual void SendToAll();

	virtual void AddClient(IClient* client);
	virtual void RemoveClient(IClient* client);

private:
};


#endif