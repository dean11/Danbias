#include "StatsUI.h"
#include <Protocols.h>
#include "Utilities.h"

using namespace ::DanBias::Client;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;

StatsUI::StatsUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->names = nullptr;
	this->kills = nullptr;
	this->death = nullptr;
}

StatsUI::~StatsUI() { /* Do nothing */ }
bool StatsUI::Init( int maxNrOfPlayers )
{
	this->textDepth = 0.2f;
	this->nameMargin = 0.25f;
	this->killsMargin = 0.45f;
	this->deathMargin = 0.6f; 
	this->lineSpacing = 0.05f;
	this->textHeightPos = 0.3;
	this->textHeight = 0.1f;
	this->textWidth = 0.4f;
	// z value should be between 0.1 - 0.5 so that it will be in front of other states
	// add textures and text for player stats


	// black background
	this->backGround = new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f), Float4(0,0,0,0.5));
	this->nameText = new Text_UI(L"name", Float3(nameMargin, textHeightPos-lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->killText = new Text_UI(L"kill", Float3(killsMargin, textHeightPos-lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->deathText = new Text_UI(L"death", Float3(deathMargin, textHeightPos-lineSpacing, textDepth), Float2(textWidth, textHeight));

	this->id = new int[maxNrOfPlayers];
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
	// Set true if UIstate have any plane to render
	return true; 
}

bool StatsUI::HaveTextRender() const
{
	// Set true if UIstate have any text to render
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
	this->nameText->RenderText(); 
	this->killText->RenderText();
	this->deathText->RenderText();

	float offset = textHeightPos;
	for( int i = 0; i< nrOfPlayers; i++)
	{
		offset += 0.05;
		this->names[i]->RenderText(Float3(nameMargin, offset, textDepth), Float2(textWidth, textHeight)); 
		this->kills[i]->RenderText(Float3(killsMargin, offset, textDepth), Float2(textWidth, textHeight));
		this->death[i]->RenderText(Float3(deathMargin, offset, textDepth), Float2(textWidth, textHeight));
	}
}

bool StatsUI::Release()
{
	// TODO: Release UI components here.
	if(this->id)
		delete this->id;
	if(this->nameText)
		delete this->nameText;
	if(this->killText)
		delete this->killText;
	if(this->deathText)
		delete this->deathText;
	if(this->backGround)
		delete this->backGround;
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

void StatsUI::addPLayer( int id, std::wstring name, int kills, int deaths )
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->id[i] == id)
		{
			// player already exist
			return;
		}
	}

	// new player
	this->id[nrOfPlayers] = id;
	this->names[nrOfPlayers] = new Text_UI(name, Float3(nameMargin, textHeightPos + nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->kills[nrOfPlayers] = new Text_UI(std::to_wstring(kills), Float3(killsMargin, textHeightPos + nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->death[nrOfPlayers] = new Text_UI(std::to_wstring(deaths), Float3(deathMargin,  textHeightPos+ nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
	nrOfPlayers ++; 
}
bool StatsUI::removePlayer( int id )
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->id[i] == id)
		{
			nrOfPlayers --; 
			this->id[i] = this->id[nrOfPlayers];

			delete this->names[this->id[i]];
			this->names[this->id[i]] = this->names[nrOfPlayers];
			this->names[nrOfPlayers] = nullptr;
			
			delete this->kills[this->id[i]];
			this->kills[this->id[i]] = this->kills[nrOfPlayers];
			this->kills[nrOfPlayers] = nullptr;

			delete this->death[this->id[i]];
			this->death[this->id[i]] = this->death[nrOfPlayers];
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
		if(this->id[i] == id)
		{
			this->kills[this->id[i]]->setText( std::to_wstring(kills) );
			return true;
		}
	}
	return false;
}
bool StatsUI::updateDeatchScore( int id, int deaths)
{
	for( int i = 0; i< nrOfPlayers; i++)
	{
		if(this->id[i] == id)
		{
			this->death[this->id[i]]->setText( std::to_wstring(deaths) );
			return true;
		}
	}
	return false;
}
void StatsUI::ChangeState( UIState next )
{
	this->nextState = next;
}
