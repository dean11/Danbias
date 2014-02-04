/////////////////////////////////////////
// Created by [Dennis Andersen] [2013] //
/////////////////////////////////////////
#ifndef DANBIAS_SERVER_DANBIAS_SERVER_H
#define DANBIAS_SERVER_DANBIAS_SERVER_H

#include <vld.h>

#define DANBIAS_SERVER

#ifdef DANBIAS_SERVER_DLL_EXPORT
	#define DANBIAS_SERVER_DLL __declspec(dllexport)
#else
	#define DANBIAS_SERVER_DLL __declspec(dllimport)
#endif

namespace DanBias
{
	enum DanBiasServerReturn
	{
		DanBiasServerReturn_Error,
		DanBiasServerReturn_Sucess,
		DanBiasServerReturn_GameNotCreated,
	};

	extern "C"
	{
		class DANBIAS_SERVER_DLL GameServerAPI
		{
		public:
			struct ServerInitDesc
			{
				char* serverName;
				int listenPort;
				bool broadcast;			//Not fully implemented!
				ServerInitDesc() 
					: serverName("Game Server")
					, listenPort(15151)
					, broadcast(true)
				{};
			};
			struct GameServerInfo
			{
				unsigned int listenPort;	// If set to 0, the default port 15151 will be used
				const char *serverIp;		// This cant be mofidfied..
			};

		public:
			static DanBiasServerReturn	ServerInitiate(const ServerInitDesc& desc);
			static void					ServerStart();
			static void					ServerStop();
			static void					ServerUpdate();
			static GameServerInfo		ServerGetInfo();
			static bool					ServerIsRunning();

			static void					GameSetMapId(const int& val);
			static void					GameSetMaxClients(const int& val);
			static void					GameSetGameMode(const int& val);
			static void					GameSetGameTime(const int& val);
			static int					GameGetMapId();
			static int					GameGetMaxClients();
			static int					GameGetGameMode();
			static int					GameGetGameTime();
			static const char*			GameGetGameName();
			static bool					GameStart();


		};//End class DanBiasServer
	}//End Extern "C"
} //End namspace DanBias

#endif // !DANBIAS_SERVER_DANBIAS_SERVER_H
