
#include "C_StaticObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_StaticObj::myData
{
	myData(){}
	Oyster::Graphics::Model::Model *model;
	// light
	// sound
	// effect
}privData;
C_StaticObj::C_StaticObj(void)
{
}


C_StaticObj::~C_StaticObj(void)
{
	
}
void C_StaticObj::Init()
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(L"worldDummy");
}
void C_StaticObj::setPos()
{

}

void C_StaticObj::Render()
{
	Oyster::Graphics::API::RenderModel(*(privData->model));
}
void C_StaticObj::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}