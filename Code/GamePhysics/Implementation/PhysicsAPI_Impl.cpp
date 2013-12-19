#include "PhysicsAPI_Impl.h"
#include "OysterPhysics3D.h"
#include "SimpleRigidBody.h"
#include "SphericalRigidBody.h"

using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

API_Impl API_instance;

namespace
{
	void OnPossibleCollision( Octree& worldScene, unsigned int protoTempRef, unsigned int deuterTempRef )
	{ /** @todo TODO: OnPossibleCollision is a temporary solution .*/
		auto proto = worldScene.GetCustomBody( protoTempRef );
		auto deuter = worldScene.GetCustomBody( deuterTempRef );

		Float4 worldPointOfContact;
		if( proto->Intersects(*deuter, worldPointOfContact) )
		{
			switch( proto->CallSubscription(proto, deuter) )
			{
			case ICustomBody::SubscriptMessage_ignore_collision_response:
				break;
			default:
				{ // Apply CollisionResponse in pure gather pattern
					ICustomBody::State protoState; proto->GetState( protoState );
					ICustomBody::State deuterState; deuter->GetState( deuterState );

					Float4 protoG = protoState.GetLinearMomentum( worldPointOfContact ),
						   deuterG = deuterState.GetLinearMomentum( worldPointOfContact );

					// calc from perspective of deuter
					Float4 normal; deuter->GetNormalAt( worldPointOfContact, normal );
					Float protoG_Magnitude = protoG.Dot( normal ),
						  deuterG_Magnitude = deuterG.Dot( normal );

					// bounce
					Float impulse = Formula::CollisionResponse::Bounce( deuterState.GetRestitutionCoeff(),
																		deuterState.GetMass(), deuterG_Magnitude,
																		protoState.GetMass(), protoG_Magnitude );;
					Float4 sumJ = normal*impulse;
					
					sumJ -= Formula::CollisionResponse::Friction( impulse, normal,
																  protoState.GetLinearMomentum(),  protoState.GetFrictionCoeff_Static(),  protoState.GetFrictionCoeff_Kinetic(),  protoState.GetMass(), 
																  deuterState.GetLinearMomentum(), deuterState.GetFrictionCoeff_Static(), deuterState.GetFrictionCoeff_Kinetic(), deuterState.GetMass());

					// calc from perspective of proto
					proto->GetNormalAt( worldPointOfContact, normal );
					protoG_Magnitude = protoG.Dot( normal ),
					deuterG_Magnitude = deuterG.Dot( normal );
					
					// bounce
					sumJ += normal * Formula::CollisionResponse::Bounce( protoState.GetRestitutionCoeff(),
																		 protoState.GetMass(), protoG_Magnitude,
																		 deuterState.GetMass(), deuterG_Magnitude );
					// FRICTION
					// Apply
					//sumJ += ( 1 / deuterState.GetMass() )*frictionImpulse;
					// FRICTION END

					protoState.ApplyImpulse( sumJ, worldPointOfContact, normal );
					proto->SetState( protoState );
				}
				break;					
			}
		}
	}
}

API & API::Instance()
{
	return API_instance;
}

API_Impl::API_Impl()
{
	this->gravityConstant = Constant::gravity_constant;
	this->updateFrameLength = 1.0f / 120.0f;
	this->destructionAction = Default::EventAction_Destruction;
	this->worldScene = Octree();
}

API_Impl::~API_Impl() {}

void API_Impl::Init( unsigned int numObjects, unsigned int numGravityWells , const Float3 &worldSize )
{
	unsigned char numLayers = 4; //!< @todo TODO: calc numLayers from worldSize
	this->worldScene = Octree( numObjects, numLayers, worldSize );
}

void API_Impl::SetFrameTimeLength( float deltaTime )
{
	this->updateFrameLength = deltaTime;
}

void API_Impl::SetGravityConstant( float g )
{
	this->gravityConstant = g;
}

void API_Impl::SetSubscription( API::EventAction_Destruction functionPointer )
{
	if( functionPointer )
	{
		this->destructionAction = functionPointer;
	}
	else
	{
		this->destructionAction = Default::EventAction_Destruction;
	}
}

