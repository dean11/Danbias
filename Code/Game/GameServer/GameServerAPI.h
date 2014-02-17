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
				const wchar_t* serverName;
				int listenPort;
				ServerInitDesc() 
					: serverName(L"Game Server")
					, listenPort(15152)
				{};
			};
			struct GameServerInfo
			{
				int listenPort;
				wchar_t serverIp[255];
			};
			typedef void(*ClientConnectedNotify)(int ID, wchar_t clientAlias[255], wchar_t clientIp[255]);
			typedef void(*ClientDisconnectedNotify)(int ID);

		public:
			static DanBiasServerReturn	ServerInitiate(const ServerInitDesc& desc);
			static void					ServerStart();
			static void					ServerStop();
			static void					ServerUpdate();
			static GameServerInfo		ServerGetInfo();
			static bool					ServerIsRunning();
			static void					NotifyWhenClientConnect(ClientConnectedNotify func);
			static void					NotifyWhenClientDisconnect(ClientDisconnectedNotify func);

			static void					GameSetMapName(const wchar_t* val);
			static void					GameSetMaxClients(const int& val);
			static void					GameSetGameMode(const wchar_t* val);
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
