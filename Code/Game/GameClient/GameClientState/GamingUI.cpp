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
	this->sharedData	= nullptr;
	this->camera		= nullptr;
	this->plane			= nullptr;
	this->text			= nullptr;
	this->nextState = GameStateUI::UIState_same;
}

GamingUI::GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera ) :
	GameStateUI()
{
	this->sharedData = shared;
	this->camera = camera;
	this->nextState = GameStateUI::UIState_same;
}

GamingUI::~GamingUI() { /* Do nothing */ }
bool GamingUI::Init()
{
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->plane	=  new Plane_UI(L"box_tex.png", Float3(0.5f, 0.0f, 0.5f), Float2(0.3f, 0.1f));
	this->text	=  new Text_UI(L"hej", Float3(0.5f,0.0f,0.1f), Float2(0.1f,0.1f));

	this->sharedData = sharedData;

	// setting input mode to all raw
	this->sharedData->keyboardDevice->Activate();
	this->sharedData->mouseDevice->Activate();
	this->sharedData->mouseDevice->AddMouseEvent(this);

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

	this->sharedData = 0;

	return true;
}
void GamingUI::SetHPtext( std::wstring hp )
{
	this->text->setText(hp);
}
void GamingUI::ReadKeyInput()
{
	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_W) )
	{ // move forward
		this->sharedData->network->Send( Protocol_PlayerMovementForward() );
	}

	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_S) )
	{ // move backward
		this->sharedData->network->Send( Protocol_PlayerMovementBackward() );
	}

	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_A) )
	{ // strafe left
		this->sharedData->network->Send( Protocol_PlayerMovementLeft() );
	}

	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_D) )
	{ // strafe right
		this->sharedData->network->Send( Protocol_PlayerMovementRight() );
	}

	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_Space) )
	{ // jump
		if(!this->key_Jump)
		{
			this->sharedData->network->Send( Protocol_PlayerJump() );
			this->key_Jump = true;
		}
	}
	else 
		this->key_Jump = false;

	// shoot
	//if( this->sharedData->mouseDevice->IsBtnDown(::Input::Enum::SAMI_MouseLeftBtn) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = true;
	//		playerShot.secondaryPressed = false;
	//		playerShot.utilityPressed = false;
	//		this->sharedData->network->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else 
	//	this->key_Shoot = false;
	//
	//if( this->sharedData->mouseDevice->IsBtnDown(::Input::Enum::SAMI_MouseRightBtn) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = false;
	//		playerShot.secondaryPressed = true;
	//		playerShot.utilityPressed = false;
	//		this->sharedData->network->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else
	//	this->key_Shoot = false;
	//
	//if( this->sharedData->mouseDevice->IsBtnDown(::Input::Enum::SAMI_MouseMiddleBtn) )
	//{
	//	if( !this->key_Shoot )
	//	{
	//		Protocol_PlayerShot playerShot;
	//		playerShot.primaryPressed = false;
	//		playerShot.secondaryPressed = false;
	//		playerShot.utilityPressed = true;
	//		this->sharedData->network->Send( playerShot );
	//		this->key_Shoot = true;
	//	}
	//} 
	//else 
	//	this->key_Shoot = false;

	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_Escape) )
	{
		this->nextState = GameStateUI::UIState_shut_down;
	} 
	if( this->sharedData->keyboardDevice->IsKeyDown(::Input::Enum::SAKI_M) )
	{
		this->nextState = GameStateUI::UIState_main_menu;
	} 
}

void GamingUI::OnMousePress	( Input::Enum::SAMI key, Input::Mouse* sender )	
{ 
	switch ( key )
	{
		case ::Input::Enum::SAMI_MouseLeftBtn:	// shoot
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_PrimaryPress) );
		break;
		case ::Input::Enum::SAMI_MouseRightBtn:
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_SecondaryPress) );
		break;
		case ::Input::Enum::SAMI_MouseMiddleBtn:	
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_UtilityPress) );
		break;
	}
}
void GamingUI::OnMouseRelease ( Input::Enum::SAMI key, Input::Mouse* sender )
{ 
	switch ( key )
	{
		case ::Input::Enum::SAMI_MouseLeftBtn:	// shoot
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_PrimaryRelease) );
		break;
		case ::Input::Enum::SAMI_MouseRightBtn:
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_SecondaryRelease) );
		break;
		case ::Input::Enum::SAMI_MouseMiddleBtn:	
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_UtilityRelease) );
		break;
	}
}
void GamingUI::OnMouseMoveVelocity ( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender )
{ 
	//send delta mouse movement 
	{
		this->camera->PitchDown( (-coordinate.y) * this->sharedData->mouseSensitivity );
		//this->camera->YawLeft( (-coordinate.x) * this->sharedData->mouseSensitivity );
		//if( deltaPos.x != 0.0f ) //This made the camera reset to a specific rotation. Why?
		{
			this->sharedData->network->Send( Protocol_PlayerLeftTurn((coordinate.x) * this->sharedData->mouseSensitivity, this->camera->GetLook()) );
		}
	}
}


