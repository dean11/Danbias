#include "Object.h"

#include <limits>
#include <vector>
#include <map>

using namespace ::Oyster::Math;
using namespace ::Oyster::Collision;
using namespace ::Oyster::Game;
using ::std::string;
using ::std::vector;
using ::std::map;

namespace GameLogic
{
	namespace PrivateStatic
	{
		struct EntityFileState
		{
			bool isLoaded;
			vector<unsigned int> bluePrintRef;
			EntityFileState() : isLoaded(false), bluePrintRef() {}
		};

		map<string, EntityFileState> loadedEntityFileLibrary;
		map<string, unsigned int> blueprintIndex = map<string, unsigned int>();
		vector<InstanceBlueprint*> blueprint = vector<InstanceBlueprint*>();
		const InstanceBlueprint invalidBluePrint = InstanceBlueprint();
		unsigned int numCreatedObjects = 0;

		class WatchDog
		{
		public:
			WatchDog( ) {}
			~WatchDog( )		
			{
				vector<InstanceBlueprint*>::size_type numBluePrints = PrivateStatic::blueprint.size();
				for( vector<InstanceBlueprint*>::size_type i = 0; i < numBluePrints; ++i ) delete PrivateStatic::blueprint[i];
			}
		} watchDog;
	}

	const unsigned int Object::invalidID = ::std::numeric_limits<unsigned int>::max();

	Object::Result Object::init( const string &iniFile )
	{/*
		if( PrivateStatic::blueprint.size() > 0 )
		{ // ReInit friendly
			vector<InstanceBlueprint*>::size_type numBluePrints = PrivateStatic::blueprint.size();
			for( vector<InstanceBlueprint*>::size_type i = 0; i < numBluePrints; ++i ) delete PrivateStatic::blueprint[i];
			PrivateStatic::blueprint = vector<InstanceBlueprint*>();
		}*/

		// remove?
		return Success;
	}

	Object::Result Object::importEntities( vector<unsigned int> &outputConfigID, const string &entityFile, const Float4x4 &transform )
	{
		outputConfigID.resize( 0 );

		PrivateStatic::EntityFileState *entityFileState = &PrivateStatic::loadedEntityFileLibrary[entityFile];

		if( entityFileState->isLoaded )
		{
			outputConfigID = entityFileState->bluePrintRef;
			return Success;
		}

		vector<InstanceBlueprint*> loadedBlueprint;
		vector<string> loadedBlueprintRefNames;
		if( InstanceBlueprint::loadFromFile( loadedBlueprint, loadedBlueprintRefNames, entityFile, transform ) == InstanceBlueprint::Success )
		{
			vector<InstanceBlueprint*>::size_type i = 0,
												  numLoaded = loadedBlueprint.size();

			for( ; i < numLoaded; ++i )
			{
				outputConfigID.push_back( (unsigned int)PrivateStatic::blueprint.size() );
				PrivateStatic::blueprintIndex[ ::Utility::String::toLowerCase(loadedBlueprintRefNames[i]) ] = (unsigned int)PrivateStatic::blueprint.size();
				PrivateStatic::blueprint.push_back( loadedBlueprint[i] );
			}

			entityFileState->isLoaded = true;
			entityFileState->bluePrintRef = outputConfigID;
			return Success;
		}
		return Failure;
	}

	void Object::clearEntityresources( )
	{
		PrivateStatic::loadedEntityFileLibrary.clear();

		vector<InstanceBlueprint*>::size_type numBluePrints = PrivateStatic::blueprint.size();
		for( vector<InstanceBlueprint*>::size_type i = 0; i < numBluePrints; ++i )
			delete PrivateStatic::blueprint[i];
		
		PrivateStatic::blueprint.resize( 0 );
	}

	unsigned int Object::getNumObjectsCreated( )
	{ return PrivateStatic::numCreatedObjects; }

	const InstanceBlueprint & Object::getConfig( unsigned int id )
	{
		if( id < PrivateStatic::blueprint.size() )
			return *PrivateStatic::blueprint[id];
		else
			return PrivateStatic::invalidBluePrint;
	}

