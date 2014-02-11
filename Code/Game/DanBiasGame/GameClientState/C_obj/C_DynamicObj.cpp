#include "C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;
struct  C_DynamicObj::myData
{
	myData(){}
	Oyster::Graphics::Model::Model *model;
	int ID;
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
void C_DynamicObj::Init(ModelInitData modelInit)
{
	C_Object::Init(modelInit);
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->Visible = modelInit.visible;
	privData->model->WorldMatrix = getWorld();
	privData->ID = modelInit.id;
}

void C_DynamicObj::Render()
{
	Oyster::Graphics::API::RenderModel(privData->model);
}
void C_DynamicObj::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}
int C_DynamicObj::GetId()
{
	return privData->ID;
}