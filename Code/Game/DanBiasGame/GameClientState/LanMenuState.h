#ifndef LAN_MENU_STATE_H
#define LAN_MENU_STATE_H

#include "GameClientState.h"
#include "OysterMath.h"

namespace DanBias
{
	namespace Client
	{
		class LanMenuState : public GameClientState
		{
		public:
			LanMenuState();
			virtual ~LanMenuState();

			virtual bool Init(Oyster::Network::NetworkClient* nwClient);
			virtual ClientState Update(float deltaTime, InputClass* KeyInput);

			ClientState ChangeState(InputClass* KeyInput);

			bool LoadModels(std::wstring file);
			bool InitCamera(Oyster::Math::Float3 startPos);

			virtual bool Render(float dt);
			virtual bool Release();
			virtual void Protocol(ProtocolStruct* protocolStruct);

			void PlayerJoinProtocol(PlayerName* name);

		private:
			Oyster::Network::NetworkClient* nwClient;
			struct myData;
			myData* privData;
		};
	}
}

#endif