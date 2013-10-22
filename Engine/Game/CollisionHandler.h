/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef OYSTER_COLLISIONHANDLER_H
#define OYSTER_COLLISIONHANDLER_H

#include "OysterMath.h"
#include "Sphere.h"

using namespace Oyster::Math;
using namespace Oyster::Collision;

namespace Oyster { namespace Game
{
// HEAD /////////////////////////////////////////////////////////////

	template<typename Entity>
	class CollisionHandler
	{
	public:
		class Reference
		{
			friend class CollisionHandler;
		public:
			static const Reference invalid;

			Reference( const Reference &ref );
			~Reference( );

			bool operator == ( const Reference &ref ) const;
			bool operator != ( const Reference &ref ) const;

			Float getBoundaryReach( ) const;
			const Float3 & getPosition( ) const;
//			const Float & getScaling( ) const;
			const unsigned int & getChannel( ) const;
			CollisionHandler * getHandler( ) const;

			void setBoundaryReach( const Float &radius );
			void setPosition( const Float3 &position );
//			void setScale( const Float &scale );
			
			void moveToLimbo( );
			void leaveLimbo( );

			bool isActive( ) const;
			bool isInLimbo( ) const;

		private:
			CollisionHandler *handler;
			unsigned int ref;

			Reference( CollisionHandler *handler = NULL, unsigned int ref = 0 );
		};
		static const typename Reference outOfSlots;

		class VisitingInstance
		{
		public:
			virtual ~VisitingInstance( ) {};
			virtual void action( Entity *hitEntity ) = 0;
		};

		typedef void (*VisitingAction)( Entity *visitor, Entity *hitEntity );

		CollisionHandler( );
		virtual ~CollisionHandler( );

		void emptyAndReInit( unsigned int maxNumEntities, unsigned int numChannels );
		void update( );

		Reference addEntity( unsigned int channel, Entity *entity, const ::Oyster::Collision::Sphere &collisionBubble, const Float &scale = 1.0f );
		void visit( unsigned int *channels, unsigned int numChannels, const ::Oyster::Collision::ICollideable *sampler, VisitingInstance *visitor );
		void visit( unsigned int *channels, unsigned int numChannels, const ::Oyster::Collision::ICollideable *sampler, Entity *visitor, VisitingAction action );
		bool collidesWithSomething( unsigned int *channels, unsigned int numChannels, const ::Oyster::Collision::ICollideable *sampler );

	private:
		enum State { Updated, Changed, Limbo, Vacant };
		struct Data { Entity *entity; unsigned int channel, numHooks; Float scale; State state; } *data;
		::Oyster::Collision::Sphere *presence;
		unsigned int maxNumEntities, numEntities;

		// TODO: Optimize by implementing a BSP oct tree here below (A)

		struct Channel { Channel(); unsigned int *content, num; } *channel;
		unsigned int numChannels;

		bool simpleCollisionConfirmation( const typename Entity &entity, const ::Oyster::Collision::ICollideable *sampler );
	};

// BODY /////////////////////////////////////////////////////////////
	template<typename Entity>
	const typename CollisionHandler<Entity>::Reference CollisionHandler<Entity>::outOfSlots = CollisionHandler<Entity>::Reference();

	template<typename Entity>
	CollisionHandler<Entity>::CollisionHandler( ) : data(NULL), presence(NULL), maxNumEntities(0), numEntities(0), channel(NULL), numChannels(0) {}

	template<typename Entity>
	CollisionHandler<Entity>::~CollisionHandler( )
	{
		if( this->data ) delete [] this->data;
		if( this->presence ) delete [] this->presence;

		// TODO: Optimize by implementing a BSP oct tree here below (B)
		if( this->channel )
		{
			for( unsigned int i = 0; i < this->numChannels; ++i ) if( this->channel[i].content )
				delete [] this->channel[i].content;
			delete [] this->channel;
		}
	}

	template<typename Entity>
	void CollisionHandler<Entity>::emptyAndReInit( unsigned int maxNumEntities, unsigned int numChannels )
	{
		if( this->data ) delete [] this->data;
		if( this->presence ) delete [] this->presence;

		this->maxNumEntities = maxNumEntities;
		this->numEntities = 0;
		if( maxNumEntities > 0 )
		{
			this->data = new CollisionHandler<Entity>::Data[maxNumEntities];
			this->presence = new Sphere[maxNumEntities];
		}
		else
		{
			this->data = NULL;
			this->presence = NULL;
		}

		// TODO: Optimize by implementing a BSP oct tree here below (C)
		if( this->channel )
		{
			for( unsigned int i = 0; i < this->numChannels; ++i ) if( this->channel[i].content )
				delete [] this->channel[i].content;
			delete [] this->channel;
		}
		this->numChannels = numChannels;
		this->channel = numChannels > 0 ? new CollisionHandler<Entity>::Channel[numChannels] : NULL;
	}

