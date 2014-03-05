#ifndef EDITOR_EDITORENTITY_H
#define EDITOR_EDITORENTITY_H

#ifdef NOEDGE_EDITOR_DLL_EXPORT
	#define NOEDGE_EDITOR_DLL __declspec(dllexport)
#else
	#define NOEDGE_EDITOR_DLL __declspec(dllimport)
#endif

#include "..\NoEdgeTypes.h"
#include "..\..\NoEdgeEditorWrapper.h"
#include "GamePhysics\PhysicsAPI.h"
#include "DllInterfaces\GFXAPI.h"
#include "Utilities.h"
#include "..\..\EditorDefault.h"


class Entity 
{
public:
	struct  EntityInitDesc
	{
		struct Mesh
		{
			std::wstring modelPath;
		} mesh;
		struct RigidBody
		{
			std::wstring cgfFile;
		} rigidBody;
		struct General
		{
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
			float mass;
			float restitution;
			float staticFriction;
			float dynamicFriction;
		} general;
	};

public:
	enum EntityType
	{
		EntityType_Interactive,
		EntityType_Light,
		EntityType_Building,
		EntityType_Projectiles,
		EntityType_Collision,
		EntityType_HazardEnv,
		EntityType_Pickup,
		EntityType_Special,
	};
	union EntitySubType
	{
		NoEdgeType_Collision	et_collision;
		NoEdgeType_Projectiles	et_projectile;
		NoEdgeType_Interactive	et_interacive;
		NoEdgeType_Buildings	et_building;
		NoEdgeType_Light		et_light;
		NoEdgeType_HazardEnv	et_hazard;
		NoEdgeType_Special		et_special;
		NoEdgeType_Pickup		et_pickup;
		int						memory;
		EntitySubType(int val)						{ memory = val; }
		EntitySubType(NoEdgeType_Collision val)		{ et_collision = val; }
		EntitySubType(NoEdgeType_Projectiles val)	{ et_projectile = val; }
		EntitySubType(NoEdgeType_Interactive val)	{ et_interacive = val; }
		EntitySubType(NoEdgeType_Buildings val)		{ et_building = val; }
		EntitySubType(NoEdgeType_Light val)			{ et_light = val; }
		EntitySubType(NoEdgeType_HazardEnv val)		{ et_hazard = val; }
		EntitySubType(NoEdgeType_Special val)		{ et_special = val; }
		EntitySubType(NoEdgeType_Pickup val)		{ et_pickup = val; }
	};

public:
	bool								HasMesh()			const;
	bool								HasRigidBody()		const;
	bool								IsVisible()			const;
	bool								IsLocked()			const;
	EntityType							GetEntityType()		const;
	EntitySubType						GetEntitySubType()	const;
	int									GetID()				const;
	virtual Oyster::Math::Float3		GetPosition()		const;
	virtual Oyster::Math::Quaternion	GetRotation()		const;
	virtual Oyster::Math::Float3		GetScale()			const;
	virtual Oyster::Math::Matrix		GetWorld()			const;

	virtual void SetPosition(Oyster::Math::Float3 position);
	virtual void SetRotation(Oyster::Math::Quaternion roation);
	virtual void SetScale(Oyster::Math::Float3 scale);


	virtual void Show( );
	virtual void Hide( );

	void Lock( );
	void Unlock( );
	
	void Update();
	void Render();

	virtual bool TryPickup(Oyster::Collision3D::Ray ray);

	virtual void Release();

	virtual~Entity();
	operator Oyster::Graphics::Model::Model*() { return this->model.mesh; }

protected:
	Entity(EntityType type, EntitySubType sub);
	bool EntityInitialize(const EntityInitDesc& desc);

protected:
	struct MeshInformation
	{
		std::wstring						name;
		Oyster::Graphics::Model::Model		*mesh;
	};
	struct RigidBoydInformation
	{
		std::wstring						name;
		Oyster::Physics::ICustomBody		*rigid;
	};
	const int							ID;
	const EntityType					entityType;
	const EntitySubType					entitySubType;
	RigidBoydInformation				body;
	MeshInformation						model;

	Oyster::Math::Float3				position; 
	Oyster::Math::Quaternion			rotation; 
	Oyster::Math::Float3				scale;
	bool								locked;
};

#endif // !EDITOR_EDITORENTITY_H
