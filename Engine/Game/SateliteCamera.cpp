#include "SateliteCamera.h"

using namespace Oyster::Game;
using namespace Oyster::Math;
using namespace Oyster::Collision;

namespace PrivateStatic
{
	inline void towardsZero( Float &target, const Float &maxStep )
	{
		if( target > maxStep ) target -= maxStep;
		else if( target < -maxStep ) target += maxStep;
		else target = 0.0f;
	}
}

SateliteCamera::SateliteCamera( const Float4x4 *rigidBody, const Float3 &offsetPos, const Float3 &upVector, const Float4x4 &_projection )
	: offsetRadian(Float3::null), offsetPan(Float3::null), offsetDistance(0.0f), focus(rigidBody),
	  defaultRelation(Float4x4::identity), projection(_projection), view(), viewProjection(),
	  sampler(), viewIsOutOfDate(true), viewProjectionIsOutOfDate(true), samplerIsOutOfDate(true)
{
	this->defaultRelation.v[3].xyz = offsetPos;
	this->defaultRelation.v[2].xyz = offsetPos.getNormalized() * -1.0f;
	this->defaultRelation.v[1].xyz = (upVector - ( this->defaultRelation.v[2].xyz * this->defaultRelation.v[2].xyz.dot(upVector) )).getNormalized();
	this->defaultRelation.v[0].xyz = this->defaultRelation.v[1].xyz.cross( this->defaultRelation.v[2].xyz );
}

void SateliteCamera::setFocus( const Float4x4 *rigidBody, const Float3 &offsetPos, const Float3 &upVector )
{
	this->defaultRelation.v[3].xyz = offsetPos;
	this->defaultRelation.v[2].xyz = offsetPos.getNormalized();
	this->defaultRelation.v[1].xyz = (upVector - ( this->defaultRelation.v[2].xyz * this->defaultRelation.v[2].xyz.dot(upVector) )).getNormalized();
	this->defaultRelation.v[0].xyz = this->defaultRelation.v[1].xyz.cross( this->defaultRelation.v[2].xyz );
	this->defaultRelation.v[2].xyz *= -1.0f;

	this->focus = rigidBody;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::setProjection( const Float4x4 &transform )
{
	this->projection = transform;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

const ::Oyster::Math::Float4x4 & SateliteCamera::getView( ) const
{
//	if( this->viewIsOutOfDate )
	{
		Float4x4 world = this->defaultRelation;

		if( this->offsetDistance != 0.0f )
			world.v[3].xyz += world.v[2].xyz * this->offsetDistance;

		if( this->offsetRadian != Float3::null || this->offsetPan != Float3::null )
		{
			Float4x4 m;
			rigidBodyMatrix( m, this->offsetRadian, offsetPan );
			transformMatrix( world, world, m );
		}

		transformMatrix( world, world, *this->focus );
		inverseRigidBodyMatrix( this->view, world );

		this->viewIsOutOfDate = false;
	}
	return this->view;
}

const Float4x4 & SateliteCamera::getProjection( ) const
{ return this->projection; }

const Float4x4 & SateliteCamera::getViewProjection( ) const
{
//	if( this->viewProjectionIsOutOfDate )
	{
		viewProjectionMatrix( this->viewProjection, this->getView(), this->projection );
		this->viewProjectionIsOutOfDate = false;
	}
	return this->viewProjection;
}

const Frustrum & SateliteCamera::getSampler( ) const
{
	if( this->samplerIsOutOfDate )
	{
		this->sampler = this->getViewProjection();
		this->samplerIsOutOfDate = false;
	}
	return this->sampler;
}

void SateliteCamera::revolveLeft( const Float &deltaRadian )
{
	this->offsetRadian.y += deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::revolveRight( const Float &deltaRadian )
{
	this->offsetRadian.y -= deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::revolveUp( const Float &deltaRadian )
{
	this->offsetRadian.x += deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::revolveDown( const Float &deltaRadian )
{
	this->offsetRadian.x -= deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::revolveRollLeft( const Float &deltaRadian )
{
	this->offsetRadian.z += deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::revolveRollRight( const Float &deltaRadian )
{
	this->offsetRadian.z -= deltaRadian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::setHorizontalRevolution( const Float &radian )
{
	this->offsetRadian.y = radian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::setVerticalRevolution( const Float &radian )
{
	this->offsetRadian.x = radian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::setRollRevolution( const Float &radian )
{
	this->offsetRadian.z = radian;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::moveIn( const Float &deltaLength )
{
	this->offsetDistance -= deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::moveOut( const Float &deltaLength )
{
	this->offsetDistance += deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panForward( const Float &deltaLength )
{
	this->offsetPan.z += deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panBackward( const Float &deltaLength )
{
	this->offsetPan.z -= deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panLeft( const Float &deltaLength )
{
	this->offsetPan.x -= deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panRight( const Float &deltaLength )
{
	this->offsetPan.x += deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panUp( const Float &deltaLength )
{
	this->offsetPan.y += deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::panDown( const Float &deltaLength )
{
	this->offsetPan.y -= deltaLength;
	this->viewIsOutOfDate = true;
	this->viewProjectionIsOutOfDate = true;
	this->samplerIsOutOfDate = true;
}

void SateliteCamera::stabiliseRoll( const Float &deltaRadian )
{
	if( this->offsetRadian.z != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetRadian.z, deltaRadian );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabiliseHorizontally( const Float &deltaRadian )
{
	if( this->offsetRadian.y != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetRadian.y, deltaRadian );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabiliseVertically( const Float &deltaRadian )
{
	if( this->offsetRadian.x != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetRadian.x, deltaRadian );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabiliseDistance( const Float &deltaLength )
{
	if( this->offsetDistance != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetDistance, deltaLength );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabilisePanX( const Float &deltaLength )
{
	if( this->offsetPan.x != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetPan.x, deltaLength );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabilisePanY( const Float &deltaLength )
{
	if( this->offsetPan.y != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetPan.y, deltaLength );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}

void SateliteCamera::stabilisePanZ( const Float &deltaLength )
{
	if( this->offsetPan.z != 0.0f )
	{
		PrivateStatic::towardsZero( this->offsetPan.z, deltaLength );
		this->viewIsOutOfDate = true;
		this->viewProjectionIsOutOfDate = true;
		this->samplerIsOutOfDate = true;
	}
}