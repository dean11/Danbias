#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "L_inputClass.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"

namespace DanBias { namespace Client
{
	class GamingUI : public GameStateUI
	{
	public:
		GamingUI( InputClass *input, ::Oyster::Network::NetworkClient *connection, Camera_FPSV2 *camera );
		virtual ~GamingUI();
		bool Init();

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
		Text_UI* text;
		Plane_UI* plane;

		bool key_forward;
		bool key_backward;
		bool key_strafeRight;
		bool key_strafeLeft;
		bool key_Shoot;
		bool key_Jump;

		GamingUI();
		void ReadKeyInput();
	};
} }

#endif