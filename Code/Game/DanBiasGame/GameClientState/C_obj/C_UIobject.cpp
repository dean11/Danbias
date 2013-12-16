#include "C_UIobject.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_UIobject::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
}privData;

C_UIobject::C_UIobject(void)
{
}


C_UIobject::~C_UIobject(void)
{
}
void C_UIobject::Init(ModelInitData modelInit)
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->WorldMatrix = modelInit.world;
	privData->model->Visible = modelInit.visible;

}
void C_UIobject::setPos()
{

}

void C_UIobject::Render()
{
	Oyster::Graphics::API::RenderModel(*(privData->model));
}
void C_UIobject::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}