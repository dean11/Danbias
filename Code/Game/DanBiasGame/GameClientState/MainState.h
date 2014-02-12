#ifndef DANBIAS_CLIENT_MAINSTATE_H
#define DANBIAS_CLIENT_MAINSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "NetworkClient.h"
#include <string>
namespace DanBias
{
	namespace Client
	{
		class MainState : public GameClientState
		{
		private:
			Oyster::Network::NetworkClient* nwClient;
			struct myData;
			myData* privData;
		public:
			MainState(void);
			~MainState(void);
			bool Init(Oyster::Network::NetworkClient* nwClient);
			bool LoadModels(std::wstring file);
			bool InitCamera(Oyster::Math::Float3 startPos);
			ClientState Update(float deltaTime, InputClass* KeyInput);
			
			bool Render();
			bool Release();

			void DataRecieved( ::Oyster::Network::NetEvent<NetworkClient*, ClientEventArgs> e );
		};
	}
}
#endif // ! DANBIAS_CLIENT_LOGINSTATE_H