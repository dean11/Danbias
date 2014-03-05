#include "..\Header\Entity.h"
#include "LevelLoader\ParseFunctions.h"
#include "Resource\ResourceManager.h"

using namespace Oyster;
using namespace Oyster::Graphics;
using namespace Oyster::Math;
using namespace Oyster::Math3D;
static int globCount = 0;

bool Entity::HasMesh() const									   
{
	return (this->model.mesh != 0);
}
bool Entity::HasRigidBody() const								   
{
	return (this->body.rigid != 0);
}
bool Entity::IsVisible() const									   
{
	return this->model.mesh->Visible;
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
	if(this->model.mesh) this->model.mesh->Visible = true;
}
void Entity::Hide( )
{
	if(this->model.mesh) this->model.mesh->Visible = true;
}
void Entity::Lock( )
{
	this->locked = true;
}
void Entity::Unlock( )
{
	this->locked = false;
}
void Entity::Update( )
{
	if(this->model.mesh)	
		this->model.mesh->WorldMatrix = GetWorld();
}
void Entity::Render( )
{
	if(this->model.mesh)
		Oyster::Graphics::API::RenderModel(this->model.mesh);
}
bool Entity::TryPickup(Oyster::Collision3D::Ray ray)
{
	return Physics::API::Instance().Intersect(ray.origin.xyz, ray.direction.xyz) == this->body.rigid;
}
void Entity::Release()
{
	Graphics::API::DeleteModel(this->model.mesh);
	this->model.mesh = 0;
}
Entity::~Entity()
{
	this->Release();
}
Entity::Entity(EntityType t, EntitySubType sub)
	:	entityType(t)
	,	ID(globCount++)
	,	entitySubType(sub)
{
	model.mesh = 0;
	body.rigid = 0;
}
bool Entity::EntityInitialize(const EntityInitDesc& desc)
{
	bool retVal = true;
	this->position = desc.general.position;
	this->rotation = desc.general.rotation;
	this->scale = desc.general.scale;

	if(desc.mesh.modelPath.size() > 0)
	{
		this->model.mesh = Oyster::Graphics::API::CreateModel(desc.mesh.modelPath);
		if( !this->model.mesh )	
		{
			return false;
		}
		this->model.mesh->WorldMatrix = this->GetWorld();
	}

//Physics rigid body 
	{
		GameLogic::LevelLoaderInternal::BoundingVolume bv;
		std::wstring cgfPath = Default::String::DEFAULT_CGF_PATH;
		cgfPath.append(desc.rigidBody.cgfFile);
		if(GameLogic::LevelFileLoader::ParseCGF(cgfPath, bv))
		{
			if( bv.geoType == GameLogic::CollisionGeometryType_CG_MESH)
			{
				std::wstring bulletPath = Default::String::DEFAULT_BULLET_PATH;
				bulletPath.append(bv.cgMesh.filename);
				this->body.rigid = Physics::API::Instance().AddTriangleMesh(  bulletPath
																			, Float4(desc.general.rotation.imaginary, desc.general.rotation.real) 
																			, desc.general.position
																			, desc.general.mass 
																			, desc.general.restitution
																			, desc.general.staticFriction
																			, desc.general.dynamicFriction  );
			}
			else if (bv.geoType == GameLogic::CollisionGeometryType_Cylinder)
			{
				this->body.rigid = Physics::API::Instance().AddCharacter( bv.cylinder.length
																		, bv.cylinder.radius
																		, Float4(desc.general.rotation.imaginary, desc.general.rotation.real) 
																		, desc.general.position
																		, desc.general.mass 
																		, desc.general.restitution
																		, desc.general.staticFriction
																		, desc.general.dynamicFriction  );
			}
			else if (bv.geoType == GameLogic::CollisionGeometryType_Box)
			{
				this->body.rigid = Physics::API::Instance().AddCollisionBox(  bv.box.size
																			, Float4(desc.general.rotation.imaginary, desc.general.rotation.real) 
																			, desc.general.position
																			, desc.general.mass 
																			, desc.general.restitution
																			, desc.general.staticFriction
																			, desc.general.dynamicFriction  );
			}
			else if (bv.geoType == GameLogic::CollisionGeometryType_Sphere)
			{
				this->body.rigid = Physics::API::Instance().AddCollisionSphere(  bv.sphere.radius
																			, Float4(desc.general.rotation.imaginary, desc.general.rotation.real) 
																			, desc.general.position
																			, desc.general.mass 
																			, desc.general.restitution
																			, desc.general.staticFriction
																			, desc.general.dynamicFriction  );
			}
		
			
			if(! this->body.rigid ) 
			{
				// If this happens something is probably wrong within physics api or something
				Oyster::Graphics::API::DeleteModel( this->model.mesh );
				return false;
			}
			this->body.rigid->SetCustomTag(this);
		}
	}
	return retVal;
}








