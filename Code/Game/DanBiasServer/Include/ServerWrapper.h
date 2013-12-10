#ifndef NETWORK_SERVER_WRAPPER_H
#define NETWORK_SERVER_WRAPPER_H


class SingletonServer
{
public:

	struct INIT_DESC
	{
		bool l;
	};
	void CreateServer(/*DATA*/);
	void StartServer(/*DATA*/);
	void StopServer(/*DATA*/);
	void TerminateServer(/*DATA*/);

	void AttachLobby(/*LOBBY*/);
	void DetachLobby(/*LOBBY*/);
	void KickClient(/*CLIENT*/);
};

#endif // !NETWORK_SERVER_WRAPPER_H
