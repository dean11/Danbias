#ifndef DANBIAS_CLIENT_RESPAWN_UI_H
#define DANBIAS_CLIENT_RESPAWN_UI_H

#include "GameStateUI.h"

namespace DanBias { namespace Client
{
	class RespawnUI : public GameStateUI
	{
	public:
		RespawnUI( ::Oyster::Network::NetworkClient *connection, float delay );
		virtual ~RespawnUI();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI() const;
		void RenderText() const;
		bool Release();

	private:
		::Oyster::Network::NetworkClient *netClient;
		float countDown;

		RespawnUI();
	};
} }

#endif