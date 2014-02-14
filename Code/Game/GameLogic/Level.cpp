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
Object* Level::createGameObj(ObjectHeader* obj, ICustomBody* rigidBody)
{
	Object* gameObj  =NULL;
	switch ((ObjectSpecialType)obj->specialTypeID)
	{
	case ObjectSpecialType_None: 
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_Sky: 
		{
			float skySize = ((SkyAttributes*)obj)->skySize; 
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_World: 
		{
			API::Instance().SetGravityPoint(Oyster::Math3D::Float3(0,0,0));
			API::Instance().SetGravity(200); // could balance gravitation with the world size

			float worldSize = ((WorldAttributes*)obj)->worldSize; 
			float atmosphereSize = ((WorldAttributes*)obj)->atmoSphereSize; 
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_Building: 
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
	case ObjectSpecialType_Stone: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_StandardBox: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_RedExplosiveBox: 
		{
			int dmg = 50; 
			//gameObj = new ExplosiveBox(rigidBody, ObjectSpecialType_RedExplosiveBox);
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	//case ObjectSpecialType_BlueExplosiveBox: 
	//	int dmg = 70; 
	//	gameObj = new ExplosiveBox(rigidBody, ObjectSpecialType_BlueExplosiveBox);
	//	break;
	case ObjectSpecialType_SpikeBox: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_Spike: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_CrystalFormation: 
		{
			int dmg = 50; 
			//gameObj = new Crystal(rigidBody);
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_CrystalShard: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_JumpPad: 
		{
			float power = ((JumpPadAttributes*)obj)->power; 
			Oyster::Math::Float3 dir = ((JumpPadAttributes*)obj)->direction; 
			//gameObj = JumpPad(rigidBody, );
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_Portal: 
		{
			Oyster::Math::Float3 destination = ((PortalAttributes*)obj)->destination; 
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_SpawnPoint: 
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	case ObjectSpecialType_Player: 
		{
			gameObj = new DynamicObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0);
		}
		break;
	case ObjectSpecialType_Generic:
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	default:
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultCollisionAfter, (ObjectSpecialType)obj->specialTypeID,0); 
		}
		break;
	}
	return gameObj;
}

ICustomBody* Level::InitRigidBodyCube( const ObjectHeader* obj)
{
	ICustomBody* rigidBody = NULL;
	Oyster::Math::Float3 rigidWorldPos;
	Oyster::Math::Float4 rigidWorldRotation;
	float rigidBodyMass;
	Oyster::Math::Float3 rigidBodySize;

	//offset the rigidPosition from modelspace to worldspace;
	rigidWorldPos = (Oyster::Math::Float3)obj->position + (Oyster::Math::Float3)obj->boundingVolume.box.position; 
	//scales the position so the collision geomentry is in the right place
	rigidWorldPos = rigidWorldPos * obj->scale;

	//offset the rigidRotation from modelspace to worldspace;
	Oyster::Math::Quaternion worldPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->rotation[0],obj->rotation[1],obj->rotation[2]), obj->rotation[3]);
	Oyster::Math::Quaternion physicsPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->boundingVolume.sphere.rotation[0],obj->boundingVolume.sphere.rotation[1],obj->boundingVolume.sphere.rotation[2]), obj->boundingVolume.sphere.rotation[3]);
	Oyster::Math::Quaternion rigidWorldQuaternion = worldPosQuaternion * physicsPosQuaternion; 

	rigidWorldRotation = Oyster::Math::Float4(rigidWorldQuaternion); 

	//mass scaled
	rigidBodyMass = obj->scale[0] * obj->scale[1] * obj->scale[2] * obj->boundingVolume.box.mass;

	//size scaled 
	rigidBodySize = (Oyster::Math::Float3)obj->boundingVolume.box.size * (Oyster::Math::Float3)obj->scale;

	//create the rigid body
	rigidBody = API::Instance().AddCollisionBox(rigidBodySize , rigidWorldRotation  , rigidWorldPos , rigidBodyMass, obj->boundingVolume.box.restitutionCoeff , obj->boundingVolume.box.frictionCoeffStatic , obj->boundingVolume.box.frictionCoeffDynamic);
	return rigidBody;
}
ICustomBody* Level::InitRigidBodySphere( const ObjectHeader* obj)
{
	ICustomBody* rigidBody = NULL;
	Oyster::Math::Float3 rigidWorldPos;
	Oyster::Math::Float4 rigidWorldRotation;
	float rigidBodyMass;
	float rigidBodyRadius;

	//offset the rigidPosition from modelspace to worldspace;
	rigidWorldPos = (Oyster::Math::Float3)obj->position + (Oyster::Math::Float3)obj->boundingVolume.sphere.position;
	//scales the position so the collision geomentry is in the right place
	rigidWorldPos = rigidWorldPos * obj->scale;

	//offset the rigidRotation from modelspace to worldspace;
	Oyster::Math::Quaternion worldPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->rotation[0],obj->rotation[1],obj->rotation[2]), obj->rotation[3]);
	Oyster::Math::Quaternion physicsPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->boundingVolume.sphere.rotation[0],obj->boundingVolume.sphere.rotation[1],obj->boundingVolume.sphere.rotation[2]), obj->boundingVolume.sphere.rotation[3]);
	Oyster::Math::Quaternion rigidWorldQuaternion = worldPosQuaternion * physicsPosQuaternion; 

	rigidWorldRotation = Oyster::Math::Float4(rigidWorldQuaternion); 


	//mass scaled
	rigidBodyMass = obj->scale[0] * obj->scale[1] * obj->scale[2] * obj->boundingVolume.sphere.mass;

	//Radius scaled
	//rigidBodyRadius = (staticObjData->scale[0] * staticObjData->scale[1] * staticObjData->scale[2] / 3) * staticObjData->boundingVolume.sphere.radius;
	rigidBodyRadius = (obj->scale[0] * obj->scale[1] * obj->scale[2]) * obj->boundingVolume.sphere.radius;

	//create the rigid body
	rigidBody = API::Instance().AddCollisionSphere( rigidBodyRadius , rigidWorldRotation , rigidWorldPos , rigidBodyMass, obj->boundingVolume.sphere.restitutionCoeff , obj->boundingVolume.sphere.frictionCoeffStatic , obj->boundingVolume.sphere.frictionCoeffDynamic);
	return rigidBody;
}
void Level::InitiateLevel(std::string levelPath)
{
	LevelLoader ll; 
	std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> objects; 
	objects = ll.LoadLevel(levelPath);

	API::Instance().SetGravityPoint(Oyster::Math3D::Float3(0,0,0));
	API::Instance().SetGravity(200);
	int objCount = objects.size();
	int modelCount = 100;

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
				staticObjData->ModelFile;

				ICustomBody* rigidBody_Static = NULL;	
				
				// collision shape
				if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Sphere)
				{
					rigidBody_Static = InitRigidBodySphere(staticObjData);
				}

				else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Box)
				{	

					rigidBody_Static = InitRigidBodySphere(staticObjData);
				}

				else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Cylinder)
				{
					//rigidBody_Static = InitRigidBodyCylinder(staticObjData);
				}

				if(rigidBody_Static != NULL)
				{
					this->staticObjects.Push(new StaticObject(rigidBody_Static, Object::DefaultCollisionAfter, (ObjectSpecialType)staticObjData->specialTypeID, 0));
					//this->staticObjects[staticObjCount]->objectID = modelCount++;

				}

			}
			break;
		case ObjectType::ObjectType_Dynamic:
			{
				ObjectHeader* dynamicObjData = ((ObjectHeader*)obj);
				dynamicObjData->ModelFile;

				ICustomBody* rigidBody_Dynamic = NULL;	

				// collision shape
				if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Sphere)
				{
					rigidBody_Dynamic = InitRigidBodySphere(dynamicObjData);
				}

				else if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Box)
				{	
					rigidBody_Dynamic = InitRigidBodyCube(dynamicObjData);
				}

				else if(dynamicObjData->boundingVolume.geoType == CollisionGeometryType_Cylinder)
				{
					//rigidBody_Dynamic = InitRigidBodyCylinder(dynamicObjData);
				}

				if(rigidBody_Dynamic != NULL)
				{

					this->dynamicObjects.Push(new DynamicObject(rigidBody_Dynamic , Object::DefaultCollisionAfter, (ObjectSpecialType)dynamicObjData->specialTypeID, 0));
					//this->dynamicObjects[dynamicObjCount]->objectID = modelCount++;
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

	levelObj = new StaticObject(rigidBody, LevelCollisionAfter, ObjectSpecialType_World, idCount++);

	//this->levelObj->objectID = idCount++;
	rigidBody->SetCustomTag(levelObj);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	int offset = 0;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, 10), 5, 0.5f, 0.8f, 0.6f);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox, Object::DefaultCollisionAfter, ObjectSpecialType_StandardBox, idCount++));

		//this->dynamicObjects[i]->objectID = idCount++;
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

	this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal, Object::DefaultCollisionAfter, ObjectSpecialType_StandardBox, idCount++));
	rigidBody_Crystal->SetCustomTag(this->dynamicObjects[nrOfBoxex]);
	//this->dynamicObjects[nrOfBoxex]->objectID = idCount++;
	


	// add house
	ICustomBody* rigidBody_House =API::Instance().AddCollisionBox(Oyster::Math::Float3(20, 20, 20), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(-50, 590, 0), 0, 0.5f, 0.8f, 0.6f);
	this->staticObjects.Push(new StaticObject(rigidBody_House, Object::DefaultCollisionAfter, ObjectSpecialType_Generic, idCount++));
	rigidBody_House->SetCustomTag(this->staticObjects[0]);
	//this->staticObjects[0]->objectID = idCount++;

	// add jumppad
	ICustomBody* rigidBody_Jumppad = API::Instance().AddCollisionBox(Oyster::Math::Float3(1, 1, 1), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(4, 600.3, 0), 5, 0.5f, 0.8f, 0.6f);

	this->staticObjects.Push(new JumpPad(rigidBody_Jumppad, ObjectSpecialType_JumpPad,idCount++ ,Oyster::Math::Float3(0,2000,0)));
	rigidBody_Jumppad->SetCustomTag(this->staticObjects[1]);
	//this->staticObjects[1]->objectID = idCount++;
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
