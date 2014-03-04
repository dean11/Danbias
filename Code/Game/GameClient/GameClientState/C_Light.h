#ifndef DANBIAS_CLIENT_CLIGHT_H
#define DANBIAS_CLIENT_CLIGHT_H
#include "DllInterfaces/GFXAPI.h"
namespace DanBias
{
	namespace Client
	{
		class C_Light
		{
		private:	
			Oyster::Graphics::Definitions::Pointlight pointLightDesc;
			int id;

		public:
			C_Light( Oyster::Graphics::Definitions::Pointlight pointLightDesc, int id );
			virtual ~C_Light();
			
			Oyster::Graphics::Definitions::Pointlight getLightDesc() const;
			void setLightDesc( Oyster::Graphics::Definitions::Pointlight pointLightDesc );

			Oyster::Math::Float3 getPos() const;
			void setPos( Oyster::Math::Float3 newPos);
			void Render();
			void Release();
			int GetId() const;
		};
	}
}

#endif
