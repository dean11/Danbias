#include "AttatchmentSocket.h"
#include "IAttatchment.h"
#include "DynamicArray.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;


AttatchmentSocket::AttatchmentSocket(void)
{
	this->attatchment = 0;
}


AttatchmentSocket::~AttatchmentSocket(void)
{
	
}

IAttatchment* AttatchmentSocket::GetAttatchment()
{
	return this->attatchment;
}

void AttatchmentSocket::SetAttatchment(IAttatchment *attatchment)
{
	this->attatchment = attatchment;
}

void AttatchmentSocket::RemoveAttatchment()
{
	
	this->attatchment = 0;

}
