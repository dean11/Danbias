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
void C_DynamicObj::Init(ModelInitData modelInit)
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->WorldMatrix = modelInit.world;
	privData->model->Visible = modelInit.visible;
}
void C_DynamicObj::setPos(Oyster::Math::Float4x4 world)
{
	privData->model->WorldMatrix  = world;
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