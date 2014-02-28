#ifndef DANBIAS_CLIENT_GAMESTATEUI_H
#define DANBIAS_CLIENT_GAMESTATEUI_H

#include "Utilities.h"
#include "NetworkClient.h"

namespace DanBias { namespace Client
{
	class GameStateUI
	{
	public:
		enum UIState
		{
			UIState_same,
			UIState_gaming,


			UIState_main_menu,
			UIState_shut_down,
			UIStat_inGameMeny,
			UIStat_resumeGame
		};

		typedef ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> NetEvent;
		static const NetEvent event_processed;

		GameStateUI();
		virtual ~GameStateUI();
		virtual UIState Update( float deltaTime ) = 0;
		virtual bool HaveGUIRender() const = 0;
		virtual bool HaveTextRender() const = 0;
		virtual void RenderGUI() = 0;
		virtual void RenderText() = 0;
		virtual bool Release() = 0;
		virtual void ChangeState( UIState next ) = 0;
		/******************************************************************
		 * @param message of the event
		 * @return message or a reference to GameStateUI::event_processed.
		 ******************************************************************/
		virtual const NetEvent & DataRecieved( const NetEvent &message );

	protected:
		UIState nextState;
	};
} }

#endif