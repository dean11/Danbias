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

			virtual bool Render();
			virtual bool Release();
			void ChangeState( ClientState next );

		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		};
	}
}

#endif