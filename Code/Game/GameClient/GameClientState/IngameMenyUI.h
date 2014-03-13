#ifndef DANBIAS_CLIENT_INGAMEMENY_UI_H
#define DANBIAS_CLIENT_INGAMEMENY_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "InputManager.h"
#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"
#include "Buttons\ButtonEllipse.h"

using namespace ::Oyster::Event;
namespace DanBias { namespace Client
{
	class IngameMenyUI : public GameStateUI
	{
	public:
		IngameMenyUI( SharedStateContent* shared );
		virtual ~IngameMenyUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void ChangeState( UIState next );
		void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );
	
		void ActivateInput();
		void DeactivateInput();

	private: /* Overidden mouse methods */
		void OnMouseMoveVelocity( Input::Struct::SAIPointFloat2D key, Input::Mouse* sender ) override;
		void OnMouseRelease( Input::Enum::SAMI key, Input::Mouse* sender );
		void OnKeyRelease( Input::Enum::SAKI key, Input::Keyboard* sender );

	private:
		::Oyster::Math::Float3 mousePos;
		C_AudioHandler* audioHandler;
		Plane_UI* mouseCursor;
		Plane_UI* background;
		EventButtonCollection menyButtons;
		bool render;

		// HACK remove this later
		Text_UI* debugOutput; 
		IngameMenyUI();
	};
} }

#endif