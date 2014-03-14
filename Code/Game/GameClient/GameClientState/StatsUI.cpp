#include "StatsUI.h"
#include <Protocols.h>
#include "Utilities.h"
#include <fstream>

using namespace ::DanBias::Client;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;

StatsUI::StatsUI() :
	GameStateUI( nullptr )
{
	/* Should never be called! */
	this->colors = 0;
	this->names = nullptr;
	this->kills = nullptr;
	this->death = nullptr;
	this->render = false;
}

StatsUI::StatsUI( SharedStateContent* shared ) :
	GameStateUI( shared )
{
	this->colors = 0;
	this->names = nullptr;
	this->kills = nullptr;
	this->death = nullptr;
	this->render = false;
}

StatsUI::~StatsUI() { /* Do nothing */ }

bool StatsUI::Init( int maxNrOfPlayers )
{
	// z value should be between 0.1 - 0.5 so that it will be in front of other states
	// add textures and text for player stats

	//Parse layout variables
	Settings::StatsUISettings::ParseStatsSettings(this->settings);

	// black background
	this->backGround = new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f), Float4(0.6,0.6,0.6,0.5));
	this->nameText = new Text_UI(L"Name", Float3(this->settings.nameMarigin, this->settings.textOffsetY - this->settings.lineSpacing, this->settings.globalZ), Float2(this->settings.nameTextSize));
	this->killText = new Text_UI(L"Kills", Float3(this->settings.killsMargin, this->settings.textOffsetY - this->settings.lineSpacing, this->settings.globalZ), Float2(this->settings.killTextSize));
	this->deathText = new Text_UI(L"Deaths", Float3(this->settings.deathMargin, this->settings.textOffsetY - this->settings.lineSpacing, this->settings.globalZ), Float2(this->settings.deathTextSize));

	this->nameText->setFontSize(this->settings.fontSize);
	this->killText->setFontSize(this->settings.fontSize);
	this->deathText->setFontSize(this->settings.fontSize);

	this->colors = new Oyster::Math::Float4[maxNrOfPlayers];
	memset(&this->colors[0], 1, sizeof(Oyster::Math::Float4) * maxNrOfPlayers);
	this->playerId = new int[maxNrOfPlayers];
	this->names	=  new Text_UI*[maxNrOfPlayers];
	this->kills	=  new Text_UI*[maxNrOfPlayers];
	this->death	=  new Text_UI*[maxNrOfPlayers];
	this->nrOfPlayers = 0;
	this->maxNrOfPlayers = maxNrOfPlayers;

	return true; 
}
GameStateUI::UIState StatsUI::Update( float deltaTime )
{
	return this->nextState;
}

bool StatsUI::HaveGUIRender() const
{
	return true; 
}

bool StatsUI::HaveTextRender() const
{
	return true; 
}

void StatsUI::RenderGUI()
{
	// render all the planes
	this->backGround->RenderTexture();
}

void StatsUI::RenderText()
{
	// render all the text
	this->nameText->RenderText(Float3(this->settings.nameMarigin, this->settings.textOffsetY, this->settings.globalZ), this->settings.nameTextSize, this->settings.fontSize); 
	this->killText->RenderText(Float3(this->settings.killsMargin, this->settings.textOffsetY, this->settings.globalZ), this->settings.killTextSize, this->settings.fontSize); 
	this->deathText->RenderText(Float3(this->settings.deathMargin, this->settings.textOffsetY, this->settings.globalZ), this->settings.deathTextSize, this->settings.fontSize); 

	float offset = this->settings.textOffsetY;
	for( int i = 0; i< nrOfPlayers; i++)
	{
		offset += this->settings.lineSpacing;
		this->names[i]->RenderText(Float3(this->settings.nameMarigin, offset, this->settings.globalZ), this->settings.nameMarginTextSize, this->settings.fontSize, this->colors[i]); 
		this->kills[i]->RenderText(Float3(this->settings.killsMargin, offset, this->settings.globalZ), this->settings.killMarginTextSize, this->settings.fontSize, this->settings.killMarginColor);
		this->death[i]->RenderText(Float3(this->settings.deathMargin, offset, this->settings.globalZ), this->settings.deathMarginTextSize, this->settings.fontSize, this->settings.deathMarginColor);
	}
}

bool StatsUI::Release()
{
	this->DeactivateInput();

	if(this->playerId)
		delete this->playerId;
	if(this->nameText)
		delete this->nameText;
	if(this->killText)
		delete this->killText;
	if(this->deathText)
		delete this->deathText;
	if(this->backGround)
		delete this->backGround;
	if(this->colors)
		delete this->colors;
	for( int i = 0; i< nrOfPlayers; i++)
	{
		delete this->names[i]; 
		delete this->kills[i];
		delete this->death[i];
	}
	delete [] this->names; 
	delete [] this->kills;
	delete [] this->death;
	return true;
}

void StatsUI::addPLayer( int id, std::wstring name, int kills, int deaths, Oyster::Math::Float3 col )
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->playerId[i] == id)
		{
			updateDeatchScore(id, deaths);
			updateKillScore(id, kills);
			// player already exist
			return;
		}
	}

	// new player
	this->colors[this->nrOfPlayers] = col;
	this->colors[this->nrOfPlayers].w = 1.0f;
	this->playerId[nrOfPlayers] = id;
	this->names[nrOfPlayers] = new Text_UI(name);
	this->kills[nrOfPlayers] = new Text_UI(std::to_wstring(kills));
	this->death[nrOfPlayers] = new Text_UI(std::to_wstring(deaths));

	nrOfPlayers ++; 
}
bool StatsUI::removePlayer( int id )
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->playerId[i] == id)
		{
			nrOfPlayers --; 
			this->playerId[i] = this->playerId[nrOfPlayers];

			delete this->names[this->playerId[i]];
			this->names[i] = this->names[nrOfPlayers];
			this->names[nrOfPlayers] = nullptr;
			
			delete this->kills[this->playerId[i]];
			this->kills[i] = this->kills[nrOfPlayers];
			this->kills[nrOfPlayers] = nullptr;

			delete this->death[this->playerId[i]];
			this->death[i] = this->death[nrOfPlayers];
			this->death[nrOfPlayers] = nullptr;
			return true;
		}
	}
	return false;
}
bool StatsUI::updateKillScore( int id, int kills)
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->playerId[i] == id)
		{
			this->kills[i]->setText( std::to_wstring(kills) );
			return true;
		}
	}
	return false;
}
bool StatsUI::updateDeatchScore( int id, int deaths)
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->playerId[i] == id)
		{
			this->death[i]->setText( std::to_wstring(deaths) );
			return true;
		}
	}
	return false;
}
std::wstring StatsUI::getPlaterName( int id)
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->playerId[i] == id)
		{
			return this->names[i]->getText();
		}
	}
	return L""; 
}
void StatsUI::ChangeState( UIState next )
{
	this->nextState = next;
}

void StatsUI::ActivateInput()
{
	this->render = true;
	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void StatsUI::DeactivateInput()
{
	this->render = false;
	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}

void StatsUI::OnKeyPress (Input::Enum::SAKI key, Input::Keyboard* sender)
{
#if defined(DEBUG) || defined(_DEBUG)
	{
		if(key == Input::Enum::SAKI_P)	Settings::StatsUISettings::ParseStatsSettings(this->settings);
	}
#endif
}