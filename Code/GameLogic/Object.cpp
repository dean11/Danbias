#include "Object.h"
#include "OysterMath.h"
#include "DllInterfaces\GFXAPI.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Graphics::Render;

using namespace Utility::DynamicMemory;

Object::Object(void)
{
	model = new Oyster::Graphics::Model::Model();
	model = Oyster::Graphics::API::CreateModel(L"bth.obj");

	model->WorldMatrix *= 0.1f;
	model->WorldMatrix.m44 = 1.0f;
}


Object::~Object(void)
{

}
void Object::Render()
{
	Oyster::Graphics::API::RenderScene(model,1);
}

void Object::Update()
{
	//dummy implementation that will be overloaded if the other class implements it in a different way
}

Object::OBJECT_TYPE Object::GetType()
{
	return this->type;
}
