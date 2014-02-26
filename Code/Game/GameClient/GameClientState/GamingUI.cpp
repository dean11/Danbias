#include "GamingUI.h"
#include <Protocols.h>
#include "Utilities.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;

GamingUI::GamingUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->input = nullptr;
	this->netClient = nullptr;
	this->camera = nullptr;
	this->plane	= nullptr;
	this->text	= nullptr;
	this->nextState = GameStateUI::UIState_same;
}

GamingUI::GamingUI( InputClass *input, NetworkClient *connection, Camera_FPSV2 *camera ) :
	GameStateUI()
{
	this->input = input;
	this->netClient = connection;
	this->camera = camera;
	this->nextState = GameStateUI::UIState_same;
}

GamingUI::~GamingUI() { /* Do nothing */ }
bool GamingUI::Init()
{
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->plane	=  new Plane_UI(L"box_tex.png", Float3(0.5f, 0.0f, 0.5f), Float2(0.3f, 0.1f));
	this->text	=  new Text_UI(L"hej", Float3(0.3f,0.0f,0.1f), Float2(0.1f,0.1f));

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
			this->netClient->Send( Protocol_PlayerLeftTurn(yaw * mouseSensitivity, camera->GetLook()) );
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
	if( this->input->IsKeyPressed(DIK_M) )
	{
		this->nextState = GameStateUI::UIState_main_menu;
	} 
	// !DEGUG KEYS
	// TODO: implement sub-menu
}

