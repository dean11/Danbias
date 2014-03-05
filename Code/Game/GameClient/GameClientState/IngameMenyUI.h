#ifndef DANBIAS_CLIENT_INGAMEMENY_UI_H
#define DANBIAS_CLIENT_INGAMEMENY_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "InputManager.h"
#include "SharedStateContent.h"
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
	
	private: /* Overidden mouse methods */
		

	private:

		// TODO add multiple UI 

		::Input::Mouse *mouseInput;
		Oyster::Math::Float3 mousePos;
		Plane_UI* mouseCursor;
		Plane_UI* background;
		EventButtonCollection menyButtons;

		// HACK remove this later
		Text_UI* debugOutput; 
		IngameMenyUI();
		void ReadKeyInput();
	};
} }

#endif