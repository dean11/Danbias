#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "L_inputClass.h"
#include "Camera_FPSV2.h"

namespace DanBias { namespace Client
{
	class GamingUI : public GameStateUI
	{
	public:
		GamingUI( InputClass *input, ::Oyster::Network::NetworkClient *connection, Camera_FPSV2 *camera );
		virtual ~GamingUI();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI() const;
		void RenderText() const;
		bool Release();

	private:
		InputClass *input;
		::Oyster::Network::NetworkClient *netClient;
		Camera_FPSV2 *camera;

		GamingUI();
		void ReadKeyInput();
	};
} }

#endif