	template<typename Entity>
	void CollisionHandler<Entity>::update( )
	{
		// TODO: Optimize by implementing a BSP oct tree here below (D)
		for( unsigned int i = 0; i < this->numEntities; ++i ) if( this->data[i].state == CollisionHandler<Entity>::Changed )
			this->data[i].state = CollisionHandler<Entity>::Updated;
	}

	template<typename Entity>
	typename CollisionHandler<Entity>::Reference CollisionHandler<Entity>::addEntity( unsigned int channel, typename Entity *entity, const ::Oyster::Collision::Sphere &collisionBubble, const Float &scale )
	{
		if( this->numEntities < this->maxNumEntities )
		{
			this->data[this->numEntities].entity = entity;
			this->data[this->numEntities].channel = channel;
			this->data[this->numEntities].numHooks = 0;
			this->data[this->numEntities].scale = scale;
			this->data[this->numEntities].state = CollisionHandler<Entity>::Updated;
			this->presence[this->numEntities] = collisionBubble;

			// TODO: Optimize by implementing a BSP oct tree here below (E)

			if( this->channel[channel].content == NULL )
			{
				this->channel[channel].content = new unsigned int[this->maxNumEntities];
				this->channel[channel].num = 0;
			}

			this->channel[channel].content[this->channel[channel].num] = this->numEntities;
			++this->channel[channel].num;
			
			//////
			return CollisionHandler<Entity>::Reference( this, this->numEntities++ );
		}
		else return CollisionHandler<Entity>::outOfSlots;
	}

	template<typename Entity>
	void CollisionHandler<Entity>::visit( unsigned int *_channel, unsigned int _numChannels, const ::Oyster::Collision::ICollideable *sampler, typename CollisionHandler<Entity>::VisitingInstance *visitor )
	{
		// TODO: Optimize by implementing a BSP oct tree here below (F1)
		for( unsigned int ch = 0; ch < _numChannels; ++ch )
			for( unsigned int i = 0; i < this->channel[_channel[ch]].num; ++i )
				if( this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Limbo && this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Vacant )
					if( sampler->Intersects( &this->presence[this->channel[ch].content[i]] ) )
						visitor->action( this->data[this->channel[ch].content[i]].entity );
	}

	template<typename Entity>
	void CollisionHandler<Entity>::visit( unsigned int *_channel, unsigned int _numChannels, const ::Oyster::Collision::ICollideable *sampler, typename Entity *visitor, typename CollisionHandler<Entity>::VisitingAction action )
	{
		// TODO: Optimize by implementing a BSP oct tree here below (F2)
		for( unsigned int ch = 0; ch < _numChannels; ++ch )
			for( unsigned int i = 0; i < this->channel[_channel[ch]].num; ++i )
				if( this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Limbo && this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Vacant )
					if( sampler->Intersects( &this->presence[this->channel[ch].content[i]] ) )
						action( visitor, this->data[this->channel[ch].content[i]].entity );
	}

	template<typename Entity>
	bool CollisionHandler<Entity>::collidesWithSomething( unsigned int *_channel, unsigned int _numChannels, const ::Oyster::Collision::ICollideable *sampler )
	{
		// TODO: Optimize by implementing a BSP oct tree here below (F3)
		for( unsigned int ch = 0; ch < _numChannels; ++ch )
			for( unsigned int i = 0; i < this->channel[_channel[ch]].num; ++i )
				if( this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Limbo && this->data[this->channel[ch].content[i]].state != CollisionHandler<Entity>::Vacant )
					if( sampler->Intersects( &this->presence[this->channel[ch].content[i]] ) )
						if( this->simpleCollisionConfirmation(*this->data[this->channel[ch].content[i]].entity, sampler) )
							return true;
		return false;
	}
	
	template<typename Entity>
	bool CollisionHandler<Entity>::simpleCollisionConfirmation( const typename Entity &entity, const ::Oyster::Collision::ICollideable *sampler )
	{ return true; }

	// CollisionHandler::Channel ////////////////////////////////////
	template<typename Entity>
	CollisionHandler<Entity>::Channel::Channel( ) : content(NULL), num(0) {}

	// CollisionHandler::Reference ////////////////////////////

	template<typename Entity> const typename CollisionHandler<Entity>::Reference CollisionHandler<Entity>::Reference::invalid = CollisionHandler<Entity>::Reference();

