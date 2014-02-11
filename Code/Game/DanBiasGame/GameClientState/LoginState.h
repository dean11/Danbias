#ifndef DANBIAS_CLIENT_LOGINSTATE_H
#define DANBIAS_CLIENT_LOGINSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "NetworkClient.h"
#include <string>
namespace DanBias
{
	namespace Client
	{

		class LoginState : public GameClientState
		{
		private:
			Oyster::Network::NetworkClient* nwClient;
			struct myData;
			myData* privData;
		public:
			LoginState(void);
			~LoginState(void);
			bool Init(Oyster::Network::NetworkClient* nwClient);
			bool LoadModels(std::wstring file);
			bool InitCamera(Oyster::Math::Float3 startPos);
			ClientState Update(float deltaTime, InputClass* KeyInput);
			
			bool Render(float dt);
			bool Release();
			void Protocol(ProtocolStruct* protocol)override;
			void PlayerJoinProtocol(PlayerName* name);

		};};};
#endif // ! DANBIAS_CLIENT_LOGINSTATE_H