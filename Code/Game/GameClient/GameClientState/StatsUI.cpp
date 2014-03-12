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
	this->names = nullptr;
	this->kills = nullptr;
	this->death = nullptr;
	this->render = false;
}

StatsUI::StatsUI( SharedStateContent* shared ) :
	GameStateUI( shared )
{
	this->names = nullptr;
	this->kills = nullptr;
	this->death = nullptr;
	this->render = false;
}

StatsUI::~StatsUI() { /* Do nothing */ }

bool StatsUI::Init( int maxNrOfPlayers )
{
	this->textDepth = 0.2f;
	this->nameMargin = 0.15f;
	this->killsMargin = 0.3f;
	this->deathMargin = 0.5f; 
	this->lineSpacing = 0.05f;
	this->textHeightPos = 0.3;
	this->textHeight = 0.1f;
	this->textWidth = 0.08f;
	// z value should be between 0.1 - 0.5 so that it will be in front of other states
	// add textures and text for player stats


	// black background
	this->backGround = new Plane_UI(L"color_white.png", Float3(0.5f, 0.5f, 0.5f), Float2(0.7f, 0.7f), Float4(0,0,0,0.5));
	this->nameText = new Text_UI(L"Name", Float3(0.15f, textHeightPos - lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->killText = new Text_UI(L"Kills", Float3(0.4f, textHeightPos - lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->deathText = new Text_UI(L"Deaths", Float3(0.6f, textHeightPos - lineSpacing, textDepth), Float2(textWidth, textHeight));

	this->playerId = new int[maxNrOfPlayers];
	this->names	=  new Text_UI*[maxNrOfPlayers];
	this->kills	=  new Text_UI*[maxNrOfPlayers];
	this->death	=  new Text_UI*[maxNrOfPlayers];
	this->nrOfPlayers = 0;
	this->maxNrOfPlayers = maxNrOfPlayers;

	//Parse layout variables
	ParseLayout();

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
	this->nameText->RenderText(); 
	this->killText->RenderText();
	this->deathText->RenderText();

	float offset = textHeightPos;
	for( int i = 0; i< nrOfPlayers; i++)
	{
		offset += 0.05;
		this->names[i]->RenderText(Float3(nameMargin, offset, textDepth)); 
		this->kills[i]->RenderText(Float3(killsMargin, offset, textDepth));
		this->death[i]->RenderText(Float3(deathMargin, offset, textDepth));
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
		if(this->playerId[i] == id)
		{
			// player already exist
			return;
		}
	}

	// new player
	this->playerId[nrOfPlayers] = id;
	this->names[nrOfPlayers] = new Text_UI(name, Float3(nameMargin, textHeightPos + nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->kills[nrOfPlayers] = new Text_UI(std::to_wstring(kills), Float3(killsMargin, textHeightPos + nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
	this->death[nrOfPlayers] = new Text_UI(std::to_wstring(deaths), Float3(deathMargin,  textHeightPos+ nrOfPlayers+lineSpacing, textDepth), Float2(textWidth, textHeight));
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

void StatsUI::ParseLayout()
{
	
	std::ifstream in("..\\Settings\\statsLayout.settings");
	if(in.is_open())
	{
		while (!in.eof())
		{
			std::string buff;
			in >> buff;

			if(buff == "header")
			{
				float fsize, yPos, xname, xkill, xdeath, zval;
				Float3 nsize, kdsize;
				in >> fsize;
				in >> yPos;
				in >> zval;
				in >> xname;
				in >> xkill;
				in >> xdeath;
				in >> nsize.x; in >> nsize.y; in >> nsize.z;
				in >> kdsize.x; in >> kdsize.y; in >> kdsize.z;
				this->textHeightPos = yPos;

				this->nameText->setFontSize(fsize);
				this->nameText->GetPos().z = zval;
				this->nameText->GetPos().y = yPos;
				this->nameText->GetPos().x = xname;

				this->killText->setFontSize(fsize);
				this->killText->GetPos().z = zval;
				this->killText->GetPos().y = yPos;
				this->killText->GetPos().x = xkill;

				this->deathText->setFontSize(fsize);
				this->deathText->GetPos().z = zval;
				this->deathText->GetPos().y = yPos;
				this->deathText->GetPos().x = xdeath;

				this->nameMargin = xname;
				this->killsMargin = xkill+0.004;
				this->deathMargin = xdeath;
				this->nameText->GetSize() = nsize;
				this->killText->GetSize() = kdsize;
				this->deathText->GetSize() = kdsize;

				for (int i = 0; i < this->nrOfPlayers; i++)
				{
					this->names[i]->setFontSize(fsize);
					this->kills[i]->setFontSize(fsize);
					this->death[i]->setFontSize(fsize);

					this->names[i]->GetSize() = nsize;
					this->kills[i]->GetSize() = kdsize;
					this->death[i]->GetSize() = kdsize;
				}
			}
		}
		in.close();
	}
}

void StatsUI::OnKeyPress (Input::Enum::SAKI key, Input::Keyboard* sender)
{
#if defined(DEBUG) || defined(_DEBUG)
	{
		if(key == Input::Enum::SAKI_P)	ParseLayout();
	}
#endif
}