#include "GamingUI.h"
#include <Protocols.h>

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;

GamingUI::GamingUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->input = nullptr;
	this->netClient = nullptr;
	this->camera = nullptr;
}

GamingUI::GamingUI( InputClass *input, NetworkClient *connection, Camera_FPSV2 *camera ) :
	GameStateUI()
{
	this->input = input;
	this->netClient = connection;
	this->camera = camera;
}

GamingUI::~GamingUI() { /* Do nothing */ }

GameStateUI::UIState GamingUI::Update( float deltaTime )
{
	return this->nextState;
}

bool GamingUI::HaveGUIRender() const
{
	return false; // TODO: change to true when we want UI elements like a crosshair
}

bool GamingUI::HaveTextRender() const
{
	return false; // TODO: change to true when we want UI elements like a chat window
}

void GamingUI::RenderGUI() const
{
	// TODO: Render crosshairs and such here. Don't forget to adjust GamingUI::HaveGUIRender
}

void GamingUI::RenderText() const
{
	// TODO: Render chattext and such here. Don't forget to adjust GamingUI::HaveGUIRender
}

bool GamingUI::Release()
{
	// TODO: Release UI components here.
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

//	if( this->input->IsKeyPressed(DIK_R) )
//	{
//		if( !this->key_Reload_Shaders )
//		{
//#ifdef _DEBUG
//			Graphics::API::ReloadShaders();
//#endif
//			this->key_Reload_Shaders = true;
//		}
//	} 
//	else 
//		this->key_Reload_Shaders = false;

	//send delta mouse movement 
	{
		this->camera->YawRight( this->input->GetYaw() * 0.017f );
		this->camera->PitchDown( this->input->GetPitch() * 0.017f );
		this->camera->UpdateOrientation();

		this->netClient->Send( Protocol_PlayerLook(this->camera->GetLook(), this->camera->GetRight()) );
	}

	// shoot
	//if( this->input->IsKeyPressed(DIK_Z) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = true;
	//		playerShot.secondaryPressed = false;
	//		playerShot.utilityPressed = false;
	//		this->netClient->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else 
	//	this->key_Shoot = false;

	//if( this->input->IsKeyPressed(DIK_X) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = false;
	//		playerShot.secondaryPressed = true;
	//		playerShot.utilityPressed = false;
	//		this->netClient->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else 
	//	this->key_Shoot = false;

	//if( this->input->IsKeyPressed(DIK_C) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = false;
	//		playerShot.secondaryPressed = false;
	//		playerShot.utilityPressed = true;
	//		this->netClient->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else 
	//	this->key_Shoot = false;

	// jump
	if( this->input->IsKeyPressed(DIK_SPACE) )
	{
		this->netClient->Send( Protocol_PlayerJump() );
	}
}