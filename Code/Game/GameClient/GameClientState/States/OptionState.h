#ifndef GAMECLIENT_OPTIONSTATE_H
#define GAMECLIENT_OPTIONSTATE_H

#include "..\GameClientState.h"
#include "..\Buttons\ButtonRectangle.h"
#include "DllInterfaces\GFXAPI.h"
#include "EventHandler\EventHandler.h"

namespace DanBias 
{ 
	namespace Client
	{
		class OptionState:	public GameClientState, Input::Mouse, Input::Keyboard
		{
		public:
			OptionState();
			~OptionState();

		public: // Overriden from GameClientState
			bool Init( SharedStateContent &shared )						override;
			ClientState Update( float deltaTime )						override;
			bool Render()												override;
			bool Release()												override;
			void ChangeState( ClientState next )						override;
			const NetEvent & DataRecieved( const NetEvent &message )	override;

		private:
			GameClientState::ClientState nextState;
			SharedStateContent* sharedData;
			
			Oyster::Event::EventButtonCollection guiElements;
		};
	}
}

#endif // !GAMECLIENT_OPTIONSTATE_H
