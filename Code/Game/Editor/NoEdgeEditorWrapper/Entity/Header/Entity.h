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

		} rigidBody;
		struct General
		{
			Oyster::Math::Float3 position; 
			Oyster::Math::Quaternion rotation; 
			Oyster::Math::Float3 scale; 
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
		EntityType_World,
	};
	union EntitySubType
	{
		NoEdgeType_Collision	et_collision;
		NoEdgeType_Projectiles	et_projectile;
		NoEdgeType_Interactive	et_interacive;
		NoEdgeType_Buildings	et_building;
		NoEdgeType_Light		et_light;
		int						memory;
		EntitySubType(int val)						{ memory = val; }
		EntitySubType(NoEdgeType_Collision val)		{ et_collision = val; }
		EntitySubType(NoEdgeType_Projectiles val)	{ et_projectile = val; }
		EntitySubType(NoEdgeType_Interactive val)	{ et_interacive = val; }
		EntitySubType(NoEdgeType_Buildings val)		{ et_building = val; }
		EntitySubType(NoEdgeType_Light val)			{ et_light = val; }
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
	operator Oyster::Graphics::Model::Model*() { return this->model; }

protected:
	Entity(EntityType type, EntitySubType sub);
	bool EntityInitialize(const EntityInitDesc& desc);

protected:
	const int							ID;
	const EntityType					entityType;
	const EntitySubType					entitySubType;
	Oyster::Physics::ICustomBody		*body;
	Oyster::Graphics::Model::Model		*model;
	Oyster::Math::Float3				position; 
	Oyster::Math::Quaternion			rotation; 
	Oyster::Math::Float3				scale;
	bool								locked;
};

#endif // !EDITOR_EDITORENTITY_H
