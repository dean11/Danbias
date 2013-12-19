#ifndef PHYSICS_API_H
#define PHYSICS_API_H

#include "OysterCollision3D.h"
#include "OysterMath.h"

#if defined PHYSICS_DLL_EXPORT
	#define PHYSICS_DLL_USAGE __declspec(dllexport)
#else
	#define PHYSICS_DLL_USAGE __declspec(dllimport)
#endif

namespace Oyster
{
	namespace Physics
	{
		class API;
		class ICustomBody;

		namespace Struct
		{
			struct SimpleBodyDescription;
			struct SphericalBodyDescription;
			struct CustomBodyState;
		}

		enum UpdateState
		{
			UpdateState_resting,
			UpdateState_altered
		};

		namespace Constant
		{
			const float gravity_constant = (const float)6.67284e-11; //!< The _big_G_! ( N(m/kg)^2 ) Used in real gravityforcefields.
		}

		class PHYSICS_DLL_USAGE API
		{
		public:
			typedef Struct::SimpleBodyDescription SimpleBodyDescription;
			typedef Struct::SphericalBodyDescription SphericalBodyDescription;

			typedef void (*EventAction_Destruction)( ::Utility::DynamicMemory::UniquePointer<ICustomBody> proto );

			/** Gets the Physics instance. */
			static API & Instance();

			/********************************************************
			 * Clears all content and reset Engine assetts such as buffers.
			 * @param numObjects: The predicted max number of active objects.
			 * @param numGravityWells: The predicted max number of active gravity wells.
			 * @param worldSize: The size of acceptable physics space.
			 ********************************************************/
			virtual void Init( unsigned int numObjects, unsigned int numGravityWells , const ::Oyster::Math::Float3 &worldSize ) = 0;

			/********************************************************
			 * Sets the time length of each physics update frame.
			 ********************************************************/
			virtual void SetFrameTimeLength( float seconds ) = 0;
			
			/********************************************************
			 * Sets the Gravityconstant in the physics that will be
			 * used in ForceField calculations.
			 * @param g: Default is the real world Constant::gravity_constant [N(m/kg)^2]
			 ********************************************************/
			virtual void SetGravityConstant( float g ) = 0;
			
			/********************************************************
			 * Sets the function that will be called by the engine
			 * whenever an object is being destroyed for some reason.
			 * - Because DestroyObject(...) were called.
			 * - Out of memory forced engine to destroy an object.
			 * @param functionPointer: If NULL, an empty default function will be set.
			 ********************************************************/
			virtual void SetSubscription( EventAction_Destruction functionPointer ) = 0;

			/********************************************************
			 * Triggers the engine to run next update frame.
			 * All accumulated forces and changes will be consumed.
			 * EventAction functions might be called.
			 ********************************************************/
			virtual void Update() = 0;

			/********************************************************
			 * An object in limbo state will be ignored during the physics frame Update.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @return true if object is in limbo state.
			 ********************************************************/
			virtual bool IsInLimbo( const ICustomBody* objRef ) = 0;
			
			/********************************************************
			 * An object in limbo state will be ignored during the physics frame Update.
			 * This will put an object in Limbo state.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 ********************************************************/
			virtual void MoveToLimbo( const ICustomBody* objRef ) = 0;
			
			/********************************************************
			 * An object in limbo state will be ignored during the physics frame Update.
			 * This will clear the accumulated force/torque and remove the Limbo state.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 ********************************************************/
			virtual void ReleaseFromLimbo( const ICustomBody* objRef ) = 0;

			/********************************************************
			 * Introduces a new object into the engine.
			 * @param handle: A pointer along with the responsibility to delete.
			 ********************************************************/
			virtual void AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle ) = 0;

			/********************************************************
			 * Fetches and removes an object from the engine.
			 * Will not call the provided EventAction_Destruction method.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @return A pointer along with the responsibility to delete. NULL if faulty objRef.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( const ICustomBody* objRef ) = 0;

			/********************************************************
			 * Removes an object from the engine.
			 * Will call the provided EventAction_Destruction method. Not if objRef is faulty.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 ********************************************************/
			virtual void DestroyObject( const ICustomBody* objRef ) = 0;

			///********************************************************
			// * Apply force on an object.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			// * @param worldF: Vector with the direction and magnitude of the force. [N]
			// ********************************************************/
			//virtual void ApplyForceAt( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF ) = 0;

