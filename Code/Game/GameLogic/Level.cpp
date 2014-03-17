#include "Level.h"
#include "CollisionManager.h"
#include "Game.h"
#include "JumpPad.h"
#include "ExplosiveCrate.h"
#include "Portal.h"
#include <Resource\OResource.h>

//Conversion from wstring to string
#include <codecvt>

using namespace GameLogic;
using namespace Utility::DynamicMemory;
using namespace Oyster::Physics;
using namespace Oyster::Math;

Level::Level(void)
{
	srand ((unsigned int)time(NULL));
	objIDCounter = 100; 
}
Level::~Level(void)
{
}
Object* Level::CreateGameObj(ObjectHeader* obj, ICustomBody* rigidBody)
{
	Object* gameObj  = NULL;
	
	switch ((ObjectSpecialType)obj->specialTypeID)
	{
	case ObjectSpecialType_None: 
		{
			//gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter); 
		}
		break;
	case ObjectSpecialType_World: 
		{
			
			API::Instance().SetGravityPoint(Oyster::Math3D::Float3(((WorldAttributes*)obj)->position));
			//API::Instance().SetGravity(200); // could balance gravitation with the world size

			float worldSize = ((WorldAttributes*)obj)->worldSize; 
			float atmosphereSize = ((WorldAttributes*)obj)->atmoSphereSize; 

			gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter); 
		}
		break;
	case ObjectSpecialType_Building: 
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter); 
		}
		break;
	case ObjectSpecialType_Stone: 
		{
			gameObj = new DynamicObject(rigidBody, DynamicObject::DynamicDefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter);
		}
		break;
	case ObjectSpecialType_StandardBox: 
		{
			gameObj = new DynamicObject(rigidBody, DynamicObject::DynamicDefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter);
		}
		break;
	case ObjectSpecialType_RedExplosiveBox: 
		{
			Oyster::Math::Float dmg = 50; 
			Oyster::Math::Float force = 50; 
			Oyster::Math::Float radie = 3; 
			gameObj = new ExplosiveCrate(rigidBody, (ObjectSpecialType)obj->specialTypeID, objIDCounter, dmg, force, radie);
		}
		break;
	case ObjectSpecialType_SpikeBox: 
		{
			gameObj = new DynamicObject(rigidBody, DynamicObject::DynamicDefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter);
		}
		break;
	case ObjectSpecialType_Spike: 
		{
			gameObj = new DynamicObject(rigidBody, DynamicObject::DynamicDefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter);
		}
		break;
	case ObjectSpecialType_CrystalFormation: 
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter, (float)NoEdgeConstants::Values::Enviroment::CrystalDamage); 
		}
		break;
	case ObjectSpecialType_CrystalShard: 
		{
			gameObj = new DynamicObject(rigidBody, DynamicObject::DynamicDefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter);
		}
		break;
	case ObjectSpecialType_JumpPad: 
		{
			float power = ((JumpPadAttributes*)obj)->power; 
			Oyster::Math::Float3 pos = rigidBody->GetState().centerPos;

			Oyster::Math::Float3 dir = pos.GetNormalized();

			Oyster::Math::Float3 pushForce = dir * power; 
			gameObj = new JumpPad(rigidBody, (ObjectSpecialType)obj->specialTypeID, objIDCounter , pushForce);
		}
		break;
	case ObjectSpecialType_Portal: 
		{
			Oyster::Math::Float3 destination = ((PortalAttributes*)obj)->destination; 
			gameObj = new Portal(rigidBody, (ObjectSpecialType)obj->specialTypeID, objIDCounter, destination);
		}
		break;
	case ObjectSpecialType_Generic:
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter); 
		}
		break;
	case ObjectSpecialType_PickupHealth:
		{
			gameObj = new PickupHealth(rigidBody, obj->specialTypeID, objIDCounter);
		}
		break;
	default:
		{
			gameObj = new StaticObject(rigidBody, Object::DefaultOnCollision, (ObjectSpecialType)obj->specialTypeID, objIDCounter); 
		}
		break;
	}
	gameObj->SetInitialPos(rigidBody->GetState().centerPos);
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
	rigidBody = API::Instance().AddCollisionBox(rigidBodySize , rigidWorldRotation  , rigidWorldPos , rigidBodyMass, obj->boundingVolume.box.restitutionCoeff , 1.8f , obj->boundingVolume.box.frictionCoeffDynamic);
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
	rigidBodyRadius = (obj->scale[0]) * obj->boundingVolume.sphere.radius;
	//rigidBodyRadius = (obj->scale[0] * obj->scale[1] * obj->scale[2]) * obj->boundingVolume.sphere.radius;

	//create the rigid body
	rigidBody = API::Instance().AddCollisionSphere( rigidBodyRadius , rigidWorldRotation , rigidWorldPos , rigidBodyMass, obj->boundingVolume.sphere.restitutionCoeff , obj->boundingVolume.sphere.frictionCoeffStatic , obj->boundingVolume.sphere.frictionCoeffDynamic);
	return rigidBody;
}
ICustomBody* Level::InitRigidBodyMesh( const ObjectHeader* obj)
{
	ICustomBody* rigidBody = NULL;
	Oyster::Math::Float3 rigidWorldPos;
	Oyster::Math::Float4 rigidWorldRotation;
	float rigidBodyMass;

	//offset the rigidPosition from modelspace to worldspace;
	rigidWorldPos = (Oyster::Math::Float3)obj->position + (Oyster::Math::Float3)obj->boundingVolume.cgMesh.position;
	//scales the position so the collision geomentry is in the right place
	
	//offset the rigidRotation from modelspace to worldspace;
	Oyster::Math::Quaternion worldPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->rotation[0],obj->rotation[1],obj->rotation[2]), obj->rotation[3]);
	Oyster::Math::Quaternion physicsPosQuaternion = Oyster::Math::Quaternion(Oyster::Math::Float3(obj->boundingVolume.cgMesh.rotation[0],obj->boundingVolume.cgMesh.rotation[1],obj->boundingVolume.cgMesh.rotation[2]), obj->boundingVolume.cgMesh.rotation[3]);
	Oyster::Math::Quaternion rigidWorldQuaternion = worldPosQuaternion * physicsPosQuaternion; 

	rigidWorldRotation = Oyster::Math::Float4(rigidWorldQuaternion); 


	//mass scaled
	rigidBodyMass = obj->scale[0] * obj->scale[1] * obj->scale[2] * obj->boundingVolume.cgMesh.mass;

	//Radius scaled
	//rigidBodyRadius = (obj->scale[0]) * obj->boundingVolume.sphere.radius;
	//rigidBodyRadius = (obj->scale[0] * obj->scale[1] * obj->scale[2]) * obj->boundingVolume.sphere.radius;

	//create the rigid body
	std::wstring fname = L"..\\Content\\Worlds\\cgf\\";
	fname.append(obj->boundingVolume.cgMesh.filename);
	rigidBody = API::Instance().AddTriangleMesh( fname , rigidWorldRotation , rigidWorldPos, obj->scale, rigidBodyMass, obj->boundingVolume.cgMesh.restitutionCoeff , obj->boundingVolume.cgMesh.frictionCoeffStatic , obj->boundingVolume.cgMesh.frictionCoeffDynamic);
	return rigidBody;
}
bool Level::InitiateLevel(std::wstring levelPath)
{
	LevelLoader ll; 
	ll.SetFolderPath("..\\Content\\Worlds\\");
	std::vector<Utility::DynamicMemory::SmartPointer<ObjectTypeHeader>> objects; 

	//Convert from wstring to string
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	std::string convertedLevelPath = converterX.to_bytes(levelPath);
	objects = ll.LoadLevel(convertedLevelPath);

	if(objects.size() == 0)
		return false;

	API::Instance().SetGravityPoint(Oyster::Math3D::Float3(0,0,0));
	API::Instance().SetGravity(NoEdgeConstants::Values::Globals::Gravity);
	int objCount = (int)objects.size();

	for (int i = 0; i < objCount; i++)
	{
		++this->objIDCounter;
		ObjectTypeHeader* obj = objects.at(i);
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

				if(	staticObjData->specialTypeID != ObjectSpecialType_None &&
					staticObjData->specialTypeID != ObjectSpecialType_NonSolidRotationSlow && 
					staticObjData->specialTypeID != ObjectSpecialType_NonSolidRotationQuick)
				{
					// collision shape
					if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Sphere)
					{
						rigidBody_Static = InitRigidBodySphere(staticObjData);
					}

					else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Box)
					{
						rigidBody_Static = InitRigidBodyCube(staticObjData);
					}

					else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_Cylinder)
					{
						//rigidBody_Static = InitRigidBodyCylinder(staticObjData);
					}

					else if(staticObjData->boundingVolume.geoType == CollisionGeometryType_CG_MESH)
					{
						rigidBody_Static = InitRigidBodyMesh(staticObjData);
					}
				}

				if(rigidBody_Static != NULL)
				{
					// create game object
					Object* staticGameObj = CreateGameObj(staticObjData, rigidBody_Static);
					
					if(staticObjData->specialTypeID == ObjectSpecialType_PickupHealth)
					{
						this->pickupSystem.CreatePickup((PickupHealth*)staticGameObj);
					}
					else if(staticGameObj != NULL)
					{
						this->staticObjects.Push((StaticObject*)staticGameObj);
					}
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
					// create game object
					Object* dynamicGameObj = CreateGameObj(dynamicObjData, rigidBody_Dynamic);
					if (dynamicGameObj != NULL)
					{
						dynamicGameObj->GetRigidBody()->SetSubscription(Level::PhysicsOnMoveLevel);
						this->dynamicObjects.Push((DynamicObject*)dynamicGameObj);
					}
				}
			}
			break;
		case ObjectType::ObjectType_Light:
			// read on client
			break;
		case ObjectType::ObjectType_SpawnPoint:
			{
				Oyster::Math::Float3 pos;
				pos.x = ((SpawnPointAttributes*)obj)->position[0];
				pos.y = ((SpawnPointAttributes*)obj)->position[1];
				pos.z = ((SpawnPointAttributes*)obj)->position[2];
				spawnPoints.addSpawnPos(pos);
			}
		default:
			break;
		}
	}

	return true;
}
bool Level::InitiateGameMode(float maxTimeSec, int endKillScore)
{
	GameModeType::EndConditions end;
	end.endTimeSec = maxTimeSec; 
	end.killCount = endKillScore; 
	this->gameMode.initGameMode(end);
	return true;
}
void Level::AddPlayerToTeam(Player *player, int teamID)
{
	this->teamManager.AddPlayerToTeam(player,teamID);
}
void Level::AddPlayerToGame(Player *player)
{
	player->ResetPlayer(spawnPoints.getSpawnPos());
	for(int i = 0; i < (int)this->playerObjects.Size(); i++)
	{
		if (!this->playerObjects[i])
		{
			this->playerObjects[i] = player;
			return;
		}
	}
	// if no free space, allocate a new spot
	this->playerObjects.Push(player);
}
void Level::RemovePlayerFromGame(Player *player)
{
	for(int i = 0; i < (int)this->playerObjects.Size(); i++)
	{
		if (this->playerObjects[i] == player)
		{
			
			// remove object tags 
			for(int k = 0; k < (int)dynamicObjects.Size(); k++)
			{
				// get affecting player
				Player* temp = dynamicObjects[k]->getAffectingPlayer();
				if(temp  && temp == player)
				{
					// remove affected by tag
					dynamicObjects[k]->RemoveAffectedBy();
					if(temp->getManipulatingPlayer())
					{
						// if disconnecting while holding a object
						dynamicObjects[k]->RemoveManipulation();
					}
				}
			}

			// remove player tags
			for(int k = 0; k < (int)playerObjects.Size(); k++)
			{
				if(playerObjects[k])
				{	
					Player* temp = playerObjects[k]->getAffectingPlayer();
					if(temp  && temp == player)
					{
						playerObjects[k]->RemoveAffectedBy();
					}
				}
			}
			if(this->playerObjects.Size() == 1)
				this->playerObjects[i] =  0;
			else
			{
				//Swap with last'
				//this->playerObjects[i] = nullptr;
				//this->playerObjects.Resize(
				this->playerObjects.Swap(i, this->playerObjects.Size() - 1);
				this->playerObjects[i, this->playerObjects.Size() - 1] = 0;
			}
			break;
		}
	}
}
void Level::CreateTeam(int teamSize)
{
	this->teamManager.CreateTeam(teamSize);
}

