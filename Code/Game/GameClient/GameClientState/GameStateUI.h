#ifndef DANBIAS_CLIENT_GAMECLIENTSTATE_H
#define DANBIAS_CLIENT_GAMECLIENTSTATE_H

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
			UIState_shut_down
		};

		typedef ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> NetEvent;
		static const NetEvent event_processed;

		GameStateUI();
		virtual ~GameStateUI();
		virtual UIState Update( float deltaTime ) = 0;
		virtual bool HaveGUIRender() const = 0;
		virtual bool HaveTextRender() const = 0;
		virtual void RenderGUI() const = 0;
		virtual void RenderText() const = 0;
		virtual bool Release() = 0;

		/******************************************************************
		 * @param message of the event
		 * @return message or a reference to GameStateUI::event_processed.
		 ******************************************************************/
		virtual const NetEvent & DataRecieved( const NetEvent &message );

	protected:
		UIState nextState;
	};
} }

namespace Utility { namespace DynamicMemory
{ // template specializationto allowuse of dynamicmemory tools
	template<>
	inline void SafeDeleteInstance( ::DanBias::Client::GameStateUI *dynamicInstance )
	{
		if( dynamicInstance )
		{
			dynamicInstance->Release();
			delete dynamicInstance;
		}
	}
} }

#endif