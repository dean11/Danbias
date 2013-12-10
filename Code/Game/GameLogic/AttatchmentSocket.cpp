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

	IAttatchment *Attatchment;


}myData;

AttatchmentSocket::AttatchmentSocket(void)
{
}


AttatchmentSocket::~AttatchmentSocket(void)
{
}

IAttatchment* AttatchmentSocket::GetAttatchment()
{
	return myData->Attatchment;
}
