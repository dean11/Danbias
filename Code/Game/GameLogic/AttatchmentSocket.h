#ifndef ATTATCHMENTSOCKET_H
#define ATTATCHMENTSOCKET_H
#include "IAttatchment.h"
namespace GameLogic
{

	class AttatchmentSocket
	{
	public:
		AttatchmentSocket(void);
		~AttatchmentSocket(void);

		IAttatchment* GetAttatchment();

	private:
		struct PrivateData;
		PrivateData *myData;
	};
}
#endif