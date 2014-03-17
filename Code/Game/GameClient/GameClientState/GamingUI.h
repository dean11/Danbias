#ifndef DANBIAS_CLIENT_GAMING_UI_H
#define DANBIAS_CLIENT_GAMING_UI_H

#include "GameStateUI.h"
#include "Input.h"
#include "Camera_FPSV2.h"
#include "Buttons\Text_UI.h"
#include "Buttons\Plane_UI.h"
#include "InputManager.h"
#include "SharedStateContent.h"
#include "GameUISettings.h"
#include <Protocols.h>
#include <list>
#include "C_obj\C_Beam.h"

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
		void Render();
		void RenderGUI();
		void RenderText();
		bool Release();
		void SetHPtext( std::wstring hp );
		void SetGameTime( float time );
		void SetEnergyText( std::wstring energy );
		void SetKillMessage( std::wstring killerMessage );
		void ChangeState( UIState next );
		void StopGamingUI();

		void ActivateInput();
		void DeactivateInput();

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
			Oyster::Math::Float4 tint;

			Utility::DynamicMemory::SmartPointer<Plane_UI> crosshair;
			C_Beam *beam;
		
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
				beam = 0;
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
				beam = 0;
			}
			void Frame(float dt)
			{
				delta += dt;
				if(beam)
				{
					this->beam->Update(dt);
				}
			}
			void Activate(GamingUI* ui)
			{
				ui->shared->weapon->SetRotationSpeed( primaryRotationSpeed );
				ui->shared->network->Send( GameLogic::Protocol_PlayerChangeWeapon( id ) );
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
							ui->shared->weapon->SetRotationSpeed(primaryRotationSpeed * 1.08);
							this->primaryShootTimer = 0;
							this->delta = 0;
						}
					break;
					case GameLogic::Protocol_PlayerShot::ShootValue_SecondaryPress:
					case GameLogic::Protocol_PlayerShot::ShootValue_SecondaryRelease:
						secondaryShootTimer += delta;
						if(secondaryShootTimer > secondaryWeaponCooldown)
						{
							ui->shared->weapon->SetRotationSpeed(secondaryRotationSpeed);
							this->secondaryShootTimer = 0;
							this->delta = 0;
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

				ui->shared->network->Send( GameLogic::Protocol_PlayerShot(s) );
			}
		};

	private:
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
		float gameTime;
		
		std::vector<WeaponData> weapons;
		int currentWeapon;
		Settings::GameUISettings::Settings settings;
		bool key_forward;
		bool key_backward;
		bool key_strafeRight;
		bool key_strafeLeft;
		bool key_zipDown;
		bool key_Drop;
		bool mouse_firstDown;
		bool mouse_secondDown;

		GamingUI();
		void ReadKeyInput(float deltaTime);
		void GUIRenderToggle(bool toggle) override;

	private:
		typedef void(GamingUI::*RenderFunc)();
		GamingUI::RenderFunc renderGuiFunc;
		GamingUI::RenderFunc renderTextFunc;

		void DummyRender();
		void InternalRenderGUI();
		void InternalRenderText();
	};
} }

#endif