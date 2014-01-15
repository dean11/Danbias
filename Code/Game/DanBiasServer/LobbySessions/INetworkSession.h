/////////////////////////////////////////
// Created by [Dennis Andersen] [2013] //
/////////////////////////////////////////
#ifndef DANBIASSERVER_INETWORKSESSION_H
#define DANBIASSERVER_INETWORKSESSION_H

class INetworkSession
{
public:
	INetworkSession();
	INetworkSession(const INetworkSession& orig);
	const INetworkSession& operator=(const INetworkSession& orig);
	virtual~INetworkSession();

	inline int GetID() const { return this->id; }

private:
	int id;

};

#endif // !DANBIASSERVER_INETWORKSESSION_H