void Level::RespawnPlayer(Player *player)
{
	//this->teamManager.RespawnPlayerRandom(player);
	player->Respawn(spawnPoints.getSpawnPos());
	
	// remove manipulation tag  
	for(int i = 0; i < (int)dynamicObjects.Size(); i++)
	{
		// get manipulating player
		Player* temp = dynamicObjects[i]->getManipulatingPlayer();
		if(temp  && temp == player)
		{
			player->UseWeapon(GameLogic::WEAPON_INTERRUPT);
			dynamicObjects[i]->RemoveManipulation();
		}
	}
}
void Level::Update(float deltaTime)
{
	// update lvl-things
	gameMode.Update(deltaTime);

	int winnerID =  gameMode.EndConditionMet(this->playerObjects);

	if (winnerID != -1 )
	{
		// game ends because of timer
		if (gameMode.TimeExit())
		{
		}

		//send message
		((Game*)&Game::Instance())->onEndGameFnc();
	}
	else
	{
		((Game*)&Game::Instance())->tickTimer += deltaTime;
		if(((Game*)&Game::Instance())->tickTimer >= ((Game*)&Game::Instance())->tickPeriod)
		{
			((Game*)&Game::Instance())->onGameTimeTickFnc(this->gameMode.GetGameTime());
			((Game*)&Game::Instance())->tickTimer = 0.0f;
		}
	}

	for(int i = 0; i < (int)this->playerObjects.Size(); i++)
	{
		if(this->playerObjects[i])
		{
			//if(this->playerObjects[i]->getAffectingPlayer() != NULL)
			//{
			//
			//}

			if (this->playerObjects[i]->GetState() == PLAYER_STATE::PLAYER_STATE_DEAD)
			{
				// true when timer reaches 0 
				if(this->playerObjects[i]->deathTimerTick(deltaTime))
					RespawnPlayer(this->playerObjects[i]);
			}
			else if (this->playerObjects[i]->GetState() == PLAYER_STATE::PLAYER_STATE_DIED)
			{
				this->playerObjects[i]->setDeathTimer((float)DEATH_TIMER);
				Player* killer = this->playerObjects[i]->getAffectingPlayer();
				if(!killer) //if there is no killer then you committed suicide
				{
					killer = this->playerObjects[i];
					// suicide penalty 
				}
				else
					killer->AddKill();

				this->playerObjects[i]->AddDeath();


				((Game*)&Game::Instance())->onDeadFnc(this->playerObjects[i], this->playerObjects[i]->GetDeath(), killer, killer->GetKills(), (float)DEATH_TIMER); // add killer ID

			}
		}
	}

	for(int i = 0; i < (int)dynamicObjects.Size(); i++)
	{
		if(dynamicObjects[i]->getAffectingPlayer() != NULL)
		{
			Oyster::Math::Float vel = dynamicObjects[i]->GetRigidBody()->GetLinearVelocity().GetMagnitude();

			if(vel <= 0.1f) // is barely moving
			{
				//set the tag AffectedBy to NULL
				dynamicObjects[i]->RemoveAffectedBy();
			}
		}
		dynamicObjects[i]->AttemptResetToInitalPos(deltaTime);
	}

	for(int i = 0; i < (int)playerObjects.Size(); i++)
	{
		if(playerObjects[i])
		{		
			if(playerObjects[i]->getAffectingPlayer() != NULL)
			{
				Oyster::Math::Float vel = playerObjects[i]->GetRigidBody()->GetLinearVelocity().GetMagnitude();

				if(vel <= 0.1f) // is barely moving
				{
					//set the tag AffectedBy to NULL
					playerObjects[i]->RemoveAffectedBy();
				}
			}
		}
	}

	this->pickupSystem.Update();
}
int Level::getNrOfDynamicObj()
{
	return this->dynamicObjects.Size(); 
}
Object* Level::GetObj( int ID) const
{
	for (int i = 0; i < (int)this->dynamicObjects.Size(); i++)
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
Utility::DynamicMemory::DynamicArray<Player*>			Level::GetPlayers()
{
	return this->playerObjects;
}
Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<StaticObject>>	Level::GetStaticObjects()
{
	return this->staticObjects;
}
Utility::DynamicMemory::DynamicArray<Utility::DynamicMemory::SmartPointer<DynamicObject>>	Level::GetDynamicObject()
{
	return this->dynamicObjects;
}

