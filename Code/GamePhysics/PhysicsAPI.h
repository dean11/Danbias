#ifndef PHYSICS_API_H
#define PHYSICS_API_H

#include "OysterCollision3D.h"
#include "OysterMath.h"

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

		class MomentOfInertia
		{
		public:
			static ::Oyster::Math::Float4x4 & CreateSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);
				
			static ::Oyster::Math::Float4x4 & CreateHollowSphereMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float radius);

			static ::Oyster::Math::Float4x4 & CreateCuboidMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float width, const ::Oyster::Math::Float depth );

			static ::Oyster::Math::Float4x4 & CreateCylinderMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float height, const ::Oyster::Math::Float radius );
	
			static ::Oyster::Math::Float4x4 & CreateRodMatrix( const ::Oyster::Math::Float mass, const ::Oyster::Math::Float length );
		};

		class API
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
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @return true if object is in limbo state.
			 ********************************************************/
			virtual bool IsInLimbo( unsigned int objRef ) = 0;
			
			/********************************************************
			 * An object in limbo state will be ignored during the physics frame Update.
			 * This will put an object in Limbo state.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 ********************************************************/
			virtual void MoveToLimbo( unsigned int objRef ) = 0;
			
			/********************************************************
			 * An object in limbo state will be ignored during the physics frame Update.
			 * This will clear the accumulated force/torque and remove the Limbo state.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 ********************************************************/
			virtual void ReleaseFromLimbo( unsigned int objRef ) = 0;

			/********************************************************
			 * Introduces a new object into the engine.
			 * @param handle: A pointer along with the responsibility to delete.
			 * @return an objectReference needed in most methods. Error::not_a_reference if faulty handle.
			 ********************************************************/
			virtual unsigned int AddObject( ::Utility::DynamicMemory::UniquePointer<ICustomBody> handle ) = 0;

			/********************************************************
			 * Fetches and removes an object from the engine.
			 * Will not call the provided EventAction_Destruction method.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @return A pointer along with the responsibility to delete. NULL if faulty objRef.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> ExtractObject( unsigned int objRef ) = 0;

			/********************************************************
			 * Removes an object from the engine.
			 * Will call the provided EventAction_Destruction method. Not if objRef is faulty.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 ********************************************************/
			virtual void DestroyObject( unsigned int objRef ) = 0;

			/********************************************************
			 * Peeks at object at objRef.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @return the instance ofthe found object. Error::nobody if faulty objRef
			 ********************************************************/
			virtual const ICustomBody & Peek( unsigned int objRef ) const = 0;

			/********************************************************
			 * Apply force on an object.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			 * @param worldF: Vector with the direction and magnitude of the force. [N]
			 ********************************************************/
			virtual void ApplyForceAt( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos, const ::Oyster::Math::Float3 &worldF ) = 0;
			
			/********************************************************
			 * Apply force on an object.
			 * @param objRefA: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param objRefB: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param deltaWhen: The elapsed simulation time since last update frame. [s]
			 * @param worldPointOfContact: Point of Collision, relative to the world origo. (Not relative to the objects) [m]
			 ********************************************************/
			virtual void ApplyCollisionResponse( unsigned int objRefA, unsigned int objRefB, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) = 0;

			/********************************************************
			 * Sets the MomentOfInertia tensor matrix of an object without changing it's angular velocity.
			 * Noticeable effect: The angular momentum will change. Changing the amount of kinetic energy.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepVelocity( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * Sets the MomentOfInertia tensor matrix of an object without changing it's angular momentum.
			 * Noticeable effect: The angular velocity will change. Can be used to create slow effects.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param localI: The tensor matrix relative to the axises of the object. @see MomentOfInertia namespace.
			 ********************************************************/
			virtual void SetMomentOfInertiaTensor_KeepMomentum( unsigned int objRef, const ::Oyster::Math::Float4x4 &localI ) = 0;
			
			/********************************************************
			 * Sets the mass of an object without changing it's linear velocity.
			 * Noticeable effect: The linear momentum will change. Changing the amount of kinetic energy.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param m: [kg]
			 ********************************************************/
			virtual void SetMass_KeepVelocity( unsigned int objRef, ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * Sets the mass of an object without changing it's linear velocity.
			 * Noticeable effect: The linear velocity will change. Can be used to create slow effects.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param m: [kg]
			 ********************************************************/
			virtual void SetMass_KeepMomentum( unsigned int objRef, ::Oyster::Math::Float m ) = 0;
			
			/********************************************************
			 * Instantly moves an object.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param worldPos: Relative to the world origo. (Not relative to object) [m]
			 ********************************************************/
			virtual void SetCenter( unsigned int objRef, const ::Oyster::Math::Float3 &worldPos ) = 0;
			
			/********************************************************
			 * Instantly redirects object.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param rotation: New rotation.
			 ********************************************************/
			virtual void SetRotation( unsigned int objRef, const ::Oyster::Math::Float4x4 &rotation ) = 0;
			
			/********************************************************
			 * Instantly moves and redirects object.
			 * @param objRef: Provided by API::AddObject(...) & ICustomBody::GetReference()
			 * @param orientation: New orientation.
			 ********************************************************/
			virtual void SetOrientation( unsigned int objRef, const ::Oyster::Math::Float4x4 &orientation ) = 0;
			
			/********************************************************
			 * Creates a new dynamically allocated object that can be used as a component for more complex ICustomBodies.
			 * @return A pointer along with the responsibility to delete.
			 ********************************************************/
			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> CreateSimpleRigidBody() const = 0;
		
		protected:
			virtual ~API() {}
		};
	
		class ICustomBody
		{
		public:
			virtual ~ICustomBody() {};

			virtual ::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const = 0;
			
			virtual bool IsSubscribingCollisions() const = 0;
			virtual bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const = 0;
			virtual bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const = 0;

			virtual unsigned int					GetReference() const = 0;
			virtual ::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const = 0;
			virtual ::Oyster::Math::Float3 &		GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			virtual ::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;
			virtual ::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const = 0;

			virtual UpdateState Update( ::Oyster::Math::Float timeStepLength ) = 0;

			virtual void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI ) = 0;
			virtual void SetMass_KeepVelocity( ::Oyster::Math::Float m ) = 0;
			virtual void SetMass_KeepMomentum( ::Oyster::Math::Float m ) = 0;
			virtual void SetCenter( const ::Oyster::Math::Float3 &worldPos ) = 0;
			virtual void SetRotation( const ::Oyster::Math::Float4x4 &rotation ) = 0;
			virtual void SetOrientation( const ::Oyster::Math::Float4x4 &orientation ) = 0;
		};

		namespace Error
		{ //! The content in here is used as API return errorvalues.
			const unsigned int not_a_reference = ::Utility::Value::numeric_limits<unsigned int>::max();

			class NullBody : public ICustomBody
			{
			public:
				virtual ~NullBody();

				::Utility::DynamicMemory::UniquePointer<ICustomBody> Clone() const;
			
				bool IsSubscribingCollisions() const;
				bool Intersects( const ICustomBody &object, ::Oyster::Math::Float &deltaWhen, ::Oyster::Math::Float3 &worldPointOfContact ) const;
				bool Intersects( const ::Oyster::Collision3D::ICollideable &shape ) const;

				unsigned int					GetReference() const;
				::Oyster::Collision3D::Sphere &	GetBoundingSphere( ::Oyster::Collision3D::Sphere &targetMem = ::Oyster::Collision3D::Sphere() ) const;
				::Oyster::Math::Float3 &		GetNormalAt( const ::Oyster::Math::Float3 &worldPos, ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
				::Oyster::Math::Float3 &		GetCenter( ::Oyster::Math::Float3 &targetMem = ::Oyster::Math::Float3() ) const;
				::Oyster::Math::Float4x4 &		GetRotation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
				::Oyster::Math::Float4x4 &		GetOrientation( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;
				::Oyster::Math::Float4x4 &		GetView( ::Oyster::Math::Float4x4 &targetMem = ::Oyster::Math::Float4x4() ) const;

				UpdateState Update( ::Oyster::Math::Float timeStepLength );

				void SetMomentOfInertiaTensor_KeepVelocity( const ::Oyster::Math::Float4x4 &localI );
				void SetMomentOfInertiaTensor_KeepMomentum( const ::Oyster::Math::Float4x4 &localI );
				void SetMass_KeepVelocity( ::Oyster::Math::Float m );
				void SetMass_KeepMomentum( ::Oyster::Math::Float m );
				void SetCenter( const ::Oyster::Math::Float3 &worldPos );
				void SetRotation( const ::Oyster::Math::Float4x4 &rotation );
				void SetOrientation( const ::Oyster::Math::Float4x4 &orientation );

			} const nobody;
		}
	}
}
#endif