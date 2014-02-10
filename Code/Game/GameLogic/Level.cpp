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
}

void Level::InitiateLevel(std::string levelPath)
{

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
	ICustomBody* rigidBody = API::Instance().AddCollisionSphere(599.2f, Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 0, 0), 0);
	
	ICustomBody::State state;
	rigidBody->GetState(state);
	state.restitutionCoeff = 0.2f;
	rigidBody->SetState(state);
	levelObj = new StaticObject(rigidBody, LevelCollisionBefore, LevelCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	this->levelObj->objectID = idCount++;
	rigidBody->SetCustomTag(levelObj);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	int offset = 0;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		rigidBody_TestBox = API::Instance().AddCollisionBox(Oyster::Math::Float3(0.5f, 0.5f, 0.5f), Oyster::Math::Float4(0, 0, 0, 1), Oyster::Math::Float3(0, 605 + i*5, -10), 5);

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
