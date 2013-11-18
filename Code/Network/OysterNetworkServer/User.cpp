#include "User.h"
User::User(int i, sockaddr_in add, std::string usr)
{
	addr=add;
	username=usr;
	curGame=-1;
	connection=NULL;
	state=ONLINE;
	lastUpdate=-1;
	updMutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (updMutex == NULL) 
	{
		printf("CreateMutex error: %d\n", GetLastError());
	}
}
User::User()
{
	username="";
	curGame=-1;
	connection=NULL;
	state=ONLINE;
	lastUpdate=-1;
	updMutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (updMutex == NULL) 
	{
		printf("CreateMutex error: %d\n", GetLastError());
	}
	lastUpdateData.pid=-1;
}
void User::setLastUpdateData(Network::ClientToServerUpdateData data)
{
	WaitForSingleObject(updMutex, INFINITE);
	lastUpdateData=data;
	ReleaseMutex(updMutex);
}
Network::ClientToServerUpdateData User::getLastUpdateData()
{
	WaitForSingleObject(updMutex, INFINITE);
	Network::ClientToServerUpdateData data=lastUpdateData;
	ReleaseMutex(updMutex);
	return data;
}