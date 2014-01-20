#include "AttatchmentSocket.h"
#include "IAttatchment.h"
#include "DynamicArray.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;

struct AttatchmentSocket::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

	SmartPointer<IAttatchment> attatchment;


}myData;

AttatchmentSocket::AttatchmentSocket(void)
{
}


AttatchmentSocket::~AttatchmentSocket(void)
{
	
}

IAttatchment* AttatchmentSocket::GetAttatchment()
{
	return myData->attatchment;
}

void AttatchmentSocket::SetAttatchment(IAttatchment *attatchment)
{
	myData->attatchment = attatchment;
}

void AttatchmentSocket::RemoveAttatchment()
{
	
	myData->attatchment = 0;

}
