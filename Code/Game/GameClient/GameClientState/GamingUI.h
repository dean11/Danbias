#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "InputManager.h"
#include "SharedStateContent.h"

namespace DanBias { namespace Client
{
	class GamingUI : public GameStateUI, Input::Mouse::MouseEvent, Input::Keyboard::KeyboardEvent
	{
	public:
		GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera );
		virtual ~GamingUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void SetHPtext( std::wstring hp );
		void SetEnergyText( std::wstring energy );
		void SetKillMessage( std::wstring killerMessage );
		void ChangeState( UIState next );
		void StopGamingUI();
	private: /* Overidden mouse methods */
		void OnMousePress			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseRelease			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseMoveVelocity	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )		override;
		void OnMouseScroll			( int delta, Input::Mouse* sender )										override;

		void OnKeyPress				( Input::Enum::SAKI key, Input::Keyboard* sender)						override;
		void OnKeyRelease			( Input::Enum::SAKI key, Input::Keyboard* sender)						override;

	private:
		struct WeaponData
		{
			Utility::DynamicMemory::SmartPointer<Plane_UI> crosshair;
			int id;
		};

	private:
		SharedStateContent *sharedData;
		Camera_FPSV2 *camera;

		// TODO add multiple UI elements
		Text_UI* hp;
		Text_UI* energy;
		Text_UI** killMessages;
		int maxMessageCount;
		float message_Timer;
		
		std::vector<WeaponData> weapons;
		int currentWeapon;
		bool key_forward;
		bool key_backward;
		bool key_strafeRight;
		bool key_strafeLeft;
		bool mouse_secondDown;

		GamingUI();
		void ReadKeyInput();
	};
} }

#endif