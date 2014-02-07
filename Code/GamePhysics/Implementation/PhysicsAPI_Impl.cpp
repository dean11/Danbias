#include "PhysicsAPI_Impl.h"
#include "OysterPhysics3D.h"
#include "SimpleRigidBody.h"
#include "SphericalRigidBody.h"

using namespace ::Oyster;
using namespace ::Oyster::Physics;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;
using namespace ::Utility::DynamicMemory;
using namespace ::Utility::Value;

API_Impl API_instance;

namespace
{
	/*void OnPossibleCollision( Octree& worldScene, unsigned int protoTempRef, unsigned int deuterTempRef )
	{
		auto proto = worldScene.GetCustomBody( protoTempRef );
		auto deuter = worldScene.GetCustomBody( deuterTempRef );

		Float4 worldPointOfContact;
		if( proto->Intersects(*deuter, worldPointOfContact) )
		{
			// Apply CollisionResponse in pure gather pattern
			ICustomBody::State protoState; proto->GetState( protoState );
			ICustomBody::State deuterState; deuter->GetState( deuterState );

			// calc from perspective of deuter.
			Float4 normal = (worldPointOfContact - Float4(deuterState.GetCenterPosition(), 1.0f )); // Init value is only borrowed
			//if( normal.Dot(normal) > 0.0f )
			{
				deuter->GetNormalAt( worldPointOfContact, normal );
			}
			//else
			//{ // special case: deuter is completly contained within proto or they have overlapping centers.

			//	normal = Float4( protoState.GetCenterPosition() - deuterState.GetCenterPosition(), 0.0f );
			//	if( normal.Dot(normal) == 0.0f )
			//	{ // they have overlapping centers. Rebound at least
			//		// calculate and store time interpolation value, for later rebound.
			//		proto->SetTimeOfContact( worldPointOfContact );
			//		return;
			//	}
			//	
			//	// borrowing the negated normal of proto.
			//	proto->GetNormalAt( worldPointOfContact, normal );
			//	normal = -normal;
			//}
			normal.Normalize();

			Float4 protoG  = Float4(protoState.GetLinearMomentum( worldPointOfContact.xyz ), 0),
				   deuterG = Float4(deuterState.GetLinearMomentum( worldPointOfContact.xyz ), 0);

			if( normal != normal ) // debug: trap
				const char *breakpoint = "This should never happen";

			if( protoG != protoG ) // debug: trap
				const char *breakpoint = "This should never happen";

			if( deuterG != deuterG ) // debug: trap
				const char *breakpoint = "This should never happen";

			Float protoG_Magnitude = protoG.Dot( normal ),
				  deuterG_Magnitude = deuterG.Dot( normal );

			// if they are not relatively moving towards eachother, there is no collision
			Float deltaPos = normal.Dot( Float4(deuterState.GetCenterPosition(), 1) - Float4(protoState.GetCenterPosition(), 1) );
			if( deltaPos < 0.0f )
			{
				if( protoG_Magnitude >= deuterG_Magnitude )
				{
					return;
				}
			}
			else if( deltaPos > 0.0f )
			{
				if( protoG_Magnitude <= deuterG_Magnitude )
				{
					return;
				}
			}
			else
			{
				return;
			}

			if( proto->CallSubscription_BeforeCollisionResponse(proto) == ICustomBody::SubscriptMessage_ignore_collision_response )
			{
				return;
			}

			// PLayerHAck
			if( proto->CallSubscription_BeforeCollisionResponse(proto) == ICustomBody::SubscriptMessage_player_collision_response )
			{
				//Float3 linearMomentum = protoState.GetLinearMomentum();
				//Float3 up = -protoState.GetGravityNormal();
				//Float3 upForce = (linearMomentum.Dot(up) * up);

				//Float3 noBounceForce = linearMomentum - upForce;
				//protoState.SetLinearMomentum(noBounceForce);
				//proto->SetState(protoState);
				return;
			}
			// calculate and store time interpolation value, for later rebound.
			proto->SetTimeOfContact( worldPointOfContact );

			// bounce
			Float4 bounceD = normal * -Formula::CollisionResponse::Bounce( deuterState.GetRestitutionCoeff(),
																		   deuterState.GetMass(), deuterG_Magnitude,
																		   protoState.GetMass(), protoG_Magnitude );
					

			// calc from perspective of proto

			normal = (worldPointOfContact - Float4(protoState.GetCenterPosition(), 1.0f )).GetNormalized();
			//if( normal.Dot(normal) > 0.0f )
			{
				proto->GetNormalAt( worldPointOfContact, normal );
				protoG_Magnitude = protoG.Dot( normal );
				deuterG_Magnitude = deuterG.Dot( normal );
				normal.Normalize();
			}
			//else
			//{ // special case: proto is completly contained within deuter.
			//	// borrowing the negated normal of deuter.
			//	deuter->GetNormalAt( worldPointOfContact, normal );
			//	normal = -normal;
			//	protoG_Magnitude = -protoG_Magnitude;
			//	deuterG_Magnitude = -deuterG_Magnitude;
			//}
			
			if( normal != normal ) // debug: trap
				const char *breakpoint = "This should never happen";
			
			// bounce
			Float4 bounceP = normal * Formula::CollisionResponse::Bounce( protoState.GetRestitutionCoeff(),
																		  protoState.GetMass(), protoG_Magnitude,
																		  deuterState.GetMass(), deuterG_Magnitude );

			Float4 bounce = Average( bounceD, bounceP );

			Float4 friction = Formula::CollisionResponse::Friction( protoG_Magnitude, normal,
																	Float4(protoState.GetLinearMomentum(), 0),  protoState.GetFrictionCoeff_Static(),  protoState.GetFrictionCoeff_Kinetic(),  protoState.GetMass(), 
																	Float4(deuterState.GetLinearMomentum(), 0), deuterState.GetFrictionCoeff_Static(), deuterState.GetFrictionCoeff_Kinetic(), deuterState.GetMass());
			
			if(protoState.GetMass() == 70)
			{
				const char* breakPoint = "here";
			}

			Float kineticEnergyPBefore = Oyster::Physics3D::Formula::LinearKineticEnergy( protoState.GetMass(), protoState.GetLinearMomentum()/protoState.GetMass() );

			protoState.ApplyImpulse( bounce.xyz, worldPointOfContact.xyz, normal.xyz );
			proto->SetState( protoState );

			Float kineticEnergyPAFter = Oyster::Physics3D::Formula::LinearKineticEnergy( protoState.GetMass(), (protoState.GetLinearMomentum() + protoState.GetLinearImpulse())/protoState.GetMass() );

			proto->CallSubscription_AfterCollisionResponse( deuter,  kineticEnergyPBefore - kineticEnergyPAFter );
		}


	}*/
	void OnPossibleCollision( Octree& worldScene, unsigned int protoTempRef, unsigned int deuterTempRef )
	{
		auto proto = worldScene.GetCustomBody( protoTempRef );
		auto deuter = worldScene.GetCustomBody( deuterTempRef );

		if(proto->GetState().GetMass() == 70)
		{
			const char *breakpoint = "STOP";
		}

		Float4 worldPointOfContact;
		if( proto->Intersects(*deuter, worldPointOfContact) )
		{
			// Apply CollisionResponse in pure gather pattern
			ICustomBody::State protoState; proto->GetState( protoState );
			ICustomBody::State deuterState; deuter->GetState( deuterState );

			Float4 normal = (worldPointOfContact - Float4(deuterState.GetCenterPosition(), 1.0f )); // Init value is only borrowed
			if( normal.Dot(normal) > 0.0f )
			{
				deuter->GetNormalAt( worldPointOfContact, normal );
			}
			else
			{ // special case: deuter is completly contained within proto or they have overlapping centers.

				normal = Float4( protoState.GetCenterPosition() - deuterState.GetCenterPosition(), 0.0f );
				if( normal.Dot(normal) == 0.0f )
				{ // they have overlapping centers. Rebound at least
					// calculate and store time interpolation value, for later rebound.
					proto->SetTimeOfContact( worldPointOfContact );
					return;
				}
				
				// borrowing the negated normal of proto.
				proto->GetNormalAt( worldPointOfContact, normal );
				normal = -normal;
			}
			normal.Normalize();

			Float4 protoG  = Float4(protoState.GetLinearMomentum( worldPointOfContact.xyz ), 0),
				   deuterG = Float4(deuterState.GetLinearMomentum( worldPointOfContact.xyz ), 0);

			Float protoG_Magnitude = protoG.Dot( normal ),
				  deuterG_Magnitude = deuterG.Dot( normal );

			if(protoState.GetMass() == 20)
			{
				const char *breakpoint = "STOP";
			}

			// if they are not relatively moving towards eachother, there is no collision
			Float deltaPos = normal.Dot( Float4(deuterState.GetCenterPosition(), 1) - Float4(protoState.GetCenterPosition(), 1) );
			if( deltaPos < 0.0f )
			{
				if( protoG_Magnitude >= deuterG_Magnitude )
				{
					return;
				}
			}
			else if( deltaPos > 0.0f )
			{
				if( protoG_Magnitude <= deuterG_Magnitude )
				{
					return;
				}
			}
			else
			{
				return;
			}

			// bounce
			Float4 bounceD = normal * -Formula::CollisionResponse::Bounce( deuterState.GetRestitutionCoeff(),
																		   deuterState.GetMass(), deuterG_Magnitude,
																		   protoState.GetMass(), protoG_Magnitude );

			normal = (worldPointOfContact - Float4(protoState.GetCenterPosition(), 1.0f )).GetNormalized();
			if( normal.Dot(normal) > 0.0f )
			{
				proto->GetNormalAt( worldPointOfContact, normal );
				protoG_Magnitude = protoG.Dot( normal );
				deuterG_Magnitude = deuterG.Dot( normal );
				normal.Normalize();
			}
			else
			{ // special case: proto is completly contained within deuter.
				// borrowing the negated normal of deuter.
				deuter->GetNormalAt( worldPointOfContact, normal );
				normal = -normal;
				protoG_Magnitude = -protoG_Magnitude;
				deuterG_Magnitude = -deuterG_Magnitude;
			}
			normal.Normalize();
			
			if( normal != normal ) // debug: trap
				const char *breakpoint = "This should never happen";

			Float4 friction = Formula::CollisionResponse::Friction( protoG_Magnitude, normal,
																	Float4(protoState.GetLinearMomentum(), 0),  protoState.GetFrictionCoeff_Static(),  protoState.GetFrictionCoeff_Kinetic(),  protoState.GetMass(), 
																	Float4(deuterState.GetLinearMomentum(), 0), deuterState.GetFrictionCoeff_Static(), deuterState.GetFrictionCoeff_Kinetic(), deuterState.GetMass());



			protoState.ApplyFriction( -friction.xyz );

			if( proto->CallSubscription_BeforeCollisionResponse(proto) == ICustomBody::SubscriptMessage_ignore_collision_response )
			{
				return;
			}

			// PLayerHAck
			if( proto->CallSubscription_BeforeCollisionResponse(proto) == ICustomBody::SubscriptMessage_player_collision_response )
			{
				return;
			}


			
			
			// bounce
			Float4 bounceP = normal * Formula::CollisionResponse::Bounce( protoState.GetRestitutionCoeff(),
																		  protoState.GetMass(), protoG_Magnitude,
																		  deuterState.GetMass(), deuterG_Magnitude );

			Float4 bounce = bounceP;

			//LinearAlgebra3D::InterpolateAxisYToNormal_UsingNlerp(state.SetOrientation(, Float4(state.GetGravityNormal(), 0.0f), 1.0f);


			if( abs(bounce.x) < 0.001 )
			{
				bounce.x = 0;
			}
			if( abs(bounce.y) < 0.001 )
			{
				bounce.y = 0;
			}
			if( abs(bounce.z) < 0.001 )
			{
				bounce.z = 0;
			}

			Float kineticEnergyPBefore = Oyster::Physics3D::Formula::LinearKineticEnergy( protoState.GetMass(), protoState.GetLinearMomentum()/protoState.GetMass() );

			protoState.ApplyImpulse( bounce.xyz, worldPointOfContact.xyz, normal.xyz );
			proto->SetState( protoState );

			Float kineticEnergyPAFter = Oyster::Physics3D::Formula::LinearKineticEnergy( protoState.GetMass(), (protoState.GetLinearMomentum() + protoState.GetLinearImpulse())/protoState.GetMass() );

			proto->CallSubscription_AfterCollisionResponse( deuter,  kineticEnergyPBefore - kineticEnergyPAFter );
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
	this->epsilon = Constant::epsilon;
	this->updateFrameLength = 1.0f / 120.0f;
	this->destructionAction = Default::EventAction_Destruction;
	this->gravity = ::std::vector<Gravity>();
	this->worldScene = Octree();
}

API_Impl::~API_Impl() {}

void API_Impl::Init( unsigned int numObjects, unsigned int numGravityWells , const Float3 &worldSize )
{
	unsigned char numLayers = 4; //!< @todo TODO: calc numLayers from worldSize
	this->gravity.resize( 0 );
	this->gravity.reserve( numGravityWells );
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

void API_Impl::SetEpsilon( float e )
{
	this->epsilon = e;
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

float API_Impl::GetFrameTimeLength() const
{
	return this->updateFrameLength;
}

void API_Impl::Update()
{ /** @todo TODO: Update is a temporary solution .*/
	::std::vector<ICustomBody*> updateList;
	this->worldScene.Sample( Universe(), updateList );
	ICustomBody::State state;
	for( int i = 0; i < updateList.size(); i++ )
	{
		auto proto = updateList[i];
		// Step 1: Apply Gravity
		Float4 gravityImpulse = Float4::null;
		proto->GetState( state );
		for( int j = 0; j < this->gravity.size(); ++j )
		{
			switch( this->gravity[j].gravityType )
			{
			case Gravity::GravityType_Well:
				{
					Float4 d = Float4( this->gravity[j].well.position, 1.0f ) - Float4( state.GetCenterPosition(), 1.0f );
					Float rSquared = d.Dot( d );
					if( rSquared != 0.0 )
					{
						if(state.GetMass() == 70)
						{
							const char *breakpoint = "STOP";
						}
						Float force = 9.82*10;
						gravityImpulse += ((this->updateFrameLength * force)) * d.GetNormalized();
					}
					break;
				}
			case Gravity::GravityType_Directed:
				gravityImpulse += Float4( this->gravity[j].directed.impulse, 0.0f );
				break;
//			case Gravity::GravityType_DirectedField:
//				//this->gravity[i].directedField.
//				//! TODO: @todo rethink
//				break;
			default: break;
			}
		}

		if( gravityImpulse != gravityImpulse ) // debug: trap
				const char *breakpoint = "This should never happen";
		Float posLength = state.GetCenterPosition().GetLength();
		if( gravityImpulse != Float4::null && posLength - 300 > state.GetReach().y )
		{
			state.ApplyLinearImpulse( gravityImpulse.xyz );
			state.SetGravityNormal( gravityImpulse.GetNormalized().xyz );
			proto->SetState( state );
		}
		if(state.GetMass() == 70)
		{
			const char *breakpoint = "STOP";
		}
		// Step 2: Apply Collision Response
		this->worldScene.Visit( proto, OnPossibleCollision );
	}

	
	for( int i = 0; i < updateList.size(); i++ )
	{
		auto proto = updateList[i];
		proto->GetState( state );
		Float3 lM = state.GetLinearMomentum();

		//LinearAlgebra3D::InterpolateAxisYToNormal_UsingNlerp(state.SetOrientation(, Float4(state.GetGravityNormal(), 0.0f), 1.0f);


		if( abs(lM.x) < this->epsilon )
		{
			state.linearMomentum.x = 0;
		}
		if( abs(lM.y) < this->epsilon )
		{
			state.linearMomentum.y = 0;
		}
		if( abs(lM.z) < this->epsilon )
		{
			state.linearMomentum.z = 0;
		}

		proto->SetState( state );

		switch( proto->Update(this->updateFrameLength) )
		{
		case UpdateState_altered:
			this->worldScene.SetAsAltered( this->worldScene.GetTemporaryReferenceOf(proto) );
			proto->CallSubscription_Move();
		case UpdateState_resting: 
		default:
			break;
		}
	}
}

bool API_Impl::IsInLimbo( const ICustomBody* objRef )
{
	return this->worldScene.IsInLimbo( objRef );
}

void API_Impl::MoveToLimbo( const ICustomBody* objRef )
{
	this->worldScene.MoveToLimbo( objRef );
}
void API_Impl::ReleaseFromLimbo( const ICustomBody* objRef )
{
	this->worldScene.ReleaseFromLimbo( objRef );
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

void API_Impl::AddGravity( const API::Gravity &g )
{
	this->gravity.push_back( g );
}

void API_Impl::RemoveGravity( const API::Gravity &g )
{
	for( ::std::vector<Gravity>::size_type i = this->gravity.size() - 1; i >= 0; --i )
	{
		if( g == this->gravity[i] )
		{
			int end = this->gravity.size() - 1;
			this->gravity[i] = this->gravity[end];
			this->gravity.resize( end );
		}
	}
}

void API_Impl::ApplyEffect( const Oyster::Collision3D::ICollideable& collideable, void* args, void(hitAction)(ICustomBody*, void*) )
{
	this->worldScene.Visit(collideable, args, hitAction);
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

		::Oyster::Physics::ICustomBody::SubscriptMessage EventAction_BeforeCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter )
		{ /* Do nothing except returning business as usual. */
			return ::Oyster::Physics::ICustomBody::SubscriptMessage_none;
		}

		void EventAction_AfterCollisionResponse( const ::Oyster::Physics::ICustomBody *proto, const ::Oyster::Physics::ICustomBody *deuter, ::Oyster::Math::Float kineticEnergyLoss )
		{ /* Do nothing except returning business as usual. */
			
		}

		void EventAction_Move( const ::Oyster::Physics::ICustomBody *object )
		{ /* Do nothing. */ }
	}
} }