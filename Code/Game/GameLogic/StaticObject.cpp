#include "StaticObject.h"

using namespace GameLogic;

struct StaticObject::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

}myData;


StaticObject::StaticObject()
{
	myData = new PrivateData();
}


StaticObject::~StaticObject(void)
{
	delete myData;
}
