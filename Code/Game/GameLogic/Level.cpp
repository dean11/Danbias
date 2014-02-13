#include "Level.h"
#include "CollisionManager.h"
#include "Game.h"

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;


Level::Level(void)
{

}
Level::~Level(void)
{
	delete this->levelObj;
	this->levelObj = NULL;
}
void Level::parseObjectType(ObjectTypeHeader* obj)
{
	/*switch (obj->objectTypeID)
	{
	case skySphere: 
		// save the skysphere to be able to rotate it 
		break;
	case jumppad: 
		// save direction 
		break;
	case portal: 
		// save portal destination 
		break;
	case world: 
		// add gravitation well here
		// add outer limit of the world
	case spawn: 
		// save spawnpoint pos 
		break;

	default:
		break;
	}*/
}
void Level::parsePhysicsObj(LevelLoaderInternal::BoundingVolumeBase* obj)
{
	// offset physObj med modelObj
}
void Level::InitiateLevel(std::string levelPath)
{
	LevelLoader ll; 
	std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> objects; 
	objects = ll.LoadLevel(levelPath);
	int objCount = objects.size();
	int modelCount = 0;
	int staticObjCount = 0;
	int dynamicObjCount = 0;
	Oyster::Math::Float3 rigidWorldPos;
	Oyster::Math::Float4 rigidWorldRotation;
	float rigidBodyMass;
	float rigidBodyRadius;
	Oyster::Math::Float3 rigidBodySize;
	for (int i = 0; i < objCount; i++)
	{
		ObjectTypeHeader* obj = objects.at(i);
		int id = obj->typeID;
		switch (obj->typeID)
		{
		case ObjectType::ObjectType_LevelMetaData:
			{
				LevelMetaData* LevelObjData = ((LevelMetaData*)obj);
				std::string levelName = LevelObjData->levelName;
				// LevelObjData->worldSize;
			}
			break;
		case ObjectType::ObjectType_Static:
			{
				
				ObjectHeader* staticObjData = ((ObjectHeader*)obj);
				//LevelLoaderInternal::BoundingVolumeBase* staticObjPhysicData = ((ObjectHeader*)obj);
				staticObjData->ModelFile;

				ICustomBody* rigidBody_Static = NULL;	
				

				// collision shape
				// radius, rotation in world, position in world, mass, restitution, static and dynamic friction
				if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Sphere)
				{
					//offset the rigidPosition from modelspace to worldspace;
					rigidWorldPos = (Oyster::Math::Float3)staticObjData->position + (Oyster::Math::Float3)staticObjData->boundingVolume.sphere.position;
					//scales the position so the collision geomentry is in the right place
					rigidWorldPos = rigidWorldPos * staticObjData->scale;

					//offset the rigidRotation from modelspace to worldspace;
					rigidWorldRotation = (Oyster::Math::Float4)staticObjData->rotation + (Oyster::Math::Float4)staticObjData->boundingVolume.sphere.rotation; 

					//mass scaled
					rigidBodyMass = staticObjData->scale[0] * staticObjData->scale[1] * staticObjData->scale[2] * staticObjData->boundingVolume.sphere.mass;

					//Radius scaled
					rigidBodyRadius = (staticObjData->scale[0] * staticObjData->scale[1] * staticObjData->scale[2] / 3) * staticObjData->boundingVolume.sphere.radius;

					//create the rigid body
					rigidBody_Static = API::Instance().AddCollisionSphere( rigidBodyRadius , rigidWorldRotation , rigidWorldPos , rigidBodyMass, staticObjData->boundingVolume.sphere.restitutionCoeff , staticObjData->boundingVolume.sphere.frictionCoeffStatic , staticObjData->boundingVolume.sphere.frictionCoeffDynamic);
				}

				else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Box)
				{	
					//offset the rigidPosition from modelspace to worldspace;
					rigidWorldPos = (Oyster::Math::Float3)staticObjData->position + (Oyster::Math::Float3)staticObjData->boundingVolume.box.position; 
					//scales the position so the collision geomentry is in the right place
					rigidWorldPos = rigidWorldPos * staticObjData->scale;

					//offset the rigidRotation from modelspace to worldspace;
					rigidWorldRotation = (Oyster::Math::Float4)staticObjData->rotation + (Oyster::Math::Float4)staticObjData->boundingVolume.box.rotation;

					//mass scaled
					rigidBodyMass = staticObjData->scale[0] * staticObjData->scale[1] * staticObjData->scale[2] * staticObjData->boundingVolume.box.mass;

					//size scaled 
					rigidBodySize = (Oyster::Math::Float3)staticObjData->boundingVolume.box.size * (Oyster::Math::Float3)staticObjData->scale;

					//create the rigid body
					rigidBody_Static = API::Instance().AddCollisionBox(rigidBodySize , rigidWorldRotation  , rigidWorldPos , rigidBodyMass, staticObjData->boundingVolume.box.restitutionCoeff , staticObjData->boundingVolume.box.frictionCoeffStatic , staticObjData->boundingVolume.box.frictionCoeffDynamic);
				}

				else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Cylinder)
				{
			
				}


				// add rigidbody to the logical obj
				// Object::DefaultCollisionBefore, Object::DefaultCollisionAfter for now, gamelogic will take care of this
				// set object_type to objID 

				if(rigidBody_Static != NULL)
				{
					this->staticObjects.Push(new StaticObject(rigidBody_Static, Object::DefaultCollisionAfter, (ObjectSpecialType)staticObjData->specialTypeID));
					this->staticObjects[staticObjCount]->objectID = modelCount++;
					rigidBody_Static->SetCustomTag(this->staticObjects[staticObjCount]);
				}

			}
			break;
		case ObjectType::ObjectType_Dynamic:
			{
				ObjectHeader* dynamicObjData = ((ObjectHeader*)obj);
				dynamicObjData->ModelFile;

				ICustomBody* rigidBody_Dynamic = NULL;	

				// collision shape
				// radius, rotation in world, position in world, mass, restitution, static and dynamic friction
				if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Sphere)
				{
					//offset the rigidPosition from modelspace to worldspace;
					rigidWorldPos = (Oyster::Math::Float3)dynamicObjData->position + (Oyster::Math::Float3)dynamicObjData->boundingVolume.sphere.position; 
					//scales the position so the collision geomentry is in the right place
					rigidWorldPos = rigidWorldPos * dynamicObjData->scale;

					//offset the rigidRotation from modelspace to worldspace;
					rigidWorldRotation = (Oyster::Math::Float4)dynamicObjData->rotation + (Oyster::Math::Float4)dynamicObjData->boundingVolume.sphere.rotation;

					//mass scaled
					rigidBodyMass = dynamicObjData->scale[0] * dynamicObjData->scale[1] * dynamicObjData->scale[2] * dynamicObjData->boundingVolume.sphere.mass;

					//size scaled
					rigidBodyRadius = (dynamicObjData->scale[0] * dynamicObjData->scale[1] * dynamicObjData->scale[2] / 3) * dynamicObjData->boundingVolume.sphere.radius;

					//create the rigid body
					rigidBody_Dynamic = API::Instance().AddCollisionBox(rigidBodyRadius , rigidWorldRotation  , rigidWorldPos , rigidBodyMass, dynamicObjData->boundingVolume.sphere.restitutionCoeff , dynamicObjData->boundingVolume.sphere.frictionCoeffStatic , dynamicObjData->boundingVolume.sphere.frictionCoeffDynamic);
				}

				else if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Box)
				{	
					//offset the rigidPosition from modelspace to worldspace;
					rigidWorldPos = (Oyster::Math::Float3)dynamicObjData->position + (Oyster::Math::Float3)dynamicObjData->boundingVolume.box.position; 
					//scales the position so the collision geomentry is in the right place
					rigidWorldPos = rigidWorldPos * dynamicObjData->scale;

					//offset the rigidRotation from modelspace to worldspace;
					rigidWorldRotation = (Oyster::Math::Float4)dynamicObjData->rotation + (Oyster::Math::Float4)dynamicObjData->boundingVolume.box.rotation;

					//mass scaled
					rigidBodyMass = dynamicObjData->scale[0] * dynamicObjData->scale[1] * dynamicObjData->scale[2] * dynamicObjData->boundingVolume.box.mass;

					//size scaled
					rigidBodySize = (Oyster::Math::Float3)dynamicObjData->boundingVolume.box.size * (Oyster::Math::Float3)dynamicObjData->scale;

					//create the rigid body
					rigidBody_Dynamic = API::Instance().AddCollisionBox(rigidBodySize , rigidWorldRotation  , rigidWorldPos , rigidBodyMass, dynamicObjData->boundingVolume.box.restitutionCoeff , dynamicObjData->boundingVolume.box.frictionCoeffStatic , dynamicObjData->boundingVolume.box.frictionCoeffDynamic);
				}

				else if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Cylinder)
				{
			
				}


				// add rigidbody to the logical obj
				// Object::DefaultCollisionBefore, Object::DefaultCollisionAfter for now, gamelogic will take care of this
				// set object_type to objID 
				if(rigidBody_Dynamic != NULL)
				{
					this->dynamicObjects.Push(new DynamicObject(rigidBody_Dynamic , Object::DefaultCollisionAfter, (ObjectSpecialType)dynamicObjData->specialTypeID));
					this->dynamicObjects[dynamicObjCount]->objectID = modelCount++;
					rigidBody_Dynamic->SetCustomTag(this->dynamicObjects[dynamicObjCount]);
				}

			}
			break;
		case ObjectType::ObjectType_Light:
			// read on client
			break;
		default:
			break;
		}
	}
}
void Level::InitiateLevel(float radius)
{
	API::Instance().SetGravityPoint(Oyster::Math3D::Float3(0,0,0));
	API::Instance().SetGravity(200);
	int idCount = 100;
	// add level sphere
	ICustomBody* rigidBody = API::Instance().AddCollisionSphere(599.2f, Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);

	levelObj = new StaticObject(rigidBody, LevelCollisionAfter, ObjectSpecialType_World);

	this->levelObj->objectID = idCount++;
	rigidBody->SetCustomTag(levelObj);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	int offset = 0;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, 10), 5, 0.5f, 0.8f, 0.6f);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox, Object::DefaultCollisionAfter, ObjectSpecialType_StandarsBox));

		this->dynamicObjects[i]->objectID = idCount++;
		rigidBody_TestBox->SetCustomTag(this->dynamicObjects[i]);
	}
	/*offset += nrOfBoxex;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0,5, -605 -( i*5)), 5);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
		rigidBody_TestBox->SetCustomTag(this->dynamicObjects[i+offset]);
		
	}
	offset += nrOfBoxex;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(200, 620 + ( i*7), 0), 5);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
		rigidBody_TestBox->SetCustomTag(this->dynamicObjects[i+offset]);	
	}
	offset += nrOfBoxex;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(5, 605 + i*5, 0), 5);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
		rigidBody_TestBox->SetCustomTag(this->dynamicObjects[i]);
		
	}*/
	




	// add crystal
	ICustomBody* rigidBody_Crystal = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(10, 605, 0), 5, 0.5f, 0.8f, 0.6f);

	this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal, Object::DefaultCollisionAfter, ObjectSpecialType_StandarsBox));
	rigidBody_Crystal->SetCustomTag(this->dynamicObjects[nrOfBoxex]);
	this->dynamicObjects[nrOfBoxex]->objectID = idCount++;
	


	// add house
	ICustomBody* rigidBody_House =API::Instance().AddCollisionBox(Oyster::Math::Float3(20, 20, 20), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(-50, 590, 0), 0, 0.5f, 0.8f, 0.6f);
	this->staticObjects.Push(new StaticObject(rigidBody_House, Object::DefaultCollisionAfter, ObjectSpecialType_Generic));
	rigidBody_House->SetCustomTag(this->staticObjects[0]);
	this->staticObjects[0]->objectID = idCount++;

	// add jumppad
	ICustomBody* rigidBody_Jumppad = API::Instance().AddCollisionBox(Oyster::Math::Float3(1, 1, 1), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(4, 600.3, 0), 5, 0.5f, 0.8f, 0.6f);

	this->staticObjects.Push(new JumpPad(rigidBody_Jumppad, JumpPad::JumpPadActivated, ObjectSpecialType_JumpPad, Oyster::Math::Float3(0,2000,0)));
	rigidBody_Jumppad->SetCustomTag(this->staticObjects[1]);
	this->staticObjects[1]->objectID = idCount++;
}

void Level::AddPlayerToTeam(Player *player, int teamID)
{
	this->teamManager.AddPlayerToTeam(player,teamID);
}

void Level::CreateTeam(int teamSize)
{
	this->teamManager.CreateTeam(teamSize);
}

void Level::RespawnPlayer(Player *player)
{
	this->teamManager.RespawnPlayerRandom(player);
}

int Level::getNrOfDynamicObj()
{
	return this->dynamicObjects.Size(); 
}
Object* Level::GetObj( int ID) const
{
	for (int i = 0; i < this->dynamicObjects.Size(); i++)
	{
		if(this->dynamicObjects[i]->GetID() == ID)
			return this->dynamicObjects[i];
	}
	return NULL;
}
void Level::PhysicsOnMoveLevel(const ICustomBody *object)
{
	// function call from physics update when object was moved
	Object* temp = (Object*)object->GetCustomTag();
	((Game*)&Game::Instance())->onMoveFnc(temp);
	
}