	template<typename Entity>
	CollisionHandler<Entity>::Reference::Reference( CollisionHandler *_handler, unsigned int _ref )
		: handler(_handler), ref(_ref)
	{ /* TODO : implement a slot allocation system */
		if( this->handler )
		{
			++this->handler->data[this->ref].numHooks;
			if( this->handler->data[this->ref].state == CollisionHandler<Entity>::Vacant )
				this->handler->data[this->ref].state = CollisionHandler<Entity>::Limbo;
		}
	}

	template<typename Entity>
	CollisionHandler<Entity>::Reference::Reference( const Reference &ref )
		: handler(ref.handler), ref(ref.ref)
	{ /* TODO : implement a slot allocation system */
		if( this->handler )
		{
			++this->handler->data[this->ref].numHooks;
			if( this->handler->data[this->ref].state == CollisionHandler<Entity>::Vacant )
				this->handler->data[this->ref].state = CollisionHandler<Entity>::Limbo;
		}
	}

	template<typename Entity>
	CollisionHandler<Entity>::Reference::~Reference( )
	{ /* TODO : implement a slot allocation system */
		if( this->handler )
		{
			if( this->handler->data[this->ref].numHooks > 0 )
				if( --this->handler->data[this->ref].numHooks == 0 )
					this->handler->data[this->ref].state = CollisionHandler<Entity>::Vacant;
		}
	}

	template<typename Entity>
	bool CollisionHandler<Entity>::Reference::operator == ( const typename CollisionHandler<Entity>::Reference &ref ) const
	{
		if( this->handler != ref.handler ) return false;
		if( this->ref != ref.ref ) return false;
		return true;
	}

	template<typename Entity>
	bool CollisionHandler<Entity>::Reference::operator != ( const typename CollisionHandler<Entity>::Reference &ref ) const
	{
		if( this->handler != ref.handler ) return true;
		if( this->ref != ref.ref ) return true;
		return false;
	}

	template<typename Entity>
	Float CollisionHandler<Entity>::Reference::getBoundaryReach( ) const
	{ return this->handler->presence[this->ref].radius; }// / this->handler->data[this->ref].scale; }

	template<typename Entity>
	const Float3 & CollisionHandler<Entity>::Reference::getPosition( ) const
	{ return this->handler->presence[this->ref].center; }
	
//	template<typename Entity>
//	const Float & CollisionHandler<Entity>::Reference::getScaling( ) const
//	{ return this->handler->data[this->ref].scale; }
	
	template<typename Entity>
	const unsigned int & CollisionHandler<Entity>::Reference::getChannel( ) const
	{ return this->handler->data[this->ref].channel; }

	template<typename Entity>
	CollisionHandler<Entity> * CollisionHandler<Entity>::Reference::getHandler( ) const
	{ return this->handler; }
	
	template<typename Entity>
	void CollisionHandler<Entity>::Reference::setBoundaryReach( const Float &radius )
	{
		this->handler->presence[this->ref].radius = radius; // * this->handler->data[this->ref].scale;
		if( this->handler->data[this->ref].state != CollisionHandler<Entity>::Limbo )
				this->handler->data[this->ref].state = CollisionHandler<Entity>::Changed; // TODO : implement a queue system
	}

	template<typename Entity>
	void CollisionHandler<Entity>::Reference::setPosition( const Float3 &position )
	{
		this->handler->presence[this->ref].center = position;
		if( this->handler->data[this->ref].state != CollisionHandler<Entity>::Limbo )
				this->handler->data[this->ref].state = CollisionHandler<Entity>::Changed; // TODO : implement a queue system
	}

//	template<typename Entity>
//	void CollisionHandler<Entity>::Reference::setScale( const Float &scale )
//	{
//		this->handler->presence[this->ref].radius *= scale / this->handler->data[this->ref].scale;
//		this->handler->data[this->ref].scale = scale;
//
//		if( this->handler->data[this->ref].state != CollisionHandler<Entity>::Limbo )
//			this->handler->data[this->ref].state = CollisionHandler<Entity>::Changed; // TODO : implement a queue system
//	}

	template<typename Entity>
	void CollisionHandler<Entity>::Reference::moveToLimbo( )
	{ /* TODO : implement a limbo system */
		this->handler->data[this->ref].state = CollisionHandler<Entity>::Limbo;
	}

	template<typename Entity>
	void CollisionHandler<Entity>::Reference::leaveLimbo( )
	{ /* TODO : implement a limbo system */
		this->handler->data[this->ref].state = CollisionHandler<Entity>::Changed;
	}

	template<typename Entity>
	bool CollisionHandler<Entity>::Reference::isActive( ) const
	{
		switch( this->handler->data[this->ref].state )
		{
		case Updated:
		case Changed:
			return true;
		default:
			return false;
		}
	}

	template<typename Entity>
	bool CollisionHandler<Entity>::Reference::isInLimbo( ) const
	{ return ( this->handler->data[this->ref].state == Limbo ); }
} }

#endif