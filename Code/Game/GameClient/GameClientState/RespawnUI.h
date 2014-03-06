#ifndef DANBIAS_CLIENT_RESPAWN_UI_H
#define DANBIAS_CLIENT_RESPAWN_UI_H

#include "GameStateUI.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"

namespace DanBias { namespace Client
{
	class RespawnUI : public GameStateUI
	{
	public:
		RespawnUI( SharedStateContent* shared );
		virtual ~RespawnUI();
		bool Init();

		// TODO countdown
		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void SetCountdown( float cd );
		void ChangeState( UIState next );

		void ActivateInput();
		void DeactivateInput();

	private:
		float countDown;
		Text_UI* text;
		Text_UI* deathCounter;
		bool render;

		RespawnUI();
	};
} }

#endif