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
				//if( staticObjPhysicData->geometryType = CollisionGeometryType_Box)
				//{
				//	//API::SimpleBodyDescription sbDesc_Static;
			
				//	//sbDesc_Static.centerPosition = staticObjData->position;
				//	//sbDesc_Static.ignoreGravity = false; // because it is static
				//	//sbDesc_Static.rotation = Oyster::Math::Float3(staticObjData->rotation[0], staticObjData->rotation[1],staticObjData->rotation[2]);//Oyster::Math::Float3(0 ,Utility::Value::Radian(90.0f), 0);


				//	////sbDesc_Static.inertiaTensor.Cuboid(staticObjPhysicData->mass); 	
				//	//sbDesc_Static.mass = staticObjPhysicData->mass;
				//	//sbDesc_Static.frictionCoeff_Static = staticObjPhysicData->frictionCoeffStatic;
				//	//sbDesc_Static.frictionCoeff_Dynamic = staticObjPhysicData->frictionCoeffDynamic;
				//	////sbDesc_Static.restitutionCoeff = 
				//	//sbDesc_Static.size = Oyster::Math::Float3(40,40,40); 
				//	//rigidBody_Static = API::Instance().CreateRigidBody(sbDesc_Static).Release();
				//	//if(rigidBody_Static)
				//	//{
				//	//	this->staticObjects.Push(new StaticObject(rigidBody_Static,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
				//	//	int id = this->staticObjects.Size()-1;
				//	//	rigidBody_Static->SetCustomTag(this->staticObjects[this->staticObjects.Size()-1]);
				//	//}
				//}
				//if( staticObjPhysicData->geometryType = CollisionGeometryType_Sphere)
				//{
				//	//API::SphericalBodyDescription sbDesc_Static;

				//	//sbDesc_Static.centerPosition = staticObjData->position;
				//	//sbDesc_Static.ignoreGravity = true; // because it is static
				//	//sbDesc_Static.rotation = Oyster::Math::Float3(staticObjData->rotation[0], staticObjData->rotation[1],staticObjData->rotation[2]);//Oyster::Math::Float3(0 ,Utility::Value::Radian(90.0f), 0);

				//	////sbDesc_Static.inertiaTensor.Sphere(staticObjPhysicData->mass); 
	
				//	//sbDesc_Static.mass = staticObjPhysicData->mass;
				//	//sbDesc_Static.frictionCoeff_Static = staticObjPhysicData->frictionCoeffStatic;
				//	//sbDesc_Static.frictionCoeff_Dynamic = staticObjPhysicData->frictionCoeffDynamic;
				//	////sbDesc_Static.restitutionCoeff = 
				//	////sbDesc_Static.radius = 
				//	//rigidBody_Static = API::Instance().CreateRigidBody(sbDesc_Static).Release();
				//	if(rigidBody_Static)
				//	{
				//		this->staticObjects.Push(new StaticObject(rigidBody_Static,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
				//		int id = this->staticObjects.Size()-1;
				//		rigidBody_Static->SetCustomTag(this->staticObjects[this->staticObjects.Size()-1]);
				//	}

				//	if (OBJECT_TYPE::OBJECT_TYPE_WORLD)
				//	{
				//		/*API::Gravity gravityWell;
				//		gravityWell.gravityType = API::Gravity::GravityType_Well;
				//		gravityWell.well.mass = 1e17f;
				//		gravityWell.well.position = Oyster::Math::Float4(0,0,0,1);
				//		API::Instance().AddGravity(gravityWell);*/
				//	}
				//}
			}
			break;
		case ObjectType::ObjectType_Dynamic:
			{
				ObjectHeader* staticObjData = ((ObjectHeader*)obj);
				//LevelLoaderInternal::BoundingVolumeBase* staticObjPhysicData = ((ObjectHeader*)obj);
				staticObjData->ModelFile;

				ICustomBody* rigidBody_Dynamic;			
				//if( staticObjPhysicData->geometryType = CollisionGeometryType_Box)
				//{
				//	//API::Instance().AddCollisionBox() 
				//	//API::SimpleBodyDescription sbDesc_Dynamic;

				//	//sbDesc_Dynamic.centerPosition = staticObjData->position;
				//	//sbDesc_Dynamic.ignoreGravity = false; // because it is static
				//	//sbDesc_Dynamic.rotation = Oyster::Math::Float3(staticObjData->rotation[0], staticObjData->rotation[1],staticObjData->rotation[2]);//Oyster::Math::Float3(0 ,Utility::Value::Radian(90.0f), 0);


				//	////sbDesc_Static.inertiaTensor.Cuboid(staticObjPhysicData->mass); 	
				//	//sbDesc_Dynamic.mass = staticObjPhysicData->mass;
				//	//sbDesc_Dynamic.frictionCoeff_Static = staticObjPhysicData->frictionCoeffStatic;
				//	//sbDesc_Dynamic.frictionCoeff_Dynamic = staticObjPhysicData->frictionCoeffDynamic;
				//	////sbDesc_Static.restitutionCoeff = 
				//	//sbDesc_Dynamic.size = Oyster::Math::Float3(40,40,40); 
				//	//rigidBody_Dynamic = API::Instance().CreateRigidBody(sbDesc_Dynamic).Release();
				//	//if(rigidBody_Dynamic)
				//	//{
				//	//	rigidBody_Dynamic->SetSubscription(Level::PhysicsOnMoveLevel);
				//	//	this->dynamicObjects.Push(new DynamicObject(rigidBody_Dynamic,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
				//	//	int id = this->dynamicObjects.Size()-1;
				//	//	rigidBody_Dynamic->SetCustomTag(this->dynamicObjects[this->dynamicObjects.Size()-1]);
				//	//}
				//}
				//if( staticObjPhysicData->geometryType = CollisionGeometryType_Sphere)
				//{
				//	//API::Instance().AddCollisionBox() 
				//	//API::SphericalBodyDescription sbDesc_Dynamic;

				//	//sbDesc_Dynamic.centerPosition = staticObjData->position;
				//	//sbDesc_Dynamic.ignoreGravity = false; // use gravity on dynamic obj
				//	//sbDesc_Dynamic.rotation = Oyster::Math::Float3(staticObjData->rotation[0], staticObjData->rotation[1],staticObjData->rotation[2]);//Oyster::Math::Float3(0 ,Utility::Value::Radian(90.0f), 0);

				//	////sbDesc_Static.inertiaTensor.Sphere(staticObjPhysicData->mass); 

				//	//sbDesc_Dynamic.mass = staticObjPhysicData->mass;
				//	//sbDesc_Dynamic.frictionCoeff_Static = staticObjPhysicData->frictionCoeffStatic;
				//	//sbDesc_Dynamic.frictionCoeff_Dynamic = staticObjPhysicData->frictionCoeffDynamic;
				//	////sbDesc_Static.restitutionCoeff = 
				//	////sbDesc_Static.radius = 
				//	//rigidBody_Dynamic = API::Instance().CreateRigidBody(sbDesc_Dynamic).Release();
				//	//if(rigidBody_Dynamic)
				//	//{
				//	//	rigidBody_Dynamic->SetSubscription(Level::PhysicsOnMoveLevel);
				//	//	this->dynamicObjects.Push(new DynamicObject(rigidBody_Dynamic,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
				//	//	int id = this->dynamicObjects.Size()-1;
				//	//	rigidBody_Dynamic->SetCustomTag(this->dynamicObjects[this->dynamicObjects.Size()-1]);
				//	//}
				//}
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
	float heading = Utility::Value::Radian(180.0f);
	float attitude = Utility::Value::Radian(0.0f);
	float bank = Utility::Value::Radian(0);

	double c1 = cos(heading/2);
	double s1 = sin(heading/2);
	double c2 = cos(attitude/2);
	double s2 = sin(attitude/2);
	double c3 = cos(bank/2);
	double s3 = sin(bank/2);
	double c1c2 = c1*c2;
	double s1s2 = s1*s2;
	double w =c1c2*c3 - s1s2*s3;
	double x =c1c2*s3 + s1s2*c3;
	double y =s1*c2*c3 + c1*s2*s3;
	double z =c1*s2*c3 - s1*c2*s3;
	double angle = 2 * acos(w);

	double norm = x*x+y*y+z*z;
	if (norm < 0.001) { // when all euler angles are zero angle =0 so
		// we can set axis to anything to avoid divide by zero
		x=1;
		y=z=0;
	} else {
		norm = sqrt(norm);
		x /= norm;
		y /= norm;
		z /= norm;
	}

	int idCount = 100;
	// add level sphere
	ICustomBody* rigidBody = API::Instance().AddCollisionSphere(599.2f, Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0, 0.5f, 0.8f, 0.6f);
	levelObj = new StaticObject(rigidBody, LevelCollisionBefore, LevelCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	this->levelObj->objectID = idCount++;
	rigidBody->SetCustomTag(levelObj);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	int offset = 0;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, 10), 5, 0.5f, 0.8f, 0.6f);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
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
	




	//// add crystal

	//ICustomBody* rigidBody_Crystal = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(10, 605, 0), 5);

	//this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
	//rigidBody_Crystal->SetCustomTag(this->dynamicObjects[nrOfBoxex]);

	//


	//// add house
	//ICustomBody* rigidBody_House =API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(10, 905, 0), 0);
	//this->staticObjects.Push(new StaticObject(rigidBody_House,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
	//rigidBody_House->SetCustomTag(this->staticObjects[0]);
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
