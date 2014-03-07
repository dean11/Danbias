#include "GamingUI.h"
#include <Protocols.h>
#include "Utilities.h"
#include "NoEdgeConstants.h"

using namespace ::DanBias::Client;
using namespace ::Oyster::Network;
using namespace ::GameLogic;
using namespace ::Utility::Value;
using namespace ::Oyster::Math;
using namespace ::Input;
using namespace ::Input::Enum;

GamingUI::GamingUI() 
	: GameStateUI( nullptr )
	, zip_Cooldown(0.4f)
	, msg_Cooldown(6.0f)
{
	/* Should never be called! */
	this->camera			= nullptr;
	this->energy			= nullptr;
	this->hp				= nullptr;
	this->key_backward		= false;
	this->key_forward		= false;
	this->key_strafeLeft	= false;
	this->key_strafeRight	= false;
	this->mouse_secondDown	= false;
	this->mouse_firstDown	= false;
	this->key_zipDown		= false;
	this->key_Drop			= false;
	this->currentWeapon		= 0;
	this->zip_Timer			= 0;
	this->nextState 		= GameStateUI::UIState_same;
}

GamingUI::GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera ) 
	: GameStateUI( shared )
	, zip_Cooldown(0.4f)
	, msg_Cooldown(6.0f)
{
	this->camera			= camera;
	this->hp				= nullptr;
	this->energy			= nullptr;
	this->key_backward		= false;
	this->key_forward		= false;
	this->key_strafeLeft	= false;
	this->key_strafeRight	= false;
	this->mouse_secondDown	= false;
	this->mouse_firstDown	= false;
	this->key_zipDown		= false;
	this->key_Drop			= false;
	this->currentWeapon		= 0;
	this->zip_Timer			= 0.0f;
	this->msg_Timer			= 0.0f;
	this->nextState			= GameStateUI::UIState_same;
}

GamingUI::~GamingUI() { }
bool GamingUI::Init()
{
	Float2 size = Oyster::Graphics::API::GetOption().resolution;
	// z value should be between 0.5 - 0.9 so that it will be behind other states
	// add textures and text
	this->hp 		= new Text_UI(L"100", Float3(0.04f,0.91f,0.1f), Float2(0.3f,0.1f), 0.05f, Float4(1,0,0,1));
	this->energy 	= new Text_UI(L"100", Float3(0.8f,0.91f,0.1f), Float2(0.3f,0.1f), 0.05f, Float4(1,1,0,1));
	this->maxMessageCount = 3;
	
	this->killMessages = new Text_UI*[maxMessageCount];
	this->killMessages[0] = new Text_UI(L"", Float3(0.02f,0.05f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));
	this->killMessages[1] = new Text_UI(L"", Float3(0.02f,0.1f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));
	this->killMessages[2] = new Text_UI(L"", Float3(0.02f,0.15f,0.1f), Float2(0.8f,0.1f), 0.035f, Float4(1,0.5,0,1));

	WeaponData w1 (	NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::SlotId
				  , NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PrimaryCooldown
				  , 3.5f );
	w1.crosshair = new Plane_UI(L"crosshair_low.png", Float3(0.5f, 0.5f, 0.1f), Float2(0.1f , 0.1f * (size.x / size.y)), Float4(1.0f, 1.0f, 1.0f, 1.0f));
	w1.tint = Float4(1.5f, 2.8f, 1.5f, 1.0f);
	this->weapons.push_back(w1);
	
	WeaponData w2 (	NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::SlotId
				  , NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCooldown
				  , 5.6f );
	w2.crosshair = new Plane_UI(L"crosshair.png", Float3(0.5f, 0.5f, 0.1f), Float2(0.011f , 0.011f * (size.x / size.y)), Float4(1.0f, 1.0f, 1.0f, 0.80f));;
	w2.tint = Float4(0.5f, 1.0f, 0.5f, 0.80f);
	this->weapons.push_back(w2);

	return true; 
}
GameStateUI::UIState GamingUI::Update( float deltaTime )
{
	ReadKeyInput(deltaTime);
	this->msg_Timer += deltaTime;
	this->weapons[this->currentWeapon].Frame(deltaTime);
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
	this->weapons[this->currentWeapon].crosshair->RenderTexture(this->weapons[this->currentWeapon].tint);
}

void GamingUI::RenderText() 
{
	this->hp->RenderText();
	this->energy->RenderText();
	if(this->msg_Timer < this->msg_Cooldown)
	{
		for (int i = 0; i < maxMessageCount; i++)
		{
			this->killMessages[i]->RenderText();
		}
	}
}

