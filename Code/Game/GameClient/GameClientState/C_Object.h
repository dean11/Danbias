#ifndef DANBIAS_CLIENT_COBJECT_H
#define DANBIAS_CLIENT_COBJECT_H
#include "DllInterfaces/GFXAPI.h"
namespace DanBias
{
	namespace Client
	{
		// RB DEBUG 
		enum RB_Type
		{
			RB_Type_Cube,
			RB_Type_Sphere, 
			RB_Type_None,
		};
		struct  RBInitData
		{
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			RB_Type type;
		};
		// !RB DEBUG 

		struct  ModelInitData
		{
			int id;
			std::wstring modelPath;
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			Oyster::Math::Float3 tint;
			Oyster::Math::Float3 gtint;
			bool visible; 
		};

		class C_Object
		{
		protected:
			Oyster::Math::Float4x4 world; 
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			int light;

			// RB DEBUG
			Oyster::Math::Float4x4 RBworld; 
			Oyster::Math::Float3 RBposition; 
			Oyster::Math::Quaternion RBrotation; 
			Oyster::Math::Float3 RBscale; 
			RB_Type type;
			// !RB DEBUG 

			int id;
		
		protected:
			Oyster::Graphics::Model::Model *model;
		public:
			C_Object();
			virtual ~C_Object();
			virtual bool Init(ModelInitData modelInit, int Light=-1);
			void updateWorld();
			//void setWorld(Oyster::Math::Float4x4 world);

			int GetLight();
			void SetLight(int i);

			Oyster::Math::Float4x4 getWorld() const;
			void setPos(Oyster::Math::Float3 newPos);
			Oyster::Math::Float3  getPos() const;
			void addPos(Oyster::Math::Float3 deltaPos);
			void setRot(Oyster::Math::Quaternion newRot);
			Oyster::Math::Quaternion  getRotation() const;
			void setScale(Oyster::Math::Float3 newScale);
			void addScale(Oyster::Math::Float3 deltaScale);
			Oyster::Math::Float3  getScale() const;

			Oyster::Math::Float3 GetTint();
			Oyster::Math::Float3 GetGlowTint();

			void SetTint(Oyster::Math::Float3);
			void SetGlowTint(Oyster::Math::Float3);
			void SetVisible(bool visible);

			// RB DEBUG
			void updateRBWorld();
			bool InitRB(RBInitData modelInit);
			Oyster::Math::Float4x4 getRBWorld() const;
			void setRBPos(Oyster::Math::Float3 newPos);
			Oyster::Math::Float3  getRBPos() const;
			void setRBRot(Oyster::Math::Quaternion newRot);
			Oyster::Math::Quaternion  getRBRotation() const;
			void setRBScale(Oyster::Math::Float3 newScale);
			Oyster::Math::Float3  getRBScale() const;
			RB_Type getBRtype()const;
			// !RB DEBUG 

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
