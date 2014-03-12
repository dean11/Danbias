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
			struct Gravity;
		}

		enum UpdateState
		{
			UpdateState_resting,
			UpdateState_altered
		};

		namespace Constant
		{
			const float gravity_constant = (const float)6.67284e-11; //!< The _big_G_! ( N(m/kg)^2 ) Used in real gravityforcefields.
			const float epsilon = (const float)1.0e-3;
		}

		class PHYSICS_DLL_USAGE API
		{
		public:
			typedef Struct::SimpleBodyDescription SimpleBodyDescription;
			typedef Struct::SphericalBodyDescription SphericalBodyDescription;
			typedef Struct::Gravity Gravity;

			typedef void (*EventAction_Destruction)( ::Utility::DynamicMemory::UniquePointer<ICustomBody> proto );
			typedef void (*EventAction_ApplyEffect)(ICustomBody* collidedBody, void* args);

			/** Gets the Physics instance. */
			static API & Instance();

			/********************************************************
			 * Clears all content and reset Engine assetts such as buffers.
			 * @param numObjects: The predicted max number of active objects.
			 * @param numGravityWells: The predicted max number of active gravity wells.
			 * @param worldSize: The size of acceptable physics space.
			 ********************************************************/
			virtual void Init() = 0;

			/********************************************************
			 * Resets stuff //Last week fixing. //Dennis
			 ********************************************************/
			virtual void Release() = 0;

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

			virtual void SetGravityPoint(::Oyster::Math::Float3 gravityPoint) = 0; 
			virtual void SetGravity(float gravity) = 0; 

			// Bullet physics
			virtual ICustomBody* AddCollisionSphere(float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction) = 0;
			virtual ICustomBody* AddCollisionBox(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction) = 0;
			virtual ICustomBody* AddCollisionCylinder(::Oyster::Math::Float3 halfSize, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction) = 0;

			virtual ICustomBody* AddCharacter(::Oyster::Math::Float height, ::Oyster::Math::Float radius, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, float mass, float restitution, float staticFriction, float dynamicFriction) = 0;

			virtual ICustomBody* AddTriangleMesh(const std::wstring fileName, ::Oyster::Math::Float4 rotation, ::Oyster::Math::Float3 position, ::Oyster::Math::Float3 scale, float mass, float restitution, float staticFriction, float dynamicFriction) = 0;

			virtual void SetTimeStep(float timeStep) = 0;

			virtual void UpdateWorld(float deltaTime) = 0;



			/********************************************************
			 * Applies an effect to objects that collide with the set volume.
			 * @param collideable: An ICollideable that defines the volume of the effect.
			 * @param args: The arguments needed for the hitAction function.
			 * @param hitAction: A function that contains the effect. Parameterlist contains the custom body 
								 the collideable hits, and the arguments sent to the function.
			 ********************************************************/
			virtual void ApplyEffect(Oyster::Collision3D::ICollideable* collideable, void* args, EventAction_ApplyEffect effect) = 0;

			virtual ICustomBody* RayClosestObjectNotMe(ICustomBody* self, Oyster::Math::Float3 origin, Oyster::Math::Float3 target) = 0;

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
				SubscriptMessage_kineticLoss,
				SubscriptMessage_ignore_collision_response,
				SubscriptMessage_player_collision_response
			};

			
			typedef SubscriptMessage (*EventAction_BeforeCollisionResponse)( const ICustomBody *proto, const ICustomBody *deuter );
			typedef void (*EventAction_AfterCollisionResponse)( const ICustomBody *proto, const ICustomBody *deuter );
			typedef void (*EventAction_Move)( const ICustomBody *object );
			typedef Struct::CustomBodyState State;

			virtual ~ICustomBody() {};

			virtual State GetState() const = 0;
			virtual State & GetState( State &targetMem ) const = 0;
			virtual void SetState( const State &state ) = 0;

			virtual void SetSubscription(EventAction_AfterCollisionResponse function) = 0;
			virtual void SetSubscription(EventAction_Move function) = 0;

			virtual void ApplyImpulse(::Oyster::Math::Float3 impulse) = 0;

			virtual void SetLinearVelocity(::Oyster::Math::Float3 velocity) = 0;
			virtual void SetPosition(::Oyster::Math::Float3 position) = 0;
			virtual void SetRotation(::Oyster::Math::Float4 quaternion) = 0;
			virtual void SetRotation(::Oyster::Math::Quaternion quaternion) = 0;
			virtual void SetRotation(::Oyster::Math::Float3 eulerAngles) = 0;
			virtual void SetRotation(::Oyster::Math::Float4x4 rotation) = 0;
			virtual void AddRotationAroundY(::Oyster::Math::Float angle) = 0;
			virtual void SetAngularFactor(::Oyster::Math::Float factor) = 0;
			virtual void SetMass(::Oyster::Math::Float mass) = 0;

			virtual void SetGravity(::Oyster::Math::Float3 gravity) = 0;

			virtual void SetUpAndRight(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 right) = 0;
			virtual void SetUpAndForward(::Oyster::Math::Float3 up, ::Oyster::Math::Float3 forward) = 0;
			virtual void SetUp(::Oyster::Math::Float3 up) = 0;

			virtual ::Oyster::Math::Float4x4 GetRotation() const = 0;
			virtual ::Oyster::Math::Float4 GetRotationAsAngularAxis() = 0;
			virtual ::Oyster::Math::Float4x4 GetOrientation() const = 0;
			virtual ::Oyster::Math::Float4x4 GetView() const = 0;
			virtual ::Oyster::Math::Float4x4 GetView(const ::Oyster::Math::Float3 &offset) const = 0;

			virtual ::Oyster::Math::Float3 GetGravity() const = 0;
			virtual ::Oyster::Math::Float3 GetLinearVelocity() const = 0;

			virtual void CallSubscription_AfterCollisionResponse(ICustomBody* bodyA, ICustomBody* bodyB) = 0;
			virtual void CallSubscription_Move() = 0;

			virtual void DisableCollisionResponse() = 0;
			virtual void TurnOnCollisionResponse() = 0;

			/********************************************************
			 * @return the void pointer set by SetCustomTag.
			 * nullptr if none is set.
			 ********************************************************/
			virtual void* GetCustomTag() const = 0;

			/********************************************************
			 * Not used by the engine itself. Just a quality of life feature
			 * for developers who want to tag something to the objects.
			 * @param ref: Anything castable to a void pointer, the engine won't care.
			 ********************************************************/
			virtual void SetCustomTag( void *ref ) = 0;

			virtual float GetLambdaUp() const = 0;
			virtual float GetLambdaForward() const = 0;

			virtual void OverrideGravity(const ::Oyster::Math::Float3& point, const ::Oyster::Math::Float gravityForce) = 0;
		};
	}
}

#include "PhysicsStructs.h"

#endif