bool GamingUI::Release()
{
	this->DeactivateInput();

	if( this->hp )		delete this->hp;
	if( this->energy )	delete this->energy;

	for (int i = 0; i < maxMessageCount; i++)
	{
		if (this->killMessages[i])
		{ 
			delete this->killMessages[i];
		}
	}
	delete [] this->killMessages;
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
	this->msg_Timer = 0;
}
void GamingUI::ReadKeyInput(float deltaTime)
{
	if( this->key_forward )			this->shared->network->Send( Protocol_PlayerMovementForward() );
	if( this->key_backward )		this->shared->network->Send( Protocol_PlayerMovementBackward() );
	if( this->key_strafeLeft )		this->shared->network->Send( Protocol_PlayerMovementLeft() );
	if( this->key_strafeRight )		this->shared->network->Send( Protocol_PlayerMovementRight() );
	if( this->key_Drop )			this->shared->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_DropItem) );

	int energy = 0;
	energy = _wtoi(this->energy->getText().c_str());

	if( this->mouse_firstDown )		
	{
		if(this->currentWeapon == 0)
		{
			if(energy >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PushCost)
				this->weapons[this->currentWeapon].Shoot(this, Protocol_PlayerShot::ShootValue_PrimaryPress);
		}
		else
		{
			if(energy >= NoEdgeConstants::Values::Weapons::MassDriveProjectileAttachment::PrimaryCost)
				this->weapons[this->currentWeapon].Shoot(this, Protocol_PlayerShot::ShootValue_PrimaryPress);
		}
	}
	if( this->mouse_secondDown )
	{
		if(this->currentWeapon == 0)
		{
			if(energy >= NoEdgeConstants::Values::Weapons::MassDriveForceAttachment::PullCost)
				this->weapons[this->currentWeapon].Shoot(this, Protocol_PlayerShot::ShootValue_SecondaryPress);
		}
	}
	if( this->key_zipDown )
	{
		this->weapons[this->currentWeapon].Shoot(this, Protocol_PlayerShot::ShootValue_UtilityPress);
	}
}

void GamingUI::OnMousePress	( Input::Enum::SAMI key, Input::Mouse* sender )	
{ 
	switch ( key )
	{
		case ::Input::Enum::SAMI_MouseLeftBtn:	// shoot
			this->mouse_firstDown = true;
			//this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_PrimaryPress) );
			//this->sharedData->weapon->Shoot();
		break;
		case ::Input::Enum::SAMI_MouseRightBtn:
			this->mouse_secondDown = true;
			//this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_SecondaryPress) );
		break;
		case ::Input::Enum::SAMI_MouseMiddleBtn:	
			//this->shared->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_UtilityPress) );
		break;
	}
}
void GamingUI::OnMouseRelease ( Input::Enum::SAMI key, Input::Mouse* sender )
{ 
	switch ( key )
	{
		case ::Input::Enum::SAMI_MouseLeftBtn:	// shoot
			this->mouse_firstDown = false;
			//this->shared->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_PrimaryRelease) );
		break;
		case ::Input::Enum::SAMI_MouseRightBtn:
			this->mouse_secondDown = false;
			//this->sharedData->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_SecondaryRelease) );
		break;
		case ::Input::Enum::SAMI_MouseMiddleBtn:	
			//this->shared->network->Send( Protocol_PlayerShot(Protocol_PlayerShot::ShootValue_UtilityRelease) );
		break;
	}
}
void GamingUI::OnMouseMoveVelocity ( Input::Struct::SAIPointFloat2D velocity, Input::Mouse* sender )
{ 
	//send delta mouse movement 
	this->camera->PitchDown( (-velocity.y) * this->shared->mouseSensitivity );
	this->shared->network->Send( Protocol_PlayerLeftTurn((velocity.x) * this->shared->mouseSensitivity, this->camera->GetLook()) );
}
void GamingUI::OnMouseScroll( int delta, Input::Mouse* sender )
{
	if(delta == -1)
	{
		this->currentWeapon = min ( this->currentWeapon + 1, (int)this->weapons.size() - 1 );
		
		this->weapons[this->currentWeapon].Activate(this);

	}
	else if(delta == 1)
	{
		this->currentWeapon = max( this->currentWeapon - 1, 0);
		this->weapons[this->currentWeapon].Activate(this);
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
		case SAKI_E:		this->key_zipDown = true;
		break;
		case SAKI_F:		this->key_Drop = true;
		break;
		case SAKI_Space:	this->shared->network->Send( Protocol_PlayerJump() );
		break;
		// swap weapon to massDriver
		case SAKI_1:
			this->currentWeapon = 0;
			this->weapons[this->currentWeapon].Activate(this);
		break; 
		// swap weapon to shooting weapon
		case SAKI_2:	
			this->currentWeapon = min( (int)this->weapons.size() , 1 );
			this->weapons[this->currentWeapon].Activate(this);
		break;
	}
}
void GamingUI::OnKeyRelease(Enum::SAKI key, Keyboard* sender)
{
	if(sender != this->shared->keyboardDevice) return;
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
		case SAKI_E:		this->key_zipDown = false;
		break;
		case SAKI_F:		this->key_Drop = false;
		break;
		case SAKI_Escape:	this->nextState = UIState_ingame_meny;
		break;
	}

	//DEBUG: 
	
	if(key == SAKI_L)
		this->shared->mouseDevice->Deactivate();
	if(key == SAKI_M)
		this->shared->mouseDevice->Activate();
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
	this->key_zipDown		= false;
	this->key_Drop			= false;
	this->mouse_firstDown	= false;
	this->mouse_secondDown	= false;
}

void GamingUI::ActivateInput()
{
	this->ActivateGUIRender();
	this->shared->mouseDevice->AddMouseEvent( this );
	this->shared->keyboardDevice->AddKeyboardEvent( this );
}

void GamingUI::DeactivateInput()
{
	this->DeactivateGUIRender();
	this->shared->mouseDevice->RemoveMouseEvent( this );
	this->shared->keyboardDevice->RemoveKeyboardEvent( this );
}