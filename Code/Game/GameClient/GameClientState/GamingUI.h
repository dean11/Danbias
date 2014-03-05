#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "InputManager.h"
#include "SharedStateContent.h"
#include <Protocols.h>

namespace DanBias { namespace Client
{
	class GamingUI : public GameStateUI
	{
	public:
		GamingUI( SharedStateContent* shared, Camera_FPSV2 *camera );
		virtual ~GamingUI();
		bool Init();

		UIState Update( float deltaTime );
		bool HaveGUIRender() const;
		bool HaveTextRender() const;
		void RenderGUI();
		void RenderText();
		bool Release();
		void SetHPtext( std::wstring hp );
		void SetEnergyText( std::wstring energy );
		void SetKillMessage( std::wstring killerMessage );
		void ChangeState( UIState next );
		void StopGamingUI();
	private: /* Overidden mouse methods */
		void OnMousePress			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseRelease			( Input::Enum::SAMI key, Input::Mouse* sender )							override;
		void OnMouseMoveVelocity	( Input::Struct::SAIPointFloat2D velocity, Input::Mouse* sender )		override;
		void OnMouseScroll			( int delta, Input::Mouse* sender )										override;

		void OnKeyPress				( Input::Enum::SAKI key, Input::Keyboard* sender)						override;
		void OnKeyRelease			( Input::Enum::SAKI key, Input::Keyboard* sender)						override;

	private:
		struct WeaponData
		{
			float delta;
			int id;

			float primaryRotationSpeed;
			float primaryWeaponCooldown;
			float primaryShootTimer;

			float secondaryRotationSpeed;
			float secondaryWeaponCooldown;
			float secondaryShootTimer;

			float middleRotationSpeed;
			float middleWeaponCooldown;
			float middleShootTimer;

			Utility::DynamicMemory::SmartPointer<Plane_UI> crosshair;
		
			WeaponData() 
			{ 
				delta						= 0.0f;
				id							= 0;
				primaryRotationSpeed		= 0.0f;
				primaryWeaponCooldown		= 0.0f;
				primaryShootTimer			= 0.0f;
				secondaryRotationSpeed		= 0.0f;
				secondaryWeaponCooldown		= 0.0f;
				secondaryShootTimer			= 0.0f;
				middleRotationSpeed			= 0.0f;
				middleWeaponCooldown		= 0.0f;
				middleShootTimer			= 0.0f;
			}
			WeaponData(int _id, float gc, float rs) 
			{ 
				delta						= 0.0f;
				id							= _id;
				primaryRotationSpeed		= rs;
				primaryWeaponCooldown		= gc;
				primaryShootTimer			= 0.0f;
				secondaryRotationSpeed		= rs;
				secondaryWeaponCooldown		= gc;
				secondaryShootTimer			= 0.0f;
				middleRotationSpeed			= rs;
				middleWeaponCooldown		= gc;
				middleShootTimer			= 0.0f;
			}
			void Frame(float dt)
			{
				delta += dt;
			}
			void Activate(GamingUI* ui)
			{
				ui->sharedData->weapon->SetRotationSpeed( primaryRotationSpeed );
				ui->sharedData->network->Send( GameLogic::Protocol_PlayerChangeWeapon( id ) );
			}
			void Shoot(GamingUI* ui, GameLogic::Protocol_PlayerShot::ShootValue s)
			{
				switch (s)
				{
					case GameLogic::Protocol_PlayerShot::ShootValue_PrimaryPress:
					case GameLogic::Protocol_PlayerShot::ShootValue_PrimaryRelease:
						primaryShootTimer += delta;
						if(primaryShootTimer > primaryWeaponCooldown)
						{
							ui->sharedData->weapon->SetRotationSpeed(primaryRotationSpeed);
							this->primaryShootTimer = 0;
							this->delta = 0;
							ui->sharedData->weapon->Shoot();
						}
					break;
					case GameLogic::Protocol_PlayerShot::ShootValue_SecondaryPress:
					case GameLogic::Protocol_PlayerShot::ShootValue_SecondaryRelease:
						secondaryShootTimer += delta;
						if(secondaryShootTimer > secondaryWeaponCooldown)
						{
							ui->sharedData->weapon->SetRotationSpeed(secondaryRotationSpeed);
							this->secondaryShootTimer = 0;
							this->delta = 0;
							ui->sharedData->weapon->Shoot();
						}
					break;
					case GameLogic::Protocol_PlayerShot::ShootValue_UtilityPress:
					case GameLogic::Protocol_PlayerShot::ShootValue_UtilityRelease:
						middleWeaponCooldown += delta;
						if(middleWeaponCooldown > middleWeaponCooldown)
						{
							this->middleWeaponCooldown = 0;
							this->delta = 0;
						}
					break;
				}	

				ui->sharedData->network->Send( GameLogic::Protocol_PlayerShot(s) );
			}
		};

	private:
		SharedStateContent *sharedData;
		Camera_FPSV2 *camera;

		// TODO add multiple UI elements
		Text_UI* hp;
		Text_UI* energy;
		Text_UI** killMessages;
		int maxMessageCount;
		const float msg_Cooldown;
		float msg_Timer;
		const float zip_Cooldown;
		float zip_Timer;
		
		std::vector<WeaponData> weapons;
		int currentWeapon;

		bool key_forward;
		bool key_backward;
		bool key_strafeRight;
		bool key_strafeLeft;
		bool key_zipDown;
		bool mouse_firstDown;
		bool mouse_secondDown;

		GamingUI();
		void ReadKeyInput(float deltaTime);
	};
} }

#endif