	const InstanceBlueprint & Object::getConfig( const ::std::string &handle )
	{
		map<string, unsigned int>::const_iterator i = PrivateStatic::blueprintIndex.find( ::Utility::String::toLowerCase(string(handle)) );
		if( i != PrivateStatic::blueprintIndex.end() )
			return *PrivateStatic::blueprint[i->second];
		else
			return PrivateStatic::invalidBluePrint;
	}

	void Object::onCollisionAction( Object *visitor, Object *hitEntity )
	{
		if( visitor == hitEntity ) return;
		if( visitor->body.Intersects( &hitEntity->body ) )
		{
			Float3 pushVector = (visitor->body.orientation.v[3].xyz - hitEntity->body.orientation.v[3].xyz).getNormalized();
			pushVector = vectorProjection( visitor->speed, pushVector );
			pushVector *= 2.0f;
			visitor->speed -= pushVector;
			visitor->acceleration=Float3::null;
		}
	}

// INSTANCE /////////////////////////////////////////////////////////

	Object::Object( )
		: MoveAble(), objectID(PrivateStatic::numCreatedObjects++), configID(Object::invalidID),
		physicsID(CollisionHandler<Object>::Reference::invalid), mass(1.0f), disableReduceMovementCount(0),
		disableReduceRotationCount(0), scaling(1.0f, 1.0f, 1.0f), view(Float4x4::identity), world(Float4x4::identity), viewIsOutOfDate(true), worldIsOutOfDate(true)
	{
		this->movementProperty.maxSpeed = this->movementProperty.deAcceleration = this->movementProperty.acceleration.forward = this->movementProperty.acceleration.backward = this->movementProperty.acceleration.horizontal = this->movementProperty.acceleration.vertical = 0.0f;
		this->rotationProperty.maxSpeed = this->rotationProperty.deAcceleration = this->rotationProperty.acceleration.pitch = this->rotationProperty.acceleration.yaw = this->rotationProperty.acceleration.roll = 0.0f;
	}
	
	Object::Object( const Object &object )
		: MoveAble(object), objectID(object.objectID), configID(object.configID), physicsID(object.physicsID), mass(object.mass),
		  movementProperty(object.movementProperty), rotationProperty(object.rotationProperty),
		  disableReduceMovementCount(object.disableReduceMovementCount), disableReduceRotationCount(object.disableReduceRotationCount),
		  scaling(object.scaling), view(object.view), world(object.world), viewIsOutOfDate(object.viewIsOutOfDate), worldIsOutOfDate(object.worldIsOutOfDate) {}

	Object::Object( const Float4x4 &orientation, const Float3 &centerOfMass )
		: MoveAble(orientation, centerOfMass), objectID(PrivateStatic::numCreatedObjects++), configID(invalidID),
		  physicsID(CollisionHandler<Object>::Reference::invalid), mass(1.0f), disableReduceMovementCount(0),
		  disableReduceRotationCount(0), scaling(1.0f, 1.0f, 1.0f), view(Float4x4::identity), world(Float4x4::identity), viewIsOutOfDate(true), worldIsOutOfDate(true)
	{ this->movementProperty.maxSpeed = this->movementProperty.deAcceleration = this->movementProperty.acceleration.forward = this->movementProperty.acceleration.backward = this->movementProperty.acceleration.horizontal = this->movementProperty.acceleration.vertical = 0.0f; }

	Object::Object( const Box &body, const Float3 &centerOfMass )
		: MoveAble(body, centerOfMass), objectID(PrivateStatic::numCreatedObjects++), configID(invalidID),
		  physicsID(CollisionHandler<Object>::Reference::invalid), mass(1.0f), disableReduceMovementCount(0),
		  disableReduceRotationCount(0), scaling(1.0f, 1.0f, 1.0f), view(Float4x4::identity), world(Float4x4::identity), viewIsOutOfDate(true), worldIsOutOfDate(true)
	{
		this->movementProperty.maxSpeed = this->movementProperty.deAcceleration = this->movementProperty.acceleration.forward = this->movementProperty.acceleration.backward = this->movementProperty.acceleration.horizontal = this->movementProperty.acceleration.vertical = 0.0f;
		this->rotationProperty.maxSpeed = this->rotationProperty.deAcceleration = this->rotationProperty.acceleration.pitch = this->rotationProperty.acceleration.yaw = this->rotationProperty.acceleration.roll = 0.0f;
	}
	
