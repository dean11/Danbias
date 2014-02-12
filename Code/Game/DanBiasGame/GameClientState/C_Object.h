#ifndef DANBIAS_CLIENT_COBJECT_H
#define DANBIAS_CLIENT_COBJECT_H
#include "DllInterfaces/GFXAPI.h"
namespace DanBias
{
	namespace Client
	{
		struct  ModelInitData
		{
			int id;
			std::wstring modelPath;
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			bool visible; 
		};

		class C_Object
		{
		private:
			Oyster::Math::Float4x4 world; 
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			Oyster::Graphics::Model::Model *model;
			int id;
			void updateWorld();
		public:

			virtual void Init(ModelInitData modelInit);
	
			void setWorld(Oyster::Math::Float4x4 world);
			Oyster::Math::Float4x4 getWorld() const;
			void setPos(Oyster::Math::Float3 newPos);
			Oyster::Math::Float3  getPos() const;
			void addPos(Oyster::Math::Float3 deltaPos);
			void setRot(Oyster::Math::Quaternion newRot);
			Oyster::Math::Quaternion  getRotation() const;
			void addRot(Oyster::Math::Quaternion deltaRot);
			void setScale(Oyster::Math::Float3 newScale);
			void addScale(Oyster::Math::Float3 deltaScale);
			Oyster::Math::Float3  getScale() const;

			virtual void Render();
			virtual void Release();
			virtual int GetId() const;
		};
	}
}

namespace Utility { namespace DynamicMemory
{ // template specializationto allowuse of dynamicmemory tools
	template<>
	inline void SafeDeleteInstance( ::DanBias::Client::C_Object *dynamicInstance )
	{
		if( dynamicInstance )
		{
			dynamicInstance->Release();
			delete dynamicInstance;
		}
	}
} }

#endif
