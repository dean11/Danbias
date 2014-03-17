#pragma once
#include "../C_Object.h"
#include "DllInterfaces\GFXAPI.h"

namespace DanBias { namespace Client
{
	class C_ClientLogic: public C_Object
	{
	public:
		class UpdateMethod;		
		C_ClientLogic();
		void Init(ModelInitData desc,UpdateMethod* updateMethod, Oyster::Graphics::Definitions::Pointlight origLight);
		void Init(ModelInitData desc, UpdateMethod* updateMethod, float speed, Oyster::Math::Float3 axis);
		virtual ~C_ClientLogic();

		/** @return true if expired */
		void Update( ::Oyster::Math::Float deltaTime );

		class UpdateMethod
		{
		public:
			UpdateMethod( C_ClientLogic* clientLogic);
			virtual ~UpdateMethod();
			virtual void Update( float dt ) = 0;
		protected:
			C_ClientLogic* clientLogic;
		};
		class UpdateRotation: public UpdateMethod
		{
		public:
			UpdateRotation(C_ClientLogic* clientLogic);
			~UpdateRotation();
			void Update( ::Oyster::Math::Float deltaTime );
		};
		class UpdateLight: public UpdateMethod
		{
		public:
			UpdateLight(C_ClientLogic* clientLogic);
			~UpdateLight();
			void Update( ::Oyster::Math::Float deltaTime );
		};

	private:
		Oyster::Graphics::Definitions::Pointlight currentLight;
		Oyster::Graphics::Definitions::Pointlight origLight;

		bool LightState;
		float interpolationSpeed;
		float interpolated;

		float rotationSpeed;
		Oyster::Math::Float3 roataionVector;
		UpdateMethod* updater; 
	};
} }
