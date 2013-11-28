#include "Object.h"
#include "OysterMath.h"
#include "DllInterfaces\GFXAPI.h"
#include "CollisionManager.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Graphics::Model;

using namespace Utility::DynamicMemory;

Object::Object(void)
{

	model = new Model();
	model = Oyster::Graphics::API::CreateModel(L"bth.obj");
	refManager.AddMapping(*rigidBody, *this);

}


Object::~Object(void)
{

	Oyster::Graphics::API::DeleteModel(model);

}
Model* Object::Render()
{
	//Oyster::Graphics::API::RenderScene(model,1);

	//model->info->Vertices.Apply(0);
	this->rigidBody->GetOrientation(model->WorldMatrix);
	return model;

}

Object::OBJECT_TYPE Object::GetType()
{
	return this->type;
}
