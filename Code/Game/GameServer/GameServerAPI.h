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
	};

	extern "C"
	{
		class DANBIAS_SERVER_DLL GameServerAPI
		{
		public:
			struct GameInitDesc
			{
				//stuff
				int connectionPort;
				int maxNumberOfClients;
				bool threaded;
			};
		public:
			static DanBiasServerReturn Create(const GameInitDesc& desc);
		};//End class DanBiasServer
	}//End Extern "C"
} //End namspace DanBias

#endif // !DANBIAS_SERVER_DANBIAS_SERVER_H
