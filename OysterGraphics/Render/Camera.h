#pragma once

#include "OysterMath.h"

namespace Oyster
{
	namespace Render
	{
		class Camera
		{
		public:
			Camera(void);
			~Camera(void);

			//position
			Math::Float3 GetPosition();
			void SetPosition(Math::Float3);

			//axis
			Math::Float3 GetRight();
			Math::Float3 GetUp();
			Math::Float3 GetLook();

			//frustrum
			float GetNearZ();
			float GetFarZ();
			float GetAspect();
			float GetFovY();
			float GetFovX();

			//set frustrum
			void SetLens(float fovY, float aspect, float zn, float zf);

			//normal LookAt
			void LookAt(Math::Float3 pos, Math::Float3 target, Math::Float3 worldUp);

			//Get Matrices
			Math::Matrix View();
			Math::Matrix Proj();
			Math::Matrix ViewProj();

			//Move Camera
			void Strafe(float d);
			void Walk(float d);
			void Fly(float d);

			//Rotate Camera
			void Pitch(float angle);
			void Yaw(float angle);
			void Roll(float angle);
			void RotateY(float angle);

			void UpdateViewMatrix();

		private:
			Math::Vector3 pos, right, up, look;
			float nearZ,farZ,aspect,fovX,fovY;
			Math::Matrix mView, mProj;
		};
	}
}
