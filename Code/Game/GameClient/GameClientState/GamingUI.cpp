#include "GamingUI.h"
#include <Protocols.h>
#include "Utilities.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Input;
using namespace ::Input::Enum;

GamingUI::GamingUI() :
	GameStateUI()
{
	/* Should never be called! */
	this->sharedData		= nullptr;
	this->camera			= nullptr;
	this->crosshair			= nullptr;
	this->energy			= nullptr;
	this->hp				= nullptr;
	this->key_backward		= false;
	this->key_forward		= false;
	this->key_strafeLeft	= false;
	this->key_strafeRight	= false;
	this->nextState 		= GameStateUI::UIState_same;
}

GamingUI::GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera ) :
	GameStateUI()
{
	this->sharedData		= shared;
	this->camera			= camera;
	this->crosshair			= nullptr;
	this->hp				= nullptr;
	this->energy			= nullptr;
	this->key_backward		= false;
	this->key_forward		= false;
	this->key_strafeLeft	= false;
	this->key_strafeRight	= false;
	this->nextState			= GameStateUI::UIState_same;
}

GamingUI::~GamingUI() { }
bool GamingUI::Init()
{
	Float2 size = Oyster::Graphics::API::GetOption().resolution;
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->hp 		= new Text_UI(L"100", Float3(0.04f,0.91f,0.1f), Float2(0.1f,0.1f), 0.05f, Float4(1,0,0,1));
	this->energy 	= new Text_UI(L"100", Float3(0.8f,0.91f,0.1f), Float2(0.1f,0.1f), 0.05f, Float4(1,1,0,1));
	this->maxMessageCount = 3;
	
	this->message_Timer = 0;
	this->killMessages = new Text_UI*[maxMessageCount];
	this->killMessages[0] = new Text_UI(L"", Float3(0.02f,0.05f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));
	this->killMessages[1] = new Text_UI(L"", Float3(0.02f,0.1f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));
	this->killMessages[2] = new Text_UI(L"", Float3(0.02f,0.15f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));

	this->crosshair	= new Plane_UI(L"croshair.png", Float3(0.5f, 0.5f, 0.1f), Float2(0.0061f , 0.0061f * (size.x / size.y)), Float4(1.0f, 1.0f, 1.0f, 0.74f));

	this->sharedData = sharedData;
	// setting input mode to all raw
	this->sharedData->keyboardDevice->Activate();
	this->sharedData->keyboardDevice->AddKeyboardEvent(this);
	this->sharedData->mouseDevice->Activate();
	this->sharedData->mouseDevice->AddMouseEvent(this);

	return true; 
}
GameStateUI::UIState GamingUI::Update( float deltaTime )
{
	ReadKeyInput();
	this->message_Timer = Max( this->message_Timer - deltaTime, 0.0f );
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

void GamingUI::RenderGUI() 
{
	this->crosshair->RenderTexture();
}

void GamingUI::RenderText() 
{
	this->hp->RenderText();
	this->energy->RenderText();
	if(this->message_Timer > 0)
	{
		for (int i = 0; i < maxMessageCount; i++)
		{
			this->killMessages[i]->RenderText();
		}
	}
}

bool GamingUI::Release()
{
	//Release as input event
	this->sharedData->keyboardDevice->RemoveKeyboardEvent(this);
	this->sharedData->mouseDevice->RemoveMouseEvent(this);

	// TODO: Release UI components here.
	if(this->crosshair) 	delete this->crosshair;
	if(this->hp)		delete this->hp;
	if(this->energy)	delete this->energy;
	for (int i = 0; i < maxMessageCount; i++)
	{
		if (this->killMessages[i])
		{ 
			delete this->killMessages[i];
		}
	}
	delete [] this->killMessages;
	this->sharedData = 0;
	return true;
}
void GamingUI::SetHPtext( std::wstring hp )
{
	this->hp->setText(hp);
}
void GamingUI::SetEnergyText( std::wstring energy )
{
	this->energy->setText(energy);
}
void GamingUI::SetKillMessage( std::wstring killerMessage )
{
	this->killMessages[2]->setText( this->killMessages[1]->getText());
	this->killMessages[1]->setText( this->killMessages[0]->getText());
	this->killMessages[0]->setText( killerMessage);
	this->message_Timer = 2;
}
void GamingUI::ReadKeyInput()
{
	if( this->key_forward )			this->sharedData->network->Send( Protocol_PlayerMovementForward() );
	if( this->key_backward )		this->sharedData->network->Send( Protocol_PlayerMovementBackward() );
	if( this->key_strafeLeft )		this->sharedData->network->Send( Protocol_PlayerMovementLeft() );
	if( this->key_strafeRight )		this->sharedData->network->Send( Protocol_PlayerMovementRight() );
}

void GamingUI::OnMousePress	( Input::Enum::SAMI key, Input::Mouse* sender )	
{ 
	switch ( key )
	{
		case ::Input::Enum::SAMI_MouseLeftBtn:	// shoot
			this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_PrimaryPress) );
			this->sharedData->weapon->Shoot();
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
	this->camera->PitchDown( (-coordinate.y) * this->sharedData->mouseSensitivity );
	//this->camera->YawLeft( (-coordinate.x) * this->sharedData->mouseSensitivity );
	//if( deltaPos.x != 0.0f ) //This made the camera reset to a specific rotation. Why?
	{
		this->sharedData->network->Send( Protocol_PlayerLeftTurn((coordinate.x) * this->sharedData->mouseSensitivity, this->camera->GetLook()) );
	}
}

void GamingUI::OnKeyPress(Enum::SAKI key, Keyboard* sender)
{
	switch (key)
	{
		case SAKI_W:		this->key_forward = true;
		break;
		case SAKI_S:		this->key_backward = true;
		break;
		case SAKI_A:		this->key_strafeLeft = true;
		break;
		case SAKI_D:		this->key_strafeRight = true;
		break;
		case SAKI_Space:	this->sharedData->network->Send( Protocol_PlayerJump() );
		break;
		// swap weapon to massDriver
		case SAKI_1:		this->sharedData->network->Send( Protocol_PlayerChangeWeapon( 0 ) );
			break; 
		// swap weapon to shooting weapon
		case SAKI_2:		this->sharedData->network->Send( Protocol_PlayerChangeWeapon( 1 ) );
			break;
		case SAKI_Escape:	this->nextState = GameStateUI::UIState_inGameMeny;
		break;
	}
}
void GamingUI::OnKeyRelease(Enum::SAKI key, Keyboard* sender)
{
	if(sender != this->sharedData->keyboardDevice) return;
	switch (key)
	{
		case SAKI_W:		this->key_forward = false;
		break;
		case SAKI_S:		this->key_backward = false;
		break;
		case SAKI_A:		this->key_strafeLeft = false;
		break;
		case SAKI_D:		this->key_strafeRight = false;
		break;
	}

	//DEBUG: 
	
	if(key == SAKI_L)
		this->sharedData->mouseDevice->Deactivate();
	if(key == SAKI_M)
		this->sharedData->mouseDevice->Activate();
}
void GamingUI::ChangeState( UIState next )
{
	this->nextState = next;
}
void GamingUI::StopGamingUI()
{
	this->key_backward		= false;
	this->key_forward		= false;
	this->key_strafeLeft	= false;
	this->key_strafeRight	= false;
}