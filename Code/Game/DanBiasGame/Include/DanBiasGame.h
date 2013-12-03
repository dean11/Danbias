#ifndef DANBIASGAME_DANBIASGAME_H
#define DANBIASGAME_DANBIASGAME_H

#if defined (DANBIAS_GAME_DLL_EXPORT)
	#define DANBIAS_GAME_DLL __declspec(dllexport)
#else
	#define DANBIAS_GAME_DLL __declspec(dllimport)
#endif

namespace DanBias
{
	extern "C"
	{
		enum DanBiasClientReturn
		{
			DanBiasClientReturn_Error,
			DanBiasClientReturn_Sucess,
		};

		struct DanBiasGameDesc
		{
			//Stuff goes here...
			int port;
		};

		class DANBIAS_GAME_DLL DanBiasGame
		{
		public:
			static DanBiasClientReturn Initiate(DanBiasGameDesc& desc);
			static DanBiasClientReturn Run();
			static void Release();
		};

	}//End Extern "C"
} //End namspace DanBias

#endif // !DANBIASGAME_DANBIASGAME_H
