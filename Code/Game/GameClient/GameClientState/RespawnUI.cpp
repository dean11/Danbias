#include "RespawnUI.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::Utility::Value;

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
}

RespawnUI::~RespawnUI() { /* Do nothing */ }

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
	return false; // TODO: change to true when we want UI elements like a chat window
}

void RespawnUI::RenderGUI() const
{
	// TODO: We need?
}

void RespawnUI::RenderText() const
{
	// TODO: Text countdown somewhere on screen would be nice
}

bool RespawnUI::Release()
{
	// TODO: Release UI components here.
	return true;
}