			///********************************************************
			// * Sets the MomentOfInertia tensor matrix of an object without changing it's angular velocity.
			// * Noticeable effect: The angular momentum will change. Changing the amount of kinetic energy.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			// ********************************************************/
			//virtual void SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			//
			///********************************************************
			// * Sets the MomentOfInertia tensor matrix of an object without changing it's angular momentum.
			// * Noticeable effect: The angular velocity will change. Can be used to create slow effects.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			// ********************************************************/
			//virtual void SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			//
			///********************************************************
			// * Sets the mass of an object without changing it's linear velocity.
			// * Noticeable effect: The linear momentum will change. Changing the amount of kinetic energy.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param m: [kg]
			// ********************************************************/
			//virtual void SetMass_KeepVelocity( const ICustomBody* objRef, ::Oyster::Math::Float m ) = 0;
			//
			///********************************************************
			// * Sets the mass of an object without changing it's linear velocity.
			// * Noticeable effect: The linear velocity will change. Can be used to create slow effects.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param m: [kg]
			// ********************************************************/
			//virtual void SetMass_KeepMomentum( const ICustomBody* objRef, ::Oyster::Math::Float m ) = 0;
			//
			///********************************************************
			// * Instantly moves an object.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			// ********************************************************/
			//virtual void SetCenter( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos ) = 0;
			//
			///********************************************************
			// * Instantly redirects object.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param rotation: New rotation.
			// ********************************************************/
			//virtual void SetRotation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &rotation ) = 0;
			//
			///********************************************************
			// * Instantly moves and redirects object.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param orientation: New orientation.
			// ********************************************************/
			//virtual void SetOrientation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &orientation ) = 0;
			//
			///********************************************************
			// * Resizes the boundingBox.
			// * @param objRef: A pointer to the ICustomBody representing a physical object.
			// * @param size: New size of this [m]
			// ********************************************************/
			//virtual void SetSize( const ICustomBody* objRef, const ::Oyster::Math::Float3 &size ) = 0;

			/********************************************************
			 * Creates a new dynamically allocated object that can be used as a component for more complex ICustomBodies.
			 * @param desc: @see API::SimpleBodyDescription
			 * @return A pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SimpleBodyDescription &desc ) const = 0;

			/********************************************************
			 * Creates a new dynamically allocated object that can be used as a component for more complex ICustomBodies.
			 * @param desc: @see API::SphericalBodyDescription
			 * @return A pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateRigidBody( const SphericalBodyDescription &desc ) const = 0;

		protected:
			virtual ~API() {}
		};
	
		//! The root interface for all physical representations processable by the engine.
		class PHYSICS_DLL_USAGE ICustomBody
		{
		public:
			enum SubscriptMessage
			{
				SubscriptMessage_none,
				SubscriptMessage_ignore_collision_response
			};

			typedef SubscriptMessage (*EventAction_Collision)( const ICustomBody *proto, const ICustomBody *deuter );
			typedef Struct::SimpleBodyDescription SimpleBodyDescription;
			typedef Struct::SphericalBodyDescription SphericalBodyDescription;
			typedef Struct::CustomBodyState State;

			virtual ~ICustomBody() {};

			/********************************************************
			 * Creates a complete copy of the current (type)object.
			 * @return An ICustomBody pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const = 0;

			/********************************************************
			 * @todo TODO: need doc
			 ********************************************************/
			virtual SubscriptMessage CallSubscription( const ICustomBody *proto, const ICustomBody *deuter ) = 0;

			/********************************************************
			 * @todo TODO: need doc
			 ********************************************************/
			virtual State GetState() const = 0;

			/********************************************************
			 * @todo TODO: need doc
			 ********************************************************/
			virtual State & GetState( State &targetMem ) const = 0;

			/********************************************************
			 * @return the linear velocity of the rigid body in a vector.
			 ********************************************************/
			virtual Math::Float3 GetRigidLinearVelocity() const = 0;

			/********************************************************
			 * @todo TODO: need doc
			 ********************************************************/
			virtual void SetState( const State &state ) = 0;

			/********************************************************
			 * @return true if Engine should apply gravity on this object.
			 ********************************************************/
			virtual bool IsAffectedByGravity() const = 0;

			/********************************************************
			 * param shape: Any defined sample shape.
			 * @return true if this truly intersects with shape.
			 ********************************************************/
			virtual bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const = 0;

			/********************************************************
			 * Performs a detailed Intersect test and returns if, when and where.
			 * @param shape: Any defined sample shape.
			 * @param worldPointOfContact: Where at timeOfContact, this and object touches eachother.
			 * @return true if this truly intersects with object.
			 ********************************************************/
			virtual bool Intersects( const ::Oyster::Collision3D::ICollideable &shape, ::Oyster::Math::Float4 &worldPointOfContact ) const = 0;

