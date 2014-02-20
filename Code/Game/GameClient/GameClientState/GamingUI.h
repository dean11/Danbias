#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"

namespace DanBias { namespace Client
{
	class GamingUI : public GameStateUI
	{
	public:
		GamingUI( ::Input::Mouse *mouseInput, ::Input::Keyboard *keyboardInput, ::Oyster::Network::NetworkClient *connection, Camera_FPSV2 *camera );
		virtual ~GamingUI();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI() const;
		void RenderText() const;
		bool Release();

	private:
		::Input::Mouse *mouseInput;
		::Input::Keyboard *keyboardInput;
		::Oyster::Network::NetworkClient *netClient;
		Camera_FPSV2 *camera;

		GamingUI();
		void ReadKeyInput();
	};
} }

#endif