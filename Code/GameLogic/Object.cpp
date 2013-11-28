#include "Object.h"
#include "OysterMath.h"
#include "DllInterfaces\GFXAPI.h"
#include "CollisionManager.h"


using namespace GameLogic;

using namespace Oyster::Math;
using namespace Oyster::Graphics::Model;

using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;

Object::Object(void)
{

	model = new Model();
	model = Oyster::Graphics::API::CreateModel(L"bth.obj");

	ICustomBody* temp = rigidBody = API::Instance().CreateSimpleRigidBody().Release();

	rigidBody->SetCenter(Float3(50,0,0));
	rigidBody->SetMass_KeepMomentum(30);
	rigidBody->SetSize(Float3(2,2,2));
	rigidBody->SetSubscription(true);
	rigidBody->SetMomentOfInertiaTensor_KeepMomentum(Float4x4(MomentOfInertia::CreateCuboidMatrix(30, 2, 2, 2)));


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
