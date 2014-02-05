#include "Level.h"
#include "CollisionManager.h"

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

	// add level sphere
	API::SphericalBodyDescription sbDesc;
	sbDesc.centerPosition = Oyster::Math::Float4(0,0,0,1);
	sbDesc.ignoreGravity = true;
	sbDesc.radius = 300; 
	sbDesc.mass = 10e12f;
	sbDesc.frictionCoeff_Static = 0;
	sbDesc.frictionCoeff_Dynamic = 0;
	//sbDesc.rotation = 
	ICustomBody* rigidBody = API::Instance().CreateRigidBody(sbDesc).Release();
	
	ICustomBody::State state;
	rigidBody->GetState(state);
	state.SetRestitutionCoeff(0.2);
	rigidBody->SetState(state);
	
	levelObj = new StaticObject(rigidBody, LevelCollisionBefore, LevelCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_WORLD);
	rigidBody->SetCustomTag(levelObj);
	
	//this->dynamicObjects = new DynamicArray< DynamicObject>;
	// add box
	API::SimpleBodyDescription sbDesc_TestBox;
	sbDesc_TestBox.centerPosition = Oyster::Math::Float4(10,320,0,0);
	sbDesc_TestBox.ignoreGravity = false;

	sbDesc_TestBox.mass = 50;
	sbDesc_TestBox.size = Oyster::Math::Float4(2,2,2,0);

	
	ICustomBody* rigidBody_TestBox;

	int nrOfBoxex = 5;
	for(int i =0; i< nrOfBoxex; i ++)
	{
		sbDesc_TestBox.centerPosition = Oyster::Math::Float4(10 + ( i*5) ,320,0,0);
		rigidBody_TestBox = API::Instance().CreateRigidBody(sbDesc_TestBox).Release();
		rigidBody_TestBox->SetSubscription(Level::PhysicsOnMoveLevel);

		this->dynamicObjects.Push(new DynamicObject(rigidBody_TestBox,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
		rigidBody_TestBox->SetCustomTag(this->dynamicObjects[i]);
	}
	




	// add crystal
	API::SimpleBodyDescription sbDesc_Crystal;
	sbDesc_Crystal.centerPosition = Oyster::Math::Float4(10, 305, 0, 0);
	sbDesc_Crystal.ignoreGravity = false;
	sbDesc_Crystal.mass = 70;
	sbDesc_Crystal.size = Oyster::Math::Float4(2,3,2,0);

	ICustomBody* rigidBody_Crystal = API::Instance().CreateRigidBody(sbDesc_Crystal).Release();
	rigidBody_Crystal->SetSubscription(Level::PhysicsOnMoveLevel);
	this->dynamicObjects.Push(new DynamicObject(rigidBody_Crystal,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_BOX));
	rigidBody_Crystal->SetCustomTag(this->dynamicObjects[nrOfBoxex]);


	// add house
	API::SimpleBodyDescription sbDesc_House;
	sbDesc_House.centerPosition = Oyster::Math::Float4(50, 300, 0, 0);
	sbDesc_House.ignoreGravity = false;

	sbDesc_House.mass = 70;
	sbDesc_House.size = Oyster::Math::Float4(2,3,2,0);


	ICustomBody* rigidBody_House = API::Instance().CreateRigidBody(sbDesc_House).Release();
	rigidBody_House->SetSubscription(Level::PhysicsOnMoveLevel);
	this->staticObjects.Push(new StaticObject(rigidBody_House,Object::DefaultCollisionBefore, Object::DefaultCollisionAfter, OBJECT_TYPE::OBJECT_TYPE_GENERIC));
	rigidBody_House->SetCustomTag(this->staticObjects[0]);

	
	// add gravitation 
	API::Gravity gravityWell;
	gravityWell.gravityType = API::Gravity::GravityType_Well;
	gravityWell.well.mass = 1e17f;
	gravityWell.well.position = Oyster::Math::Float4(0,0,0,1);
	API::Instance().AddGravity(gravityWell);
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
}
