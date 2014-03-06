//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

/*
	De här klasserna är troligtvis extremt fulkodade och hårdkodade.

*/

#ifndef FIRST_PERSON_WEAPON_H
#define FIRST_PERSON_WEAPON_H

#include "DllInterfaces/GFXAPI.h"
#include "C_Object.h"
#include "WinTimer.h"

namespace DanBias
{
	namespace Client
	{
		class Coil : public C_Object
		{
		public:
			Coil();
			~Coil();

			bool Init();

			void Rotate(float angle);
			void Shoot();

			void Update();
			void Render();

			void SetInitialRotation(Oyster::Math::Quaternion rot);
			void SetOffsetPosition(Oyster::Math::Float3 offsetPos);
			void SetWorld(Oyster::Math::Matrix world);
			void SetRotate(float angle);

		private:
			Oyster::Math::Float3 maxGlow;
			Oyster::Math::Float3 minGlow;
			
			Oyster::Math::Float4 angular;
			Oyster::Math::Quaternion initialRotation;
			Oyster::Math::Float3 offsetPosition;

			bool increaseGlow;
			float percent;
			float totalPercent;

		};

		class FirstPersonWeapon : public C_Object
		{
		public:
			FirstPersonWeapon();
			~FirstPersonWeapon();

			bool Init();
			void Shoot();
			void Update(Oyster::Math::Matrix viewMatrix, Oyster::Math::Float3 look);
			void Render();
			void SetRotationSpeed(float speed);
			
		private:
			Oyster::Math::Float4 initialRotation;
			Oyster::Math::Matrix viewMatrix;
			Oyster::Math::Float3 look;
			Oyster::Math::Float4 angular;

			Coil* base;
			Coil* flaps;
			Coil* coil[6];
			int coilID;

			bool rotate;

			Utility::WinTimer timer;
			double elapsed;
			float maxRotation;
			float maxRot;
			float currentRotation;
			float smallStepRotation;
		};
	}
}

#endif