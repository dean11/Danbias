#include "C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;
struct  C_DynamicObj::myData
{
	myData(){}
	Oyster::Graphics::Model::Model *model;
	// light
	// sound
	// effect
}privData;
C_DynamicObj::C_DynamicObj(void)
{
}


C_DynamicObj::~C_DynamicObj(void)
{
	 
}
void C_DynamicObj::Init()
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(L"crate");
}
void C_DynamicObj::setPos()
{

}

void C_DynamicObj::Render()
{
	Oyster::Graphics::API::RenderModel(*(privData->model));
}
void C_DynamicObj::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}