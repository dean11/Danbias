#include "C_UIobject.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_UIobject::myData
{
	myData(){}
	Oyster::Graphics::Model::Model *model;
	int ID;
}privData;

C_UIobject::C_UIobject(void)
{
}


C_UIobject::~C_UIobject(void)
{
}
void C_UIobject::Init(ModelInitData modelInit)
{
	C_Object::Init(modelInit);
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->Visible = modelInit.visible;
	privData->model->WorldMatrix = getWorld();
	privData->ID = modelInit.id;

}
void C_UIobject::setPos(Oyster::Math::Float4x4 world)
{
	privData->model->WorldMatrix  = world;
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
int C_UIobject::GetId()
{
	return privData->ID;
}