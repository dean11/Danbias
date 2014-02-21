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
	this->plane	= nullptr;
	this->text	= nullptr;
}

StatsUI::~StatsUI() { /* Do nothing */ }
bool StatsUI::Init()
{
	// z value should be between 0.1 - 0.5 so that it will be in front of other states
	// add textures and text for player stats
	this->plane	=  new Plane_UI(L"box_tex.png", Float3(0.0f, 0.0f, 0.5f), Float2(0.3f, 0.1f));
	this->text	=  new Text_UI(L"Stats", Float3(0.0f,0.0f,0.1f), Float2(0.1f,0.1f));

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

void StatsUI::RenderGUI() const
{
	// render all the planes
	this->plane->RenderTexture();
}

void StatsUI::RenderText() const
{
	// render all the text
	this->text->RenderText();
}

bool StatsUI::Release()
{
	// TODO: Release UI components here.
	if(this->plane) 
		delete this->plane;
	if(this->text)
		delete this->text;
	return true;
}
