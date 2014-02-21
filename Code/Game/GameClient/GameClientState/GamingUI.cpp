#include "GamingUI.h"
#include <Protocols.h>
#include "Utilities.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Input;

GamingUI::GamingUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->mouseInput = nullptr;
	this->keyboardInput = nullptr;
	this->netClient = nullptr;
	this->camera = nullptr;
	this->plane	= nullptr;
	this->text	= nullptr;
}

GamingUI::GamingUI( Mouse *mouseInput, Keyboard *keyboardInput, NetworkClient *connection, Camera_FPSV2 *camera ) :
	GameStateUI()
{
	this->mouseInput = mouseInput;
	this->keyboardInput = keyboardInput;
	this->netClient = connection;
	this->camera = camera;
}

GamingUI::~GamingUI() { /* Do nothing */ }
bool GamingUI::Init()
{
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->plane	=  new Plane_UI(L"box_tex.png", Float3(0.5f, 0.0f, 0.5f), Float2(0.3f, 0.1f));
	this->text	=  new Text_UI(L"hej", Float3(0.5f,0.0f,0.1f), Float2(0.1f,0.1f));

	return true; 
}
GameStateUI::UIState GamingUI::Update( float deltaTime )
{
	ReadKeyInput();
	return this->nextState;
}

bool GamingUI::HaveGUIRender() const
{
	return true; 
}

bool GamingUI::HaveTextRender() const
{
	return true; 
}

void GamingUI::RenderGUI() const
{
	this->plane->RenderTexture();
}

void GamingUI::RenderText() const
{
	this->text->RenderText();
}

bool GamingUI::Release()
{
	// TODO: Release UI components here.
	if(this->plane) 
		delete this->plane;
	if(this->text)
		delete this->text;
	return true;
}
void GamingUI::SetHPtext( std::wstring hp )
{
	this->text->setText(hp);
}
void GamingUI::ReadKeyInput()
{
	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_W) )
	{ // move forward
		this->netClient->Send( Protocol_PlayerMovementForward() );
	}

	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_S) )
	{ // move backward
		this->netClient->Send( Protocol_PlayerMovementBackward() );
	}

	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_A) )
	{ // strafe left
		this->netClient->Send( Protocol_PlayerMovementLeft() );
	}

	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_D) )
	{ // strafe right
		this->netClient->Send( Protocol_PlayerMovementRight() );
	}

	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_Space) )
	{ // jump
		if(!this->key_Jump)
		{
			this->netClient->Send( Protocol_PlayerJump() );
			this->key_Jump = true;
		}
	}
	else 
		this->key_Jump = false;

	// shoot
	if( this->mouseInput->IsBtnDown(::Input::Enum::SAMI_MouseLeftBtn) )
	{
		if( !this->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = true;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = false;
			this->netClient->Send( playerShot );
			this->key_Shoot = true;
		}
	} 
	else 
		this->key_Shoot = false;
	
	if( this->mouseInput->IsBtnDown(::Input::Enum::SAMI_MouseRightBtn) )
	{
		if( !this->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = true;
			playerShot.utilityPressed = false;
			this->netClient->Send( playerShot );
			this->key_Shoot = true;
		}
	} 
	else 
		this->key_Shoot = false;
	
	if( this->mouseInput->IsBtnDown(::Input::Enum::SAMI_MouseMiddleBtn) )
	{
		if( !this->key_Shoot )
		{
			Protocol_PlayerShot playerShot;
			playerShot.primaryPressed = false;
			playerShot.secondaryPressed = false;
			playerShot.utilityPressed = true;
			this->netClient->Send( playerShot );
			this->key_Shoot = true;
		}
	} 
	else 
		this->key_Shoot = false;

	//send delta mouse movement 
	{
		static const float mouseSensitivity = Radian( 1.0f );
		::Input::Struct::SAIPoint2D deltaPos;
		this->mouseInput->GetDeltaPosition( deltaPos );

		this->camera->PitchDown( deltaPos.y * mouseSensitivity );;
		//if( deltaPos.x != 0.0f ) //This made the camera reset to a specific rotation. Why?
		{
			this->netClient->Send( Protocol_PlayerLeftTurn(deltaPos.x * mouseSensitivity) );
		}
	}

	if( this->keyboardInput->IsKeyDown(::Input::Enum::SAKI_Escape) )
	{
		this->nextState = GameStateUI::UIState_shut_down;
	} 
}

