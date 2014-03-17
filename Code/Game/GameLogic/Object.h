//////////////////////////////////////////////////
//Created by Erik and Linda of the GameLogic team
//////////////////////////////////////////////////


#ifndef OBJECT_H
#define OBJECT_H

#include "GameLogicStates.h"
#include "GameAPI.h"
#include <PhysicsAPI.h>

namespace GameLogic
{
	class Game;

	class Object	:public IObjectData
	{
	public:
		typedef Oyster::Physics::ICustomBody::SubscriptMessage (*OnCollisionCallback)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss);

	public:		
		Object();
		Object(Oyster::Physics::ICustomBody *rigidBody, void (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		Object(Oyster::Physics::ICustomBody *rigidBody, Oyster::Physics::ICustomBody::SubscriptMessage (*collisionFuncAfter)(Oyster::Physics::ICustomBody *proto,Oyster::Physics::ICustomBody *deuter,Oyster::Math::Float kineticEnergyLoss), ObjectSpecialType type, int objectID);
		~Object(void);

		inline ObjectSpecialType				GetObjectType() const	override	{ return this->type; }
		inline int								GetID()			const	override	{ return this->objectID; }	
		inline Oyster::Math::Float3				GetScale()				override;	
		inline Oyster::Math::Float3				GetPosition()			override;
		inline Oyster::Math::Quaternion			GetRotation()			override;
		inline Oyster::Math::Float4x4			GetOrientation()		override;
		inline Oyster::Physics::ICustomBody*	GetRigidBody()						{ return this->rigidBody; }
		inline Oyster::Math::Float				GetExtraDamageOnCollision()			{ return this->extraDamageOnCollision; }
		inline Oyster::Math::Float3				GetInitialPos()						{return this->initialPos;}
		inline void								SetInitialPos(Oyster::Math::Float3 initialPos)	{this->initialPos = initialPos;}

		virtual void BeginFrame()	{ };
		virtual void EndFrame()		{ };
		bool IsDisabled() const;
		void SetOnCollision(OnCollisionCallback func);

		static Oyster::Physics::ICustomBody::SubscriptMessage DefaultOnCollision(Oyster::Physics::ICustomBody *rigidBodyObject, Oyster::Physics::ICustomBody *obj, Oyster::Math::Float kineticEnergyLoss);

	protected:
		Oyster::Physics::ICustomBody *rigidBody;

		static const Game* gameInstance;

		//Oyster::Math::Float3 scale;				//The scale of both rigid body and the mesh

		ObjectSpecialType type;
		int objectID;

		Oyster::Math::Float extraDamageOnCollision;

		Oyster::Math::Float3 initialPos;
		Oyster::Math::Float resetCounter;

	};

}

#endif 