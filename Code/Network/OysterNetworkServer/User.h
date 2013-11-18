#include "ServerInclude.h"
#ifndef USER_H
#define USER_H
enum UserState
{
	OFFLINE,
	OFFLINE_INGAME,
	ONLINE,
	ONLINE_QUEUEING,
	ONLINE_INLOBBY,
	ONLINE_INGAME
};
class User
{
private:
	std::string username;
	int curGame;
	sockaddr_in addr;
	UserState state;
	long lastUpdate;
	HANDLE updMutex;
	Network::ClientToServerUpdateData lastUpdateData;
public:
	void setLastUpdateData(Network::ClientToServerUpdateData data);
	Network::ClientToServerUpdateData getLastUpdateData();
	void setLastUpdate(long upd){lastUpdate=upd;}
	long getLastUpdate() {return lastUpdate;}
	HANDLE threadHandle;
	SOCKET connection;
	User();
	User(int id, sockaddr_in addr, std::string usr="Unknown");
	//SOCKET getTCPSocket() const {return connection;}
	sockaddr_in getAddr() const {return addr;}
	std::string getUsername() const {return username;}
	void setUsername(std::string usr){username=usr;}
	void setState(UserState st){state=st;}
	UserState getState(){return state;}
	void setGame(int gid){curGame=gid;}
	bool isIngame() {return state==ONLINE_INGAME;}
	int getGame(){return curGame;}
};
#endif