	Object::~Object( ) {}

	Object & Object::operator = ( const Object &object )
	{
		MoveAble::operator=(object);
		this->objectID = object.objectID;
		this->configID = object.configID;
		this->physicsID = object.physicsID;
		this->mass = object.mass;
		this->movementProperty = object.movementProperty;
		this->rotationProperty = object.rotationProperty;
		this->disableReduceMovementCount = object.disableReduceMovementCount;
		this->disableReduceRotationCount = object.disableReduceRotationCount;
		this->scaling = object.scaling;

		if( !object.viewIsOutOfDate )
			this->view = object.view;
		if( !object.worldIsOutOfDate )
			this->world = object.world;
		this->viewIsOutOfDate = object.viewIsOutOfDate;
		this->worldIsOutOfDate = object.worldIsOutOfDate;

		return *this;
	}

	Object * Object::clone( ) const
	{ return new Object(*this); }

	Object::Type Object::getType( ) const
	{ return Undefined; }

	const unsigned int & Object::getObjectID( ) const
	{ return this->objectID; }
	
	const unsigned int & Object::getConfigID( ) const
	{ return this->configID; }

	const CollisionHandler<Object>::Reference & Object::getPhysicsID( ) const
	{ return this->physicsID; }

	const Float & Object::getMass( ) const
	{ return this->mass; }

	const Float3 & Object::getCenterOfMass( ) const
	{ return this->rotationPivot; }
	
	Float3 Object::getInertia( const Float3 &offset ) const
	{ return MoveAble::getMovement( offset ) *= this->mass; }

	const Box & Object::getCollisionBox( ) const
	{ return this->body; }

	bool Object::isActive( ) const
	{ return this->physicsID.isActive(); }

	void Object::setConfig( unsigned int id )
	{
		this->configID = id;
		this->loadConfig();
	}

	void Object::setConfig( const ::std::string &id )
	{
		map<string, unsigned int>::const_iterator ref = PrivateStatic::blueprintIndex.find( ::Utility::String::toLowerCase(string(id)) );
		if( ref != PrivateStatic::blueprintIndex.end() )
		{
			this->configID = ref->second;
			this->loadConfig();	
		}
	}

	void Object::loadConfig( )
	{
		if( this->configID != Object::invalidID )
			this->loadConfig( *PrivateStatic::blueprint[this->configID] );
	}

	void Object::setPhysicsID( const CollisionHandler<Object>::Reference &id )
	{ this->physicsID = id; }

	void Object::setMass( const Float &mass )
	{ this->mass = mass; }

	void Object::setCenterOfMass( const Float3 &localPos )
	{ this->rotationPivot = localPos; }

	void Object::setScaling( const ::Oyster::Math::Float3 &scaleL )
	{
		this->body.boundingOffset.x /= this->scaling.x;
		this->body.boundingOffset.y /= this->scaling.y;
		this->body.boundingOffset.z /= this->scaling.z;
		this->body.boundingOffset *= scaleL;

		if( this->physicsID != CollisionHandler<Object>::Reference::invalid )
			this->physicsID.setBoundaryReach( this->body.boundingOffset.length() );

		this->scaling = scaleL;
		this->worldIsOutOfDate = this->viewIsOutOfDate = true;
	}

	void Object::applyForceL( const Float3 &localForce, const Float3 &localPos )
	{ MoveAble::accelerate( localForce / this->mass, localPos ); }

	void Object::applyForceW( const Float3 &worldForce, const Float3 &worldPos )
	{
		Float4 localForce, localPos;
		transformVector( localForce, Float4(worldForce, 0.0f), this->getViewNormalMatrix() );
		transformVector( localPos, Float4(worldPos, 1.0f), this->getViewPointMatrix() );

		MoveAble::accelerate( localForce.xyz / this->mass, localPos.xyz );
	}

	void Object::moveToLimbo( )
	{
		Float4x4 m = this->getOrientation();
		m.v[0]=m.v[1]=m.v[2]=m.v[3] = Float4::null;
		this->setOrientation(m);
		this->stop();
		this->physicsID.moveToLimbo(); 
	}

	void Object::releaseFromLimbo( )
	{ 
		this->physicsID.leaveLimbo(); 
	}

