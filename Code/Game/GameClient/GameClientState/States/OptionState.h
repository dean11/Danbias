#ifndef GAMECLIENT_OPTIONSTATE_H
#define GAMECLIENT_OPTIONSTATE_H

#include "..\GameClientState.h"
#include "..\Buttons\ButtonRectangle.h"
#include "DllInterfaces\GFXAPI.h"
#include "EventHandler\EventHandler.h"
#include "..\GameClientState.h"

namespace DanBias 
{ 
	namespace Client
	{
		class OptionState:	public GameClientState, Input::Mouse::MouseEvent, Input::Keyboard::KeyboardEvent
		{
		public:
			OptionState();
			~OptionState();

		public: // Overriden from GameClientState
			bool Init( SharedStateContent &shared )						override;
			ClientState Update( float deltaTime )						override;
			bool Render()												override;
			bool Release()												override;
			void ChangeState( ClientState next )						override;
			//void PlaySound( Sound::ISound* sound, Sound::IChannel* channel, PlayMode playMode ) override;
			void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );
			const DanBias::Client::GameClientState::NetEvent & DataRecieved( const NetEvent &message )	override;

		private: /* Derived mouse events */
			void OnMouse				( const Input::Struct::MouseEventData& eventData ) override;
			void OnMousePress			( Input::Enum::SAMI key, Input::Mouse* sender ) override;
			void OnMouseDown			( Input::Enum::SAMI key, Input::Mouse* sender ) override;
			void OnMouseRelease			( Input::Enum::SAMI key, Input::Mouse* sender ) override;
			void OnMouseMovePixelPos	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender ) override;
			void OnMouseMoveVelocity	( Input::Struct::SAIPointFloat2D coordinate, Input::Mouse* sender ) override;
			void OnMouseScroll			( int delta, Input::Mouse* sender ) override;
		
		private: /* Derived keyboard events */
			void OnKeyEvent		(const Input::Struct::KeyboardEventData& eventData) override;
			void OnKeyPress		(Input::Enum::SAKI key, Input::Keyboard* sender) override;
			void OnKeyDown		(Input::Enum::SAKI key, Input::Keyboard* sender) override;
			void OnKeyRelease	(Input::Enum::SAKI key, Input::Keyboard* sender) override;

		private: /* Button events */
			static void OnButtonInteract(Oyster::Event::ButtonEvent<OptionState*>& e);

		private:
			void CreateMouseSlider();

		private:
			GameClientState::ClientState nextState;
			SharedStateContent* sharedData;

			Oyster::Event::EventButtonCollection guiElements;
			Oyster::Math::Float3 musOrientation;
			
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
				float mouseSensitivity;
				bool mute;
				OptionsData()
				{
					Oyster::Graphics::API::Option op = Oyster::Graphics::API::GetOption();
					toogleFullScreen = op.fullscreen;
					resolution = op.resolution;
					invertMouse = false;
					toggleSound = true;
					currentRes = 5; //720p
				}
			} options;
			struct MouseSensitivitySlider
			{
				ButtonRectangle<OptionState*> *button;
				Oyster::Graphics::API::Texture mouseSlider;
				bool isHeld;
				Oyster::Math::Float3 pos;
				MouseSensitivitySlider()
				{
					button = 0;
					isHeld = false;
				}
			} mouseSensitivity;

			ButtonRectangle<OptionState*> *fScreenBtnToggle;
		};
	}
}

#endif // !GAMECLIENT_OPTIONSTATE_H
