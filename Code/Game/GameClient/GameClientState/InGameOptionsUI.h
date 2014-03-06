#ifndef DANBIAS_CLIENT_INGAME_OPTIONS_UI_H
#define DANBIAS_CLIENT_INGAME_OPTIONS_UI_H

#include "GameStateUI.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"

namespace DanBias { namespace Client
{
	class InGameOptionsUI : public GameStateUI
	{
	public:
		InGameOptionsUI( SharedStateContent* shared );
		virtual ~InGameOptionsUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void ChangeState( UIState next );
	
		void ActivateInput();
		void DeactivateInput();

	private:
		enum ButtonType
		{
			ButtonType_FullScreen,
			ButtonType_FlipResLeft,
			ButtonType_FlipResRight,
			ButtonType_Apply,
			ButtonType_Back,
		};

		struct OptionsData
		{
			int currentRes;
			bool toogleFullScreen;
			Oyster::Math::Float2 resolution;
			bool toggleSound;
			bool invertMouse;
			OptionsData()
			{
				::Oyster::Graphics::API::Option op = ::Oyster::Graphics::API::GetOption();
				toogleFullScreen = op.fullscreen;
				resolution = op.resolution;
				invertMouse = false;
				toggleSound = true;
				currentRes = 5; //720p
			}
		} options;

		::Oyster::Event::EventButtonCollection guiElements;
		::Oyster::Math::Float3 mousePos;

		ButtonRectangle<InGameOptionsUI*> *fScreenBtnToggle;

		bool render;

		/* Button events */
		static void OnButtonInteract( ::Oyster::Event::ButtonEvent<InGameOptionsUI*>& msg );

		InGameOptionsUI();

		 /* Overidden input methods */
		void OnMouseRelease( Input::Enum::SAMI key, Input::Mouse* sender );
		void OnKeyRelease( Input::Enum::SAKI key, Input::Keyboard* sender );
	};
} }

#endif