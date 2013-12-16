
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
void C_StaticObj::Init(ModelInitData modelInit)
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->WorldMatrix = modelInit.world;
	privData->model->Visible = modelInit.visible;

}
void C_StaticObj::setPos(Oyster::Math::Float4x4 world)
{
	privData->model->WorldMatrix  = world;
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