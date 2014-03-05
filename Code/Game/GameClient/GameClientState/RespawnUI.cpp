#include "RespawnUI.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;

RespawnUI::RespawnUI() :
	GameStateUI( nullptr )
{
	/* Should never be called! */
	this->countDown = 0.0f;
	this->text	= nullptr;
	this->render = false;
}

RespawnUI::RespawnUI(  SharedStateContent* shared  ) :
	GameStateUI( shared )
{
	this->countDown = 0.0f;
	this->text	= nullptr;
	this->render = false;
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
	return false;
}

bool RespawnUI::HaveTextRender() const
{
	return this->render;
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
	this->DeactivateInput();

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

void RespawnUI::ActivateInput()
{
	this->render = true;
	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void RespawnUI::DeactivateInput()
{
	this->render = false;
	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}