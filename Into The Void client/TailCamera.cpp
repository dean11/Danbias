#include "TailCamera.h"

using namespace Oyster::Game;
using namespace Oyster::Math;

namespace PrivateStatic
{
	inline void applyOffsetBoundary( Float &target, const Float &boundary )
	{
		if( target > boundary ) target = boundary;
		else if( target < -boundary ) target = -boundary;
	}
}

TailCamera::TailCamera( const Float4x4 &rigidBody, const Float3 &offsetPos, const Float3 &upVector, const Float4x4 &projection )
	: SateliteCamera(&rigidBody, offsetPos, upVector, projection), maxOffsetRadian(1.5f), maxOffsetDistance(1.5f) {}

const Float4x4 & TailCamera::getView( ) const
{
	PrivateStatic::applyOffsetBoundary( this->offsetDistance, this->maxOffsetDistance );
	
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.x, this->maxOffsetRadian );
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.y, this->maxOffsetRadian );
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.z, this->maxOffsetRadian );
	
	PrivateStatic::applyOffsetBoundary( this->offsetPan.x, this->maxOffsetDistance );
	PrivateStatic::applyOffsetBoundary( this->offsetPan.y, this->maxOffsetDistance );
	PrivateStatic::applyOffsetBoundary( this->offsetPan.z, this->maxOffsetDistance );

	return SateliteCamera::getView();
}

const Float4x4 & TailCamera::getViewProjection( ) const
{
	PrivateStatic::applyOffsetBoundary( this->offsetDistance, this->maxOffsetDistance );
	
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.x, this->maxOffsetRadian );
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.y, this->maxOffsetRadian );
	PrivateStatic::applyOffsetBoundary( this->offsetRadian.z, this->maxOffsetRadian );
	
	PrivateStatic::applyOffsetBoundary( this->offsetPan.x, this->maxOffsetDistance );
	PrivateStatic::applyOffsetBoundary( this->offsetPan.y, this->maxOffsetDistance );
	PrivateStatic::applyOffsetBoundary( this->offsetPan.z, this->maxOffsetDistance );

	return SateliteCamera::getViewProjection();
}

void TailCamera::setMaxOffsetRadian( Float radian )
{ this->maxOffsetRadian = radian; }

void TailCamera::setMaxOffsetDistance( Float distance )
{ this->maxOffsetDistance = distance; }

void TailCamera::setRevolution( const Float &unitHorizontal, const Float &unitVertical )
{
	SateliteCamera::setHorizontalRevolution( unitHorizontal * this->maxOffsetRadian );
	SateliteCamera::setVerticalRevolution( unitVertical * this->maxOffsetRadian );
}