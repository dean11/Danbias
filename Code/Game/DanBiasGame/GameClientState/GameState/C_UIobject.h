#pragma once
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
			void setPos();

			void Render();
			void Release();
		};};};