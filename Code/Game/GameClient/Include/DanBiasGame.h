/////////////////////////////////////////
// Created by [Dennis Andersen] [2013] //
/////////////////////////////////////////
#ifndef DANBIASGAME_DANBIASGAME_H
#define DANBIASGAME_DANBIASGAME_H

#define DANBIAS_CLIENT

#if defined (DANBIAS_GAME_DLL_EXPORT)
	#define DANBIAS_GAME_DLL __declspec(dllexport)
#else
	#define DANBIAS_GAME_DLL __declspec(dllimport)
#endif

#define NOMINMAX
#include <Windows.h>
#include <vld.h>

namespace DanBias
{
	extern "C"
	{
		enum DanBiasClientReturn
		{
			DanBiasClientReturn_Error,
			DanBiasClientReturn_Success
		};

		struct DanBiasGameDesc
		{
			//Stuff goes here...
			int port;
			const char* IP; 
			HINSTANCE hinst;
			int nCmdShow;

		};
		
		class DanBiasGamePrivateData;
		class DANBIAS_GAME_DLL DanBiasGame
		{
		public:
			//--------------------------------------------------------------------------------------
			// Interface API functions
			//--------------------------------------------------------------------------------------
			static DanBiasClientReturn Initiate(DanBiasGameDesc& desc);
			static DanBiasClientReturn Run();
			static void Release();

		private:
			enum Result
			{
				Result_continue,
				Result_quit,
				Result_error
			};

			static HRESULT	InitDirect3D();
			static HRESULT	InitInput( HWND handle );
			static HRESULT  InitSound();

			static Result Update(float deltaTime);
			static HRESULT Render();
			static HRESULT CleanUp();
		};
		

	}//End Extern "C"
} //End namespace DanBias

#endif // !DANBIASGAME_DANBIASGAME_H
