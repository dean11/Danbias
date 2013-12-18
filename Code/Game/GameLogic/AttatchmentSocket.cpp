#include "AttatchmentSocket.h"
#include "IAttatchment.h"
using namespace GameLogic;

struct AttatchmentSocket::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

	IAttatchment *attatchment;


}myData;

AttatchmentSocket::AttatchmentSocket(void)
{
}


AttatchmentSocket::~AttatchmentSocket(void)
{
	if (myData->attatchment)
	{
		delete myData->attatchment;
	}
}

IAttatchment* AttatchmentSocket::GetAttatchment()
{
	return myData->attatchment;
}

void AttatchmentSocket::SetAttatchment(IAttatchment *attatchment)
{
	if (myData->attatchment)
	{
		delete myData->attatchment;
	}

	myData->attatchment = attatchment;
}

void AttatchmentSocket::RemoveAttatchment()
{
	if (myData->attatchment)
	{
		delete myData->attatchment;
	}
}
