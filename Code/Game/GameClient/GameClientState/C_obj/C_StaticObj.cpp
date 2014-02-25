#include "C_StaticObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

C_StaticObj::C_StaticObj(void)
	:C_Object()
{

}
C_StaticObj::~C_StaticObj(void)
{
	
}
bool C_StaticObj::Init(ModelInitData modelInit)
{
	return C_Object::Init(modelInit);
}
