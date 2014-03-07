#ifndef GAMELOGIC_NOEDGECONSTANTS
#define GAMELOGIC_NOEDGECONSTANTS

namespace NoEdgeConstants
{
	namespace Enum
	{
		namespace Player
		{

		}
		namespace Globals
		{

		}
	}

	namespace Text
	{

	}

	namespace Values
	{
		namespace Globals
		{
			const float LogicFrameTime					= 1.0f / 120.f;
			const float PhysicsFrameTime				= 1.0f / 120.f;
			const float NetworkFrameTime				= 1.0f / 70.f;
			const float Gravity							= 10.0f;
		}

		namespace Limits
		{
			const float FallDamageThreshold				= 80.0f;
		}

		namespace Player
		{
			const float ForwardVelocity					= 19.0f;
			const float BackwardVelocity				= 13.0f;
			const float StrafeVelocity					= 18.0f;
			const float JumpVelocity					= 23.0f;
			const float Mass							= 23.0f;
			const float DampeningFactor					= 0.2f;
			const float RestitutionCoeff				= 0.0f;
			const float FrictionStatic					= 0.0f;
			const float FrictionDynamic					= 0.0f;
		}

		namespace Weapons
		{
			const unsigned int MaxNumberOfSockets		= 2u;

			namespace MassDriveForceAttachment
			{
				const int SlotId						= 0;
				const float MaxEnergy					= 100.0f;
				const float RechargeRate				= 0.3f;

				const float Pullforce					= 1000.0f;
				const float Pushforce					= 3000.0f;
				const float Zipforce					= 2000.0f;

				const float PullCost					= 18.0f;
				const float PushCost					= 9.0f;
				const float ZipCost						= 40.0f;

				const float PrimaryCooldown				= 0.2f;
				const float SecondaryCooldown			= 0.0f;
				const float MiddleCooldown				= 0.0f;

				const float MaxDistanceFromPlayer		= 5.0f;
			}

			namespace MassDriveProjectileAttachment
			{
				const int SlotId						= 1;
				const float MaxEnergy					= 100.0f;
				const float RechargeRate				= 0.3f;
				const float PrimaryDamage				= 5.0f;
				const float PrimaryCost                 = 30;

				const float PrimaryCooldown				= 0.1f;
				const float SecondaryCooldown			= 0.0f;
				const float MiddleCooldown				= 0.0f;
			}
		}
	}
}

#endif // !GAMELOGIC_NOEDGECONSTANTS
