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

		enum UpdateState
		{
			resting,
			altered
		};

		namespace Constant
		{
			const float gravity_constant = (const float)6.67284e-11; //!< The _big_G_! ( N(m/kg)^2 ) Used in real gravityforcefields.
		}

		class PHYSICS_DLL_USAGE MomentOfInertia
		{
		public:
			static ::Oyster::Math::Float4x4 & CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);
				
			static ::Oyster::Math::Float4x4 & CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);

			static ::Oyster::Math::Float4x4 & CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );

			static ::Oyster::Math::Float4x4 & CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
	
			static ::Oyster::Math::Float4x4 & CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
		};

		class PHYSICS_DLL_USAGE API
		{
		public:
			typedef void (*EventAction_Collision)( unsigned int, unsigned int );
			typedef void (*EventAction_Destruction)( unsigned int, ::Utility::DynamicMemory::UniquePointer<ICustomBody> );

			/** Gets the Physics instance. */
			static API & Instance();

			/********************************************************
			 * Sets the time length of each physics update frame.
			 ********************************************************/
			virtual void SetDeltaTime( float seconds ) = 0;
			
			/********************************************************
			 * Sets the Gravityconstant in the physics that will be
			 * used in ForceField calculations.
			 * @param g: Default is the real world Constant::gravity_constant [N(m/kg)^2]
			 ********************************************************/
			virtual void SetGravityConstant( float g ) = 0;

			/********************************************************
			 * Sets the function that will be called by the engine
			 * whenever a subscribed collision occurs.
			 ********************************************************/
			virtual void SetAction( EventAction_Collision functionPointer ) = 0;
			
			/********************************************************
			 * Sets the function that will be called by the engine
			 * whenever an object is being destroyed for some reason.
			 * - Because DestroyObject(...) were called.
			 * - Out of memory forced engine to destroy an object.
			 ********************************************************/
			virtual void SetAction( EventAction_Destruction functionPointer ) = 0;

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

			/********************************************************
			 * Apply force on an object.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			 * @param worldF: Vector with the direction and magnitude of the force. [N]
			 ********************************************************/
			virtual void ApplyForceAt( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF ) = 0;
			
			/********************************************************
			 * Apply force on an object.
			 * @param objRefA: A pointer to the ICustomBody representing a physical object.
			 * @param objRefB: A pointer to the ICustomBody representing a physical object.
			 * @param deltaWhen: The elapsed simulation time since last update frame. [s]
			 * @param worldPointOfContact: Point of Collision, relative to the world origo. (Not relative to the objects) [m]
			 ********************************************************/
			virtual void ApplyCollisionResponse( const ICustomBody* objRefA, const ICustomBody* objRefB, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) = 0;

			/********************************************************
			 * Sets the MomentOfInertia tensor matrix of an object without changing it's angular velocity.
			 * Noticeable effect: The angular momentum will change. Changing the amount of kinetic energy.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepVelocity( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * Sets the MomentOfInertia tensor matrix of an object without changing it's angular momentum.
			 * Noticeable effect: The angular velocity will change. Can be used to create slow effects.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepMomentum( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * Sets the mass of an object without changing it's linear velocity.
			 * Noticeable effect: The linear momentum will change. Changing the amount of kinetic energy.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param m: [kg]
			 ********************************************************/
			virtual void SetMass_KeepVelocity( const ICustomBody* objRef, ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * Sets the mass of an object without changing it's linear velocity.
			 * Noticeable effect: The linear velocity will change. Can be used to create slow effects.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param m: [kg]
			 ********************************************************/
			virtual void SetMass_KeepMomentum( const ICustomBody* objRef, ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * Instantly moves an object.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			 ********************************************************/
			virtual void SetCenter( const ICustomBody* objRef, const ::Oyster::Math::Float3 &worldPos ) = 0;
			
			/********************************************************
			 * Instantly redirects object.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param rotation: New rotation.
			 ********************************************************/
			virtual void SetRotation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &rotation ) = 0;
			
			/********************************************************
			 * Instantly moves and redirects object.
			 * @param objRef: A pointer to the ICustomBody representing a physical object.
			 * @param orientation: New orientation.
			 ********************************************************/
			virtual void SetOrientation( const ICustomBody* objRef, const ::Oyster::Math::Float4x4 &orientation ) = 0;
			
			/********************************************************
			 * Creates a new dynamically allocated object that can be used as a component for more complex ICustomBodies.
			 * @return A pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateSimpleRigidBody() const = 0;
		
		protected:
			virtual ~API() {}
		};
	
		//! The root interface for all physical representations processable by the engine.
		class PHYSICS_DLL_USAGE ICustomBody
		{
		public:
			virtual ~ICustomBody() {};

			/********************************************************
			 * Creates a complete copy of the current (type)object.
			 * @return An ICustomBody pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const = 0;
			
			/********************************************************
			 * @return true if Engine should call the EventAction_Collision function.
			 ********************************************************/
			virtual bool IsSubscribingCollisions() const = 0;

			/********************************************************
			 * Performs a detailed Intersect test and returns if, when and where.
			 * @param object: What this is intersect testing against.
			 * @param deltaWhen: Time in seconds since last update frame til timeOfContact. 0.0f <= deltaWhen <= deltaTime
			 * @param worldPointOfContact: Where at timeOfContact, this and object touches eachother.
			 * @return true if this truly intersects with object.
			 ********************************************************/
			virtual bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const = 0;
			
			/********************************************************
			 * param shape: Any defined sample shape.
			 * @return true if this truly intersects with shape.
			 ********************************************************/
			virtual bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const = 0;

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
			virtual ::Oyster::Math::Float3 & GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			
			/********************************************************
			 * The world position of this center of gravity.
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a position in worldSpace.
			 ********************************************************/
			virtual ::Oyster::Math::Float3 & GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			
			/********************************************************
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a copy of this's rotation matrix.
			 ********************************************************/
			virtual ::Oyster::Math::Float4x4 & GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			
			/********************************************************
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a copy of this's orientation matrix.
			 ********************************************************/
			virtual ::Oyster::Math::Float4x4 & GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			
			/********************************************************
			 * @param targetMem: Provided memory that written into and then returned.
			 * @return a copy of this's view matrix.
			 ********************************************************/
			virtual ::Oyster::Math::Float4x4 & GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;

			/********************************************************
			 * To be only called by Engine
			 * Is called during API::Update
			 ********************************************************/
			virtual UpdateState Update( ::Oyster::Math::Float timeStepLength ) = 0;

			/********************************************************
			 * To be only called by Engine
			 * Use API::SetMomentOfInertiaTensor_KeepVelocity(...) instead
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetMomentOfInertiaTensor_KeepMomentum(...)
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetMass_KeepVelocity(...)
			 ********************************************************/
			virtual void SetMass_KeepVelocity( ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetMass_KeepMomentum(...)
			 ********************************************************/
			virtual void SetMass_KeepMomentum( ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetCenter(...)
			 ********************************************************/
			virtual void SetCenter( const ::Oyster::Math::Float3 &worldPos ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetRotation(...)
			 ********************************************************/
			virtual void SetRotation( const ::Oyster::Math::Float4x4 &rotation ) = 0;
			
			/********************************************************
			 * To be only called by Engine
			 * Use API::SetOrientation(...)
			 ********************************************************/
			virtual void SetOrientation( const ::Oyster::Math::Float4x4 &orientation ) = 0;
		};
	}
}
#endif