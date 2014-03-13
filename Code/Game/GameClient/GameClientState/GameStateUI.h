#ifndef DANBIAS_CLIENT_GAMESTATEUI_H
#define DANBIAS_CLIENT_GAMESTATEUI_H

#include "Utilities.h"
#include "NetworkClient.h"
#include "Input.h"
#include "SharedStateContent.h"

namespace DanBias { namespace Client
{
	class GameStateUI : public Input::Mouse::MouseEvent, public Input::Keyboard::KeyboardEvent
	{
	public:
		enum UIState
		{
			UIState_same,
			UIState_previous,
			UIState_gaming,
			UIState_main_menu,
			UIState_shut_down,
			UIState_ingame_meny,
			UIState_ingame_options,
			UIState_resume_game,
			UIState_match_end
		};

		typedef ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> NetEvent;
		static const NetEvent event_processed;

		GameStateUI( SharedStateContent* shared );
		virtual ~GameStateUI();
		virtual UIState Update( float deltaTime ) = 0;
		virtual bool HaveGUIRender() const = 0;
		virtual bool HaveTextRender() const = 0;
		virtual void RenderGUI() = 0;
		virtual void RenderText() = 0;
		virtual bool Release() = 0;
		virtual void ChangeState( UIState next ) = 0;

		virtual void GUIRenderToggle(bool toggle) { toggle; };

		virtual void ActivateInput() = 0;	
		virtual void DeactivateInput() = 0;

		/******************************************************************
		 * @param message of the event
		 * @return message or a reference to GameStateUI::event_processed.
		 ******************************************************************/
		virtual const NetEvent & DataRecieved( const NetEvent &message );

	protected:
		SharedStateContent* shared;
		UIState nextState;
		bool active;
	};
} }

#endif