			/********************************************************
			 * Performs a detailed Intersect test and returns if, when and where.
			 * @param object: What this is intersect testing against.
			 * @param worldPointOfContact: Where at timeOfContact, this and object touches eachother.
			 * @return true if this truly intersects with object.
			 ********************************************************/
			virtual bool Intersects( const ICustomBody &object, ::Oyster::Math::Float4 &worldPointOfContact ) const = 0;

			/********************************************************
			 * Required by Engine's Collision Search.
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a sphere shape that contains the ICustomBody.
			 ********************************************************/
			virtual ::Oyster::Collision3D::Sphere & GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const = 0;
			
			/********************************************************
			 * Required by Engine's Collision Responsing.
			 * @param worldPos: Should be worldPointOfContact from Intersects( ... )
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a surface normal in worldSpace.
			 ********************************************************/
			virtual ::Oyster::Math::Float4 & GetNormalAt( const ::Oyster::Math::Float4 &worldPos, ::Oyster::Math::Float4 &targetMem = ::Oyster::Math::Float4() ) const = 0;
			
			/********************************************************
			 * The gravity normal will have same direction as the total gravity force pulling on this and have the magnitude of 1.0f.
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a normalized vector in worldSpace. Exception: Null vector if no gravity been applied.
			 ********************************************************/
			virtual ::Oyster::Math::Float3 & GetGravityNormal( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;

			///********************************************************
			// * The world position of this center of gravity.
			// * @param targetMem: Provided memory that written into and then returned.
			// * @return a position in worldSpace.
			// ********************************************************/
			//virtual ::Oyster::Math::Float3 & GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			//
			///********************************************************
			// * @param targetMem: Provided memory that written into and then returned.
			// * @return a copy of this's rotation matrix.
			// ********************************************************/
			//virtual ::Oyster::Math::Float4x4 & GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			//
			///********************************************************
			// * @param targetMem: Provided memory that written into and then returned.
			// * @return a copy of this's orientation matrix.
			// ********************************************************/
			//virtual ::Oyster::Math::Float4x4 & GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			//
			///********************************************************
			// * @param targetMem: Provided memory that written into and then returned.
			// * @return a copy of this's view matrix.
			// ********************************************************/
			//virtual ::Oyster::Math::Float4x4 & GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;

			/********************************************************
			 * To not be called if is in Engine
			 * Is called during API::Update
			 ********************************************************/
			virtual UpdateState Update( ::Oyster::Math::Float timeStepLength ) = 0;
			
			/********************************************************
			 * Sets which scene this ICustomBody is within.
			 * Reserved to only be used by the scene.
			 * @todo TODO: create an IScene interface
			 ********************************************************/
			virtual void SetScene( void *scene ) = 0;

			/********************************************************
			 * Sets the function that will be called by the engine
			 * whenever a collision occurs.
			 * @param functionPointer: If NULL, an empty default function will be set.
			 ********************************************************/
			virtual void SetSubscription( EventAction_Collision functionPointer ) = 0;

			/********************************************************
			 * @param ignore: True if Engine should not apply Gravity.
			 ********************************************************/
			virtual void SetGravity( bool ignore) = 0;

			/********************************************************
			 * Used by Engine
			 * @param normalizedVector: Should have same direction as the pullinggravity.
			 ********************************************************/
			virtual void SetGravityNormal( const ::Oyster::Math::Float3 &normalizedVector ) = 0;

			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetMomentOfInertiaTensor_KeepVelocity(...) instead
			// ********************************************************/
			//virtual void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetMomentOfInertiaTensor_KeepMomentum(...)
			// ********************************************************/
			//virtual void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetMass_KeepVelocity(...)
			// ********************************************************/
			//virtual void SetMass_KeepVelocity( ::Oyster::Math::Float m ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetMass_KeepMomentum(...)
			// ********************************************************/
			//virtual void SetMass_KeepMomentum( ::Oyster::Math::Float m ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetCenter(...)
			// ********************************************************/
			//virtual void SetCenter( const ::Oyster::Math::Float3 &worldPos ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetRotation(...)
			// ********************************************************/
			//virtual void SetRotation( const ::Oyster::Math::Float4x4 &rotation ) = 0;
			//
			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetOrientation(...)
			// ********************************************************/
			//virtual void SetOrientation( const ::Oyster::Math::Float4x4 &orientation ) = 0;

			///********************************************************
			// * To not be called if is in Engine
			// * Use API::SetSize(...)
			// ********************************************************/
			//virtual void SetSize( const ::Oyster::Math::Float3 &size ) = 0;

			///********************************************************
			// * To not be called if is in Engine
			// * Use API::?? @todo TODO: 
			// ********************************************************/
			//virtual void SetMomentum( const ::Oyster::Math::Float3 &worldG ) = 0;
		};
	}
}

#include "PhysicsStructs.h"
#include "PhysicsFormula.h"

#endif