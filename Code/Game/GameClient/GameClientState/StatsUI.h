#ifndef DANBIAS_CLIENT_STATS_UI_H
#define DANBIAS_CLIENT_STATS_UI_H

#include "GameStateUI.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"

namespace DanBias { namespace Client
{
	class StatsUI : public GameStateUI
	{
	public:
		StatsUI();
		virtual ~StatsUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI() const;
		void RenderText() const;
		bool Release();
		// TODO add function to add a new players statistics 
		// TODO add function to remove a players statistics 

	private:
		// TODO add multiple UI elements
		// one for each player ingame
		Text_UI* text;
		Plane_UI* plane;
	};
} }

#endif