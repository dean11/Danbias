#ifndef DANBIAS_CLIENT_CPLAYER_H
#define DANBIAS_CLIENT_CPLAYER_H
#include "C_DynamicObj.h"
namespace DanBias
{
	namespace Client
	{
		class C_Player : public C_DynamicObj
		{
		private:
		public:
			C_Player(void);
			virtual ~C_Player(void);
			void Init(ModelInitData modelInit);

		};
	}
}
#endif
