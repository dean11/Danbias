
#include "C_StaticObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_StaticObj::myData
{
	myData(){}
	Oyster::Graphics::Model::Model *model;
	int ID;

}privData;
C_StaticObj::C_StaticObj(void)
{
}


C_StaticObj::~C_StaticObj(void)
{
	
}
void C_StaticObj::Init(ModelInitData modelInit)
{
	C_Object::Init(modelInit);
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->Visible = modelInit.visible;
	privData->model->WorldMatrix = getWorld();
	privData->ID = modelInit.id;

}


void C_StaticObj::Render()
{
	Oyster::Graphics::API::RenderModel(privData->model);
}
void C_StaticObj::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}
int C_StaticObj::GetId()
{
	return privData->ID;
}