void API_Impl::Update()
{ /** @todo TODO: Update is a temporary solution .*/
	


	::std::vector<ICustomBody*> updateList;
	auto proto = this->worldScene.Sample( Universe(), updateList ).begin();
	for( ; proto != updateList.end(); ++proto )
	{
		// Step 1: @todo TODO: Apply Gravity

		// Step 2: Apply Collision Response
		this->worldScene.Visit( *proto, OnPossibleCollision );
	}

	proto = updateList.begin();
	for( ; proto != updateList.end(); ++proto )
	{
		switch( (*proto)->Update(this->updateFrameLength) )
		{
		case UpdateState_altered:
			this->worldScene.SetAsAltered( this->worldScene.GetTemporaryReferenceOf(*proto) );
		case UpdateState_resting: default:
			break;
		}
	}
}

bool API_Impl::IsInLimbo( const ICustomBody* objRef )
{
	//! @todo TODO: implement stub
	return true;
}

void API_Impl::MoveToLimbo( const ICustomBody* objRef )
{
	/** @todo TODO: Fix this function.*/
}
void API_Impl::ReleaseFromLimbo( const ICustomBody* objRef )
{
	/** @todo TODO: Fix this function.*/
}

void API_Impl::AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle )
{
	this->worldScene.AddObject( handle );
}

UniquePointer<ICustomBody> API_Impl::ExtractObject( const ICustomBody* objRef )
{
	return this->worldScene.Extract( objRef );
}

void API_Impl::DestroyObject( const ICustomBody* objRef )
{
	UniquePointer<ICustomBody> object = this->worldScene.Extract( objRef );
	if( object )
	{
		this->destructionAction( object );
	}
}

//void API_Impl::ApplyForceAt( const ICustomBody* objRef, const Float3 &worldPos, const Float3 &worldF )
//{
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		//this->worldScene.GetCustomBody( tempRef )->Apply //!< @todo TODO: need function
//		this->worldScene.SetAsAltered( tempRef );		
//	}
//}
//
//void API_Impl::SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const Float4x4 &localI )
//{ // deprecated
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetMomentOfInertiaTensor_KeepVelocity( localI );
//	}
//}
//
//void API_Impl::SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const Float4x4 &localI )
//{ // deprecated
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetMomentOfInertiaTensor_KeepMomentum( localI );
//	}
//}
//
//void API_Impl::SetMass_KeepVelocity( const ICustomBody* objRef, Float m )
//{ // deprecated
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetMass_KeepVelocity( m );
//	}
//}
//
//void API_Impl::SetMass_KeepMomentum( const ICustomBody* objRef, Float m )
//{ // deprecated
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetMass_KeepMomentum( m );
//	}
//}
//
//void API_Impl::SetCenter( const ICustomBody* objRef, const Float3 &worldPos )
//{
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		//this->worldScene.GetCustomBody( tempRef )->Set //!< @todo TODO: need function
//		this->worldScene.EvaluatePosition( tempRef );
//	}
//}
//
//void API_Impl::SetRotation( const ICustomBody* objRef, const Float4x4 &rotation )
//{
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetRotation( rotation );
//		this->worldScene.EvaluatePosition( tempRef );
//	}
//}
//
//void API_Impl::SetOrientation( const ICustomBody* objRef, const Float4x4 &orientation )
//{
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetOrientation( orientation );
//		this->worldScene.EvaluatePosition( tempRef );
//	}
//}
//
//void API_Impl::SetSize( const ICustomBody* objRef, const Float3 &size )
//{
//	unsigned int tempRef = this->worldScene.GetTemporaryReferenceOf( objRef );
//	if( tempRef != this->worldScene.invalid_ref )
//	{
//		this->worldScene.GetCustomBody( tempRef )->SetSize( size );
//		this->worldScene.EvaluatePosition( tempRef );
//	}
//}

UniquePointer<ICustomBody> API_Impl::CreateRigidBody( const API::SimpleBodyDescription &desc ) const
{
	return new SimpleRigidBody( desc );
}

UniquePointer<ICustomBody> API_Impl::CreateRigidBody( const API::SphericalBodyDescription &desc ) const
{
	return new SphericalRigidBody( desc );
}

namespace Oyster { namespace Physics
{
	namespace Default
	{
		void EventAction_Destruction( ::Utility::DynamicMemory::UniquePointer<::Oyster::Physics::ICustomBody> proto )
		{ /* Do nothing except allowing the proto uniquePointer destroy itself. */ }

		::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_Collision( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter )
		{ /* Do nothing except returning business as usual. */
			return ::Oyster::Physics::ICustomBody::SubscriptMessage_none;
		}
	}
} }