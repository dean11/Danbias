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
	Oyster::Graphics::API::StartGuiRender();
	this->plane->RenderTexture();
}

void GamingUI::RenderText() const
{
	Oyster::Graphics::API::StartTextRender();
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

void GamingUI::ReadKeyInput()
{
	if( this->input->IsKeyPressed(DIK_W) )
	{
		this->netClient->Send( Protocol_PlayerMovementForward() );
	}

	if( this->input->IsKeyPressed(DIK_S) )
	{
		this->netClient->Send( Protocol_PlayerMovementBackward() );
	}

	if( this->input->IsKeyPressed(DIK_A) )
	{
		this->netClient->Send( Protocol_PlayerMovementLeft() );
	}

	if( this->input->IsKeyPressed(DIK_D) )
	{
		this->netClient->Send( Protocol_PlayerMovementRight() );
	}

//send delta mouse movement 
	{
		static const float mouseSensitivity = Radian( 1.0f );
		this->camera->PitchDown( this->input->GetPitch() * mouseSensitivity );
		float yaw = this->input->GetYaw();
		//if( yaw != 0.0f )	//This made the camera reset to a specific rotation.
		{
			this->netClient->Send( Protocol_PlayerLeftTurn(yaw * mouseSensitivity) );
		}
	}

	// shoot
	if( this->input->IsKeyPressed(DIK_Z) )
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
	if( this->input->IsKeyPressed(DIK_X) )
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
	if( this->input->IsKeyPressed(DIK_C) )
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

	// jump
	if( this->input->IsKeyPressed(DIK_SPACE) )
	{
		if(!this->key_Jump)
		{
			this->netClient->Send( Protocol_PlayerJump() );
			this->key_Jump = true;
		}
	}
	else 
		this->key_Jump = false;

	if( this->input->IsKeyPressed(DIK_ESCAPE) )
	{
		this->nextState = GameStateUI::UIState_shut_down;
	} 
	// !DEGUG KEYS
	// TODO: implement sub-menu
}

