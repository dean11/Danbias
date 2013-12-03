#include "Object.h"
#include "OysterMath.h"
#include "DllInterfaces\GFXAPI.h"
#include "CollisionManager.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Graphics::Model;

using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;

Object::Object(std::wstring objFile)
{

	//model = new Model();
	model = Oyster::Graphics::API::CreateModel(objFile);

	API::SimpleBodyDescription sbDesc;
	//sbDesc.centerPosition = 

	//poi
	ICustomBody* temp = rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();

	GameLogic::RefManager::getInstance()->AddMapping(*rigidBody, *this);

}


Object::~Object(void)
{

	Oyster::Graphics::API::DeleteModel(model);

}

void Object::Render()
{
	this->rigidBody->GetOrientation(model->WorldMatrix);
	Oyster::Graphics::API::RenderScene(model, 1);
}

Object::OBJECT_TYPE Object::GetType()
{
	return this->type;
}
