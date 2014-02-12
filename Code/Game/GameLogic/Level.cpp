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

				ICustomBody* rigidBody_Static;	

				// collision shape
				// radius, rotation in world, position in world, mass, restitution, static and dynamic friction
				ICustomBody* rigidBody = API::Instance().AddCollisionSphere(599.2f, Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);
				
				// add rigidbody to the logical obj
				// Object::DefaultCollisionBefore, Object::DefaultCollisionAfter for now, gamelogic will take care of this
				// set object_type to objID 
				this->staticObjects.Push(new StaticObject(rigidBody, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));

				this->staticObjects[staticObjCount]->objectID = modelCount++;
				rigidBody->SetCustomTag(this->staticObjects[staticObjCount]);
			}
			break;
		case ObjectType::ObjectType_Dynamic:
			{
				ObjectHeader* staticObjData = ((ObjectHeader*)obj);
				staticObjData->ModelFile;

				ICustomBody* rigidBody_Dynamic;			

				rigidBody_Dynamic = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, 10), 5, 0.5f, 0.8f, 0.6f);

				this->dynamicObjects.Push(new DynamicObject(rigidBody_Dynamic, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
				this->dynamicObjects[dynamicObjCount]->objectID = modelCount++;
				rigidBody_Dynamic->SetCustomTag(this->dynamicObjects[dynamicObjCount]);
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
	levelObj = new StaticObject(rigidBody, LevelCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	this->levelObj->objectID = idCount++;
	rigidBody->SetCustomTag(levelObj);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	int offset = 0;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, 10), 5, 0.5f, 0.8f, 0.6f);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
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

	this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
	rigidBody_Crystal->SetCustomTag(this->dynamicObjects[nrOfBoxex]);
	this->dynamicObjects[nrOfBoxex]->objectID = idCount++;
	


	// add house
	ICustomBody* rigidBody_House =API::Instance().AddCollisionBox(Oyster::Math::Float3(20, 20, 20), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(-50, 590, 0), 0, 0.5f, 0.8f, 0.6f);
	this->staticObjects.Push(new StaticObject(rigidBody_House, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
	rigidBody_House->SetCustomTag(this->staticObjects[0]);
	this->staticObjects[0]->objectID = idCount++;

	// add jumppad
	ICustomBody* rigidBody_Jumppad = API::Instance().AddCollisionBox(Oyster::Math::Float3(1, 1, 1), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(4, 600.3, 0), 5, 0.5f, 0.8f, 0.6f);

	this->staticObjects.Push(new JumpPad(rigidBody_Jumppad, JumpPad::JumpPadActivated, OBJECT_TYPE::OBJECT_TYPE_JUMPPAD, Oyster::Math::Float3(0,2000,0)));
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
	for (int i = 0; i< this->dynamicObjects.Size(); i++)
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
