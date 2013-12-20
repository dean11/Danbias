#include "C_Player.h"
#include "DllInterfaces/GFXAPI.h"
using namespace DanBias::Client;

struct  C_Player::myData
{
	myData(){}
	Oyster::Math3D::Float4x4 view;
	Oyster::Math3D::Float4x4 proj; 
	Oyster::Graphics::Model::Model *model;
	int ID;
}privData;

C_Player::C_Player(void)
{
}


C_Player::~C_Player(void)
{
	 
}

void C_Player::Init(ModelInitData modelInit)
{
	// load models
	privData = new myData();
	privData->model = Oyster::Graphics::API::CreateModel(modelInit.modelPath);
	privData->model->WorldMatrix = modelInit.world;
	privData->model->Visible = modelInit.visible;


}
void C_Player::setPos(Oyster::Math::Float4x4 world)
{
	privData->model->WorldMatrix  = world;
}

void C_Player::Render()
{
		Oyster::Graphics::API::RenderModel(*(privData->model));
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