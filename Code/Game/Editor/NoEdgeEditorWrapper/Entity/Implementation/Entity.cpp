#include "..\Header\Entity.h"

using namespace Oyster;
using namespace Oyster::Graphics;
using namespace Oyster::Math;
using namespace Oyster::Math3D;
static int globCount = 0;

bool Entity::HasMesh() const									   
{
	return (this->model != 0);
}
bool Entity::HasRigidBody() const								   
{
	return (this->body != 0);
}
bool Entity::IsVisible() const									   
{
	return this->model->Visible;
}
bool Entity::IsLocked() const									   
{
	return this->locked;
}
Entity::EntityType Entity::GetEntityType() const						   
{
	return this->entityType;
}
Entity::EntitySubType Entity::GetEntitySubType() const
{
	return this->entitySubType;
}
Float3 Entity::GetPosition() const
{
	return this->position;
}
Quaternion Entity::GetRotation() const
{
	return this->rotation;
}
Float3 Entity::GetScale() const
{
	return this->scale;
}
Oyster::Math::Matrix Entity::GetWorld() const
{
	Float4x4 translation = TranslationMatrix(this->position); 
	Float4x4 rot = RotationMatrix(this->rotation);
	Float4x4 scale = ScalingMatrix(this->scale);
	Float4x4 world = translation * rot * scale;

	return world;
}
int Entity::GetID() const										   
{
	return this->ID;
}
void Entity::SetPosition(Oyster::Math::Float3 position)			   
{
	this->position = position;
}
void Entity::SetRotation(Oyster::Math::Quaternion roation)		   
{
	this->rotation = rotation;
}
void Entity::SetScale(Oyster::Math::Float3 scale)				   
{
	this->scale = scale;
}
void Entity::Show( )
{
	if(this->model) this->model->Visible = true;
}
void Entity::Hide( )
{
	if(this->model) this->model->Visible = true;
}
void Entity::Lock( )
{
	this->locked = true;
}
void Entity::Unlock( )
{
	this->locked = false;
}
void Entity::Update()
{
	if(this->model)	
		this->model->WorldMatrix = GetWorld();
}
void Entity::Render()
{
	if(this->model)		
		Oyster::Graphics::API::RenderModel(this->model);
}
bool Entity::TryPickup(Oyster::Collision3D::Ray ray)
{
	return Physics::API::Instance().Intersect(ray.origin.xyz, ray.direction.xyz) == this->body;
}
void Entity::Release()
{
	Graphics::API::DeleteModel(this->model);
	this->model = 0;
}
Entity::~Entity()
{
	this->Release();
}
Entity::Entity(EntityType t, EntitySubType sub)
	:	entityType(t)
	,	ID(globCount++)
	,	entitySubType(sub)
	,	model(0)
	,	body(0)
{

}
bool Entity::EntityInitialize(const EntityInitDesc& desc)
{
	bool retVal = true;
	this->position = desc.general.position;
	this->rotation = desc.general.rotation;
	this->scale = desc.general.scale;

	if(desc.mesh.modelPath.size() > 0)
	{
		this->model = Oyster::Graphics::API::CreateModel(desc.mesh.modelPath);
		if(!this->model)	retVal = false;
		this->model->WorldMatrix = this->GetWorld();
	}

	//this->body = Physics::API::Instance().AddTriangleMesh(
	//this->body->SetCustomTag(this);
	//Oyster::Physics::ICustomBody* rigidBody = Oyster::Physics::API::Instance().AddCharacter(	1.0f, 1.0f, Oyster::Math::Float4(0, 0, 0, 1), 
	//																							this->position, 50.0f, 1.0f, 1.0f, 1.0f );
	return retVal;
}








