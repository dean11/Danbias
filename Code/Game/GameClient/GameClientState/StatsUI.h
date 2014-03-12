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
		StatsUI( SharedStateContent* shared );
		virtual ~StatsUI();
		bool Init( int maxNROfPlayers );

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void addPLayer( int id, std::wstring name, int kills, int deaths );
		bool removePlayer( int id );
		bool updateKillScore( int id, int kills);
		bool updateDeatchScore( int id, int deaths); 
		void ChangeState( UIState next );

		void ActivateInput();
		void DeactivateInput();
		void ParseLayout();
		void OnKeyPress (Input::Enum::SAKI key, Input::Keyboard* sender);

	private:
		// TODO add multiple UI elements
		// one for each player ingame
		Text_UI* nameText;
		Text_UI* killText;
		Text_UI* deathText;

		int * playerId; 
		Text_UI** names;
		Text_UI** kills;
		Text_UI** death;
		Plane_UI* backGround;
		int nrOfPlayers; 
		int maxNrOfPlayers; 
		float textDepth;
		float nameMargin;
		float killsMargin;
		float deathMargin; 
		float lineSpacing;
		float textHeightPos; 
		float textHeight;
		float textWidth;
		bool render;

		StatsUI();
	};
} }

#endif