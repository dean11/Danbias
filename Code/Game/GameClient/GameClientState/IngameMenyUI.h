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
	
		void ActivateInput();
		void DeactivateInput();

	private: /* Overidden mouse methods */
		

	private:
		// TODO add multiple UI 
		Oyster::Math::Float3 mousePos;
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