#include "DynamicObject.h"

using namespace GameLogic;

struct DynamicObject::PrivateData
{
	PrivateData()
	{

	}

	~PrivateData()
	{

	}

}myData;


DynamicObject::DynamicObject()
{
	myData = new PrivateData();
}


DynamicObject::~DynamicObject(void)
{
	delete myData;
}