#include "Game.h"
#include "Lobby.h"
//void ControlPlayer( GameLogic::Player& p,const ClientToServerUpdateData &update);
const int NR_CONNECTTHREADS=1;
const int NR_SIMULTCPCONNECTS=1;
//threads can only take 1 argument
struct ThreadArguments;
struct ConnThreadData
{
	sockaddr_in srcAddr;

	ClientToServerUpdateData tmpdata;
	char* buffer;
	int bufLen;
	int dataSize;
};
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
const short TCPSendPort = 11111;
const short TCPRecvPort = 11110;
const short UDPSendPort = 11001;
const short UDPRecvPort = 11000;

class SocketServer
{
private:
	bool serverGameCreationActive;
	HANDLE gameCreateHandle;
	bool serverTCPConnectionLoopActive;
	bool serverUDPReceiveLoopActive;
	bool serverTCPReceiveLoopActive;
	bool setupStatus;
	int iResult;
	WSADATA wsaData;

	SOCKET UDPSocket;
	SOCKET TCPSocket;

	sockaddr_in TCPRecvAddr;
	sockaddr_in UDPRecvAddr;

	int addrSize;

	HANDLE tcpDataHandle[NR_SIMULTCPCONNECTS];
	ConnThreadData tcpData[NR_SIMULTCPCONNECTS];

	HANDLE udpDataHandle[NR_CONNECTTHREADS];
	ConnThreadData connData[NR_CONNECTTHREADS];

	int dataSize;


	char* sendEffectDataBuffer;
	char* sendGameDataBuffer;
	int sendGameDataBufferSize;
	ServerToClientUpdateData sendGameDataStruct;
	std::vector<User> users;
	std::vector<Game> games;
	Lobby lobby;
	int nrActiveSessions;
	std::vector<Event::GameEvent*> sessionEvents;
	std::vector<Network::EffectData> sessionEffects;
	//GameLogic::Session* session;
	std::vector<ServerTimer> timer;
	int DEBUGCTR;
	std::vector<long> updateCount;
	std::vector<float> timeTillUpdate;
	std::vector<::std::string> maps;
	std::string text;
	int playersPerSessionCount;
	int killsRequiredPerSession;
	bool lobbyActive;
public:
	virtual ~SocketServer();
	//Debug force modify functions
	void processAllSessionEvents(int serverID);
	void processAllSessionEffects(int gid);
	void processSessionPlayerData(int serverID);
	//End of debug items
	void updateServers();
	SocketServer();
	bool checkConnection(int userID);
	bool initUDPSocket();
	bool initTCPSocket();
	//void firstTimeConnect();
	bool loadMapList(char* map);
	bool serverGameCreationLoop(int delay);
	bool startThreads();
	static DWORD activateUDPReceiveLoop(ThreadArguments* tra);
	void stopUDPReceiveLoops();
	//TCP functions
	static DWORD activateTCPConnectLoop(ThreadArguments* tra);
	void receiveConnection(int threadID);
	//End of TCP functions
	bool sendData(int uid, const char*, int);
	bool sendGameData(int serverID);
	bool sendKeyFrameData(int size, const char* data);
	void sendInitData(int gid);
	void sendRenderData(int gid);
	void sendEventData(int gid, int size);
	void sendLobbyInitData(int lid);
	bool closeConnection();
	void receiveDataUDP(int threadID);

	static DWORD activateServerGameLoop(ThreadArguments* tra);
	void startGameCreateLoop(int delay);
	void stopGameCreateLoop();
	void parseReceivedData(int threadID/*char*, int*/);//char and int required if i don't want to use the class buffer
	void ParseReceivedData(ConnThreadData* data);

	void parseServercommand(int pid, int threadID);
	void parseData(int pid, int gid, int threadID);
	void parseMessage(int pid, int threadID);

	void addUser(int threadID);
	void AddUser(ConnThreadData* data);
	void removeUser(int id);

	bool isReady() const {return setupStatus;}
	bool LoadInitData(char* maploc);
};
struct ThreadArguments
{
	SocketServer* ptr;
	int threadID;
};