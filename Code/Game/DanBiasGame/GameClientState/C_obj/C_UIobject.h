#ifndef DANBIAS_CLIENT_CUIOBJECT_H
#define DANBIAS_CLIENT_CUIOBJECT_H
#include "../C_Object.h"
namespace DanBias
{
	namespace Client
	{
		class C_UIobject : public C_Object 
		{
		private:
			struct myData;
			myData* privData;
		public:
			C_UIobject(void);
			virtual ~C_UIobject(void);
			void Init(ModelInitData modelInit);
			void setPos(Oyster::Math::Float4x4 world);

			void Render();
			void Release();
			int GetId();
};};};
#endif