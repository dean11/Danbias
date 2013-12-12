#ifndef DANBIASGAME_DANBIASGAME_H
#define DANBIASGAME_DANBIASGAME_H

#if defined (DANBIAS_GAME_DLL_EXPORT)
	#define DANBIAS_GAME_DLL __declspec(dllexport)
#else
	#define DANBIAS_GAME_DLL __declspec(dllimport)
#endif

#define NOMINMAX
#include <Windows.h>



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
			HINSTANCE hinst;
			int nCmdShow;

		};
		LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
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
			
			static HRESULT      InitWindow( HINSTANCE hInstance, int nCmdShow);
			static HRESULT		InitDirect3D();
			static HRESULT		InitInput();

			static HRESULT Update(float deltaTime);
			static HRESULT Render(float deltaTime);
			static HRESULT CleanUp();

			static void protocolRecived();
		private:
			static __int64 cntsPerSec;
			static __int64 prevTimeStamp;
			static float secsPerCnt;
			
			static HINSTANCE			g_hInst;  
			static HWND					g_hWnd;
			static DanBiasGamePrivateData* m_data;
		};
		

	}//End Extern "C"
} //End namespace DanBias

#endif // !DANBIASGAME_DANBIASGAME_H
