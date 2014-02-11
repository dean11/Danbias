#include "C_Player.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_Player::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
	Oyster::Math3D::Float4 lookDir;
	int ID;
}privData;

C_Player::C_Player(void)
	:C_DynamicObj()
{
}


C_Player::~C_Player(void)
{
	 
}

void C_Player::Init(ModelInitData modelInit)
{
	C_Object::Init(modelInit);
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->Visible = modelInit.visible;
	privData->model->WorldMatrix = getWorld();
	privData->ID = modelInit.id;
	privData->lookDir = Oyster::Math3D::Float4 (0,0,1,0);
}


void C_Player::Render()
{
		Oyster::Graphics::API::RenderModel(privData->model);
}
void C_Player::Release()
{
	Oyster::Graphics::API::DeleteModel(privData->model);
	delete privData; 
}
int C_Player::GetId()
{
	return privData->ID;
}