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
	class GamingUI : public GameStateUI, Input::Mouse::MouseEvent
	{
	public:
		GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera );
		virtual ~GamingUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI() const;
		void RenderText() const;
		bool Release();
		void SetHPtext( std::wstring hp );

	private: /* Overidden mouse methods */
		void OnMouse				( const Input::Struct::MouseEventData& eventData )						override { }
		void OnMousePress			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseDown			( Input::Enum::SAMI key, Input::Mouse* sender )							override { }
		void OnMouseRelease			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseMovePixelPos	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )		override { }
		void OnMouseMoveVelocity	( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )		override;
		void OnMouseScroll			( int delta, Input::Mouse* sender )										override { }

	private:
		SharedStateContent *sharedData;
		Camera_FPSV2 *camera;

		// TODO add multiple UI elements
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