	void Object::update( )
	{
		// Speed Control
		Float speedSquared = this->speed.dot( this->speed );

		if( speedSquared > ( this->movementProperty.maxSpeed * this->movementProperty.maxSpeed ) )
		{ // drop the speed to max and kill acceleration this update
			this->speed *= ( this->movementProperty.maxSpeed / ::std::sqrt(speedSquared) );
			//this->acceleration = Float3::null;
		}
		if( this->disableReduceMovementCount == 0 )
		{ // kill movement if moving slow enough, or deAccelerate
			Float deAccelerate = this->movementProperty.deAcceleration * MoveAble::getDiscreteTimeSlice();

			if( speedSquared <= ( deAccelerate * deAccelerate ) )
				this->speed = Float3::null;
			else 
			{
				speedSquared = ::std::sqrt(speedSquared);
				this->speed *= (speedSquared - deAccelerate) / speedSquared;
			}
		}

		// Rotation Control
		speedSquared = this->rotationalSpeed.dot( this->rotationalSpeed );

		if( speedSquared > ( this->rotationProperty.maxSpeed * this->rotationProperty.maxSpeed ) )
		{ // drop the rotationSpeed to max and kill acceleration this update
			this->rotationalSpeed *= ( this->rotationProperty.maxSpeed / ::std::sqrt(speedSquared) );
			this->rotationalAcceleration = Float3::null;
		}
		if( this->disableReduceRotationCount == 0 )
		{ // kill rotation if moving slow enough, or deAccelerate
			Float deAccelerate = this->rotationProperty.deAcceleration * MoveAble::getDiscreteTimeSlice();

			if( speedSquared <= ( deAccelerate * deAccelerate ) )
				this->rotationalSpeed = Float3::null;
			else 
			{
				speedSquared = ::std::sqrt(speedSquared);
				this->rotationalSpeed *= (speedSquared - deAccelerate) / speedSquared;
			}
		}

		MoveAble::update();
		if( this->physicsID != CollisionHandler<Object>::Reference::invalid )
			this->physicsID.setPosition( this->body.orientation.v[3].xyz );
		this->worldIsOutOfDate = this->viewIsOutOfDate = true;
	}

	void Object::pitchUp( )
	{ this->accelerateTurn( this->body.orientation.v[0].xyz * -this->rotationProperty.acceleration.pitch ); }

	void Object::pitchDown( )
	{ this->accelerateTurn( this->body.orientation.v[0].xyz * this->rotationProperty.acceleration.pitch ); }

	void Object::yawLeft( )
	{ this->accelerateTurn( this->body.orientation.v[1].xyz * -this->rotationProperty.acceleration.yaw ); }

	void Object::yawRight( )
	{ this->accelerateTurn( this->body.orientation.v[1].xyz * this->rotationProperty.acceleration.yaw ); }

	void Object::rollLeft( )
	{ this->accelerateTurn( this->body.orientation.v[2].xyz * this->rotationProperty.acceleration.roll ); }

	void Object::rollRight( )
	{ this->accelerateTurn( this->body.orientation.v[2].xyz * -this->rotationProperty.acceleration.roll ); }

	void Object::thrustForward( )
	{ this->accelerate( this->body.orientation.v[2].xyz * this->movementProperty.acceleration.forward ); }

	void Object::thrustBackward( )
	{ this->accelerate( this->body.orientation.v[2].xyz * -this->movementProperty.acceleration.backward ); }

	void Object::strafeLeft( )
	{ this->accelerate( this->body.orientation.v[0].xyz * -this->movementProperty.acceleration.horizontal ); }

	void Object::strafeRight( )
	{ this->accelerate( this->body.orientation.v[0].xyz * this->movementProperty.acceleration.horizontal ); }

	void Object::climb( )
	{ this->accelerate( this->body.orientation.v[1].xyz * this->movementProperty.acceleration.vertical ); }

	void Object::dive( )
	{ this->accelerate( this->body.orientation.v[1].xyz * -this->movementProperty.acceleration.vertical ); }

	void Object::disableMovementReduction( )
	{ ++this->disableReduceMovementCount; }
	
	void Object::enableMovementReduction( bool forceIt )
	{
		if( forceIt )
			this->disableReduceMovementCount = 0;
		else if( --this->disableReduceMovementCount < 0) this->disableReduceMovementCount = 0;
	}

