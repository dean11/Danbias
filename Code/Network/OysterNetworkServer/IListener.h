#ifndef NETWORK_SERVER_ILISTENER_H
#define NETWORK_SERVER_ILISTENER_H

/////////////////////////////////////////////////////////////////////
// Created by Pontus 2013
/////////////////////////////////////////////////////////////////////

class IListener
{
public:
	virtual bool Init(unsigned int port) = 0;
	virtual int Accept() = 0;

};

#endif