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

		public:
			C_UIobject(void);
			virtual ~C_UIobject(void);
			bool Init(ModelInitData modelInit);
			void setPos(Oyster::Math::Float4x4 world);
};};};
#endif