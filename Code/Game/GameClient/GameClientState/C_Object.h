#ifndef DANBIAS_CLIENT_COBJECT_H
#define DANBIAS_CLIENT_COBJECT_H
#include "DllInterfaces/GFXAPI.h"
namespace DanBias
{
	namespace Client
	{
		enum RB_Type
		{
			RB_Type_Cube,
			RB_Type_Sphere
		};
		struct  ModelInitData
		{
			int id;
			std::wstring modelPath;
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			bool visible; 
		};
		struct  RBInitData
		{
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			RB_Type type;
		};

		class C_Object
		{
		private:
			Oyster::Math::Float4x4 world; 
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 

			// RB DEBUG
			Oyster::Math::Float3 RBposition; 
			Oyster::Math::Quaternion RBrotation; 
			Oyster::Math::Float3 RBscale; 
			RB_Type type;
	
			int id;
		
		protected:
			Oyster::Graphics::Model::Model *model;
		public:
			C_Object();
			virtual ~C_Object();
			virtual bool Init(ModelInitData modelInit);
			void updateWorld();
			//void setWorld(Oyster::Math::Float4x4 world);
			Oyster::Math::Float4x4 getWorld() const;
			void setPos(Oyster::Math::Float3 newPos);
			Oyster::Math::Float3  getPos() const;
			void addPos(Oyster::Math::Float3 deltaPos);
			void setRot(Oyster::Math::Quaternion newRot);
			Oyster::Math::Quaternion  getRotation() const;
			void setScale(Oyster::Math::Float3 newScale);
			void addScale(Oyster::Math::Float3 deltaScale);
			Oyster::Math::Float3  getScale() const;

			// RB DEBUG
			bool InitRB(RBInitData modelInit);
			Oyster::Math::Float4x4 getRBWorld() const;
			void setRBPos(Oyster::Math::Float3 newPos);
			Oyster::Math::Float3  getRBPos() const;
			void setRBRot(Oyster::Math::Quaternion newRot);
			Oyster::Math::Quaternion  getRBRotation() const;
			void setRBScale(Oyster::Math::Float3 newScale);
			Oyster::Math::Float3  getRBScale() const;
			RB_Type getBRtype()const;
			virtual void Render();
			virtual void Release();
			virtual int GetId() const;
		};
	}
}

#include "Utilities.h"

namespace Utility { namespace DynamicMemory
{
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