	void Object::disableRotationReduction( )
	{ ++this->disableReduceRotationCount; }
	
	void Object::enableRotationReduction( bool forceIt )
	{
		if( forceIt )
			this->disableReduceRotationCount = 0;
		else if( --this->disableReduceRotationCount < 0) this->disableReduceRotationCount = 0;
	}

	Object::Result Object::writeToKeyFrame( KeyFrame &buffer ) const
	{
		if( buffer.numObjects < (signed)::Utility::StaticArray::numElementsOf( buffer.Objects ) )
		{
			buffer.Objects[buffer.numObjects].id = this->objectID;
			buffer.Objects[buffer.numObjects].TypeId = this->configID;
			buffer.Objects[buffer.numObjects].RotationVec = this->rotationalSpeed;
			buffer.Objects[buffer.numObjects].MoveVec = this->speed;

			//TODO: edit to world when they are avaliable
			buffer.Objects[buffer.numObjects].World = this->body.orientation;

			//buffer.Objects[buffer.numObjects].ForwardVec = this->body.orientation.v[2].xyz;
			//buffer.Objects[buffer.numObjects].UpVec = this->body.orientation.v[1].xyz;

			++buffer.numObjects;
			return Success;
		}
		return Failure;
	}

	const Float4x4 & Object::getWorldPointMatrix( ) const
	{
		if( this->worldIsOutOfDate )
		{
			transformMatrix( this->world,
							 Float4x4( Float4::standardUnitX * this->scaling.x,
									   Float4::standardUnitY * this->scaling.y,
									   Float4::standardUnitZ * this->scaling.z,
									   Float4::standardUnitW ),
							 this->body.orientation );
			this->worldIsOutOfDate = false;
		}
		return this->world;
	}

	const Float4x4 & Object::getViewPointMatrix( ) const
	{
		if( this->viewIsOutOfDate )
		{
			this->view = this->getWorldPointMatrix().getInverse();
			this->viewIsOutOfDate = false;
		}
		return this->view;
	}

	void Object::setOrientation( const Float4x4 & orientation )
	{
		MoveAble::setOrientation( orientation );
		if( this->physicsID != CollisionHandler<Object>::Reference::invalid )
			this->physicsID.setPosition( orientation.v[3].xyz );
		this->worldIsOutOfDate = this->viewIsOutOfDate = true;
	}

	const InstanceBlueprint & Object::getConfigData( unsigned int configID )
	{ return *PrivateStatic::blueprint[configID]; }

	void Object::loadConfig( const InstanceBlueprint &config )
	{
		this->mass = (Float)config.mass;
		this->setCenterOfMass( config.centerOfMass );
		this->body.boundingOffset = ::Utility::Value::abs( config.hitBox.position ) + config.hitBox.halfSize;
		
		this->movementProperty.maxSpeed = config.movementProperty.maxSpeed;
		this->movementProperty.deAcceleration = config.movementProperty.deAcceleration;
		this->movementProperty.acceleration.forward = config.movementProperty.acceleration.forward;
		this->movementProperty.acceleration.backward = config.movementProperty.acceleration.backward;
		this->movementProperty.acceleration.horizontal = config.movementProperty.acceleration.horizontal;
		this->movementProperty.acceleration.vertical = config.movementProperty.acceleration.vertical;
		
		this->rotationProperty.maxSpeed = config.rotationProperty.maxSpeed;
		this->rotationProperty.deAcceleration = config.rotationProperty.deAcceleration;
		this->rotationProperty.acceleration.pitch = config.rotationProperty.acceleration.pitch;
		this->rotationProperty.acceleration.yaw = config.rotationProperty.acceleration.yaw;
		this->rotationProperty.acceleration.roll = config.rotationProperty.acceleration.roll;

		if( this->physicsID != CollisionHandler<Object>::Reference::invalid )
			this->physicsID.setBoundaryReach( this->body.boundingOffset.length() );
	}
}

namespace Oyster { namespace Game
{
	template <>
	bool CollisionHandler<::GameLogic::Object>::simpleCollisionConfirmation( const typename ::GameLogic::Object &entity, const ::Oyster::Collision::ICollideable *sampler )
	{ return sampler->Intersects( &entity.getCollisionBox() ); }
} }