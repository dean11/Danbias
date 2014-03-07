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
			ButtonType_MouseSensitivity,
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

		struct MouseSensitivitySlider
			{
				ButtonRectangle<InGameOptionsUI*> *button;
				Oyster::Graphics::API::Texture mouseSlider;
				bool isHeld;
				Oyster::Math::Float3 pos;
				MouseSensitivitySlider()
				{
					button = 0;
					isHeld = false;
				}
			} mouseSensitivity;

		::Oyster::Event::EventButtonCollection guiElements;
		::Oyster::Math::Float3 mousePos;

		ButtonRectangle<InGameOptionsUI*> *fScreenBtnToggle;

		bool render;

		/* Button events */
		static void OnButtonInteract( ::Oyster::Event::ButtonEvent<InGameOptionsUI*>& msg );

		InGameOptionsUI();

		void CreateMouseSlider();

		 /* Overidden input methods */
		void OnMouseRelease( Input::Enum::SAMI key, Input::Mouse* sender );
		void OnKeyRelease( Input::Enum::SAKI key, Input::Keyboard* sender );
		void OnMouseMoveVelocity( Input::Struct::SAIPointFloat2D coordinate, Input::Mouse* sender );
	};
} }

#endif