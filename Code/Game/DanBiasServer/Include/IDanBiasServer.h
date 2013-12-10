/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIAS_SERVER_DANBIAS_SERVER_H
#define DANBIAS_SERVER_DANBIAS_SERVER_H


#if defined (DANBIAS_SERVER_DLL_EXPORT)
	#define DANBIAS_SERVER_DLL __declspec(dllexport)
#else
	#define DANBIAS_SERVER_DLL __declspec(dllimport)
#endif

namespace DanBias
{
	extern "C"
	{
		enum DanBiasServerReturn
		{
			DanBiasServerReturn_Error,
			DanBiasServerReturn_Sucess,
		};

		struct DanBiasServerDesc
		{
			//Stuff goes here...
			int port;
		};

		class DANBIAS_SERVER_DLL DanBiasServer
		{
		public:
			static DanBiasServerReturn Initiate(DanBiasServerDesc& desc);
			static DanBiasServerReturn Run();
			static DanBiasServerReturn Release();
		};

	}//End Extern "C"
} //End namspace DanBias

#endif // !DANBIAS_SERVER_DANBIAS_SERVER_H
