#include "C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

C_DynamicObj::C_DynamicObj(void)
		:C_Object()
{
}


C_DynamicObj::~C_DynamicObj(void)
{
	 
}
bool C_DynamicObj::Init(ModelInitData modelInit)
{
	return C_Object::Init(modelInit);
}
