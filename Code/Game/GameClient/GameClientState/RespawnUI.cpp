#include "RespawnUI.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;

RespawnUI::RespawnUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->netClient = nullptr;
	this->countDown = 0.0f;
}

RespawnUI::RespawnUI( NetworkClient *connection, float delay ) :
	GameStateUI()
{
	this->netClient = connection;
	this->countDown = delay;
	this->text	= nullptr;
}

RespawnUI::~RespawnUI() { /* Do nothing */ }

bool RespawnUI::Init()
{
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->text	=  new Text_UI(L"DEAD", Float3(0.35f,0.35f,0.5f), Float2(0.5f,0.2f), 0.1f);
	this->deathCounter	=  new Text_UI(L"", Float3(0.45f,0.55f,0.5f), Float2(0.2f,0.2f), 0.1f);
	return true; 
}
GameStateUI::UIState RespawnUI::Update( float deltaTime )
{
	this->countDown = Max( this->countDown - deltaTime, 0.0f );
	return this->nextState;
}

bool RespawnUI::HaveGUIRender() const
{
	return false; // TODO: change to true when we want UI elements like a crosshair
}

bool RespawnUI::HaveTextRender() const
{
	return true; // TODO: change to true when we want UI elements like a chat window
}

void RespawnUI::RenderGUI() 
{
	// TODO:BLOODY SCREEN
}

void RespawnUI::RenderText() 
{
	this->text->RenderText();
	this->deathCounter->RenderText(std::to_wstring((int)countDown));
}

bool RespawnUI::Release()
{
	if(this->text)
		delete this->text;
	if(this->deathCounter)
		delete this->deathCounter;
	return true;
}
void RespawnUI::SetCountdown( float cd )
{
	this->countDown = cd;
}

void RespawnUI::ChangeState( UIState next )
{
	this->nextState = next;
}

