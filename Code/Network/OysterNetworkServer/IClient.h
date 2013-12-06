#ifndef OYSTER_NETWORK_SERVER_I_CLIENT_H
#define OYSTER_NETWORK_SERVER_I_CLIENT_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

class IClient
{
public:
	virtual ~IClient() {}

	virtual void Disconnect() {};
	virtual bool IsConnected() {};

	virtual void Send() {};

private:

};

#endif