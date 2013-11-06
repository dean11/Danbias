/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Frustrum.h"
#include "Collision.h"

using namespace Oyster::Math;
using namespace Oyster::Collision;

namespace PrivateStatic
{
	inline void vpToPlanes( Plane &lp, Plane &rp, Plane &bp, Plane &tp, Plane &np, Plane &fp, const Float4x4 &vp )
	{
		Float4x4 m = vp.getTranspose();

		// left
		lp.normal  = m.v[3].xyz + m.v[0].xyz;
		lp.phasing = lp.normal.length();
		lp.normal /= lp.phasing;
		lp.phasing = (m.v[3].w + m.v[0].w) / lp.phasing;

		// right
		rp.normal  = m.v[3].xyz - m.v[0].xyz;
		rp.phasing = rp.normal.length();
		rp.normal /= rp.phasing;
		rp.phasing = (m.v[3].w - m.v[0].w) / rp.phasing;
		
		// bottom
		bp.normal  = m.v[3].xyz + m.v[1].xyz;
		bp.phasing = bp.normal.length();
		bp.normal /= bp.phasing;
		bp.phasing = (m.v[3].w + m.v[1].w) / bp.phasing;

		// top
		tp.normal  = m.v[3].xyz - m.v[1].xyz;
		tp.phasing = tp.normal.length();
		tp.normal /= tp.phasing;
		tp.phasing = (m.v[3].w - m.v[1].w) / tp.phasing;
		
		// near leftHanded DirectX
		np.normal  = m.v[2].xyz;
		np.phasing = np.normal.length();
		np.normal /= np.phasing;
		np.phasing = m.v[2].w / np.phasing;

		// far lefthanded
		fp.normal  = m.v[3].xyz - m.v[2].xyz;
		fp.phasing = fp.normal.length();
		fp.normal /= fp.phasing;
		fp.phasing = (m.v[3].w - m.v[2].w) / fp.phasing;

		/*/ near rightHanded openGL
		np.normal  = m.v[3].xyz + m.v[2].xyz;
		np.phasing = np.normal.length();
		np.normal /= np.phasing;
		np.phasing = -(m.v[3].w + m.v[2].w) / np.phasing;

		// far rightHanded
		fp.normal  = m.v[3].xyz - m.v[2].xyz;
		fp.phasing = fp.normal.length();
		fp.normal /= fp.phasing;
		fp.phasing = -(m.v[3].w - m.v[2].w) / fp.phasing;*/
	}

	void interpolatePlanes( Plane &target, const Plane &planeA, const Plane &planeB, float interpolation )
	{
		float counterInterpol = 1.0f - interpolation;
		target.normal   = counterInterpol * planeA.normal;
		target.normal  += interpolation * planeB.normal;
		target.phasing  = counterInterpol * planeA.phasing;
		target.phasing += interpolation * planeB.phasing;
		target.normal.normalize();
	}
}

Frustrum::Frustrum( ) : ICollideable(ICollideable::Frustrum),
	leftPlane(Float3::standardUnitX, -0.5f), rightPlane(-Float3::standardUnitX, 0.5f),
	bottomPlane(Float3::standardUnitY, -0.5f), topPlane(-Float3::standardUnitY, 0.5f),
	nearPlane(Float3::standardUnitZ, -0.5f), farPlane(-Float3::standardUnitZ, 0.5f) {}

Frustrum::Frustrum( const Frustrum &frustrum ) : ICollideable(ICollideable::Frustrum),
	leftPlane(frustrum.leftPlane), rightPlane(frustrum.rightPlane),
	bottomPlane(frustrum.bottomPlane), topPlane(frustrum.topPlane),
	nearPlane(frustrum.nearPlane), farPlane(frustrum.farPlane) {}

Frustrum::Frustrum( const Float4x4 &vp ) : ICollideable(ICollideable::Frustrum)
{ PrivateStatic::vpToPlanes( this->leftPlane, this->rightPlane, this->bottomPlane, this->topPlane, this->nearPlane, this->farPlane, vp ); }

Frustrum::~Frustrum( ) {}

Frustrum & Frustrum::operator = ( const Frustrum &frustrum )
{
	this->leftPlane = frustrum.leftPlane;
	this->rightPlane = frustrum.rightPlane;
	this->bottomPlane = frustrum.bottomPlane;
	this->topPlane = frustrum.topPlane;
	this->nearPlane = frustrum.nearPlane;
	this->farPlane = frustrum.farPlane;
	return *this;
}

Frustrum & Frustrum::operator = ( const Float4x4 &vp )
{
	PrivateStatic::vpToPlanes( this->leftPlane, this->rightPlane, this->bottomPlane, this->topPlane, this->nearPlane, this->farPlane, vp );
	return *this;
}

void Frustrum::split( Frustrum target[], unsigned int numX, unsigned int numY, unsigned int numZ ) const
{
	float incrementX = 1.0f / numX,
		  incrementY = 1.0f / numY,
		  incrementZ = 1.0f / numZ,
		  interpolX = 0.0f,
		  interpolY = 0.0f,
		  interpolZ = 0.0f;
	
	unsigned int i = 0U,
				 stepY = numX,
				 stepZ = numX * numY;

	Collision::Plane invRight( -this->rightPlane.normal, -this->rightPlane.phasing ),
					 invBottom( -this->bottomPlane.normal, -this->bottomPlane.phasing ),
					 invFar( -this->farPlane.normal, -this->farPlane.phasing );

	for( unsigned int iz = 0U; iz < numZ; ++iz )
	{
		interpolY = 0.0f;
		for( unsigned int iy = 0U; iy < numY; ++iy )
		{
			interpolX = 0.0f;
			for( unsigned int ix = 0U; ix < numX; ++ix )
			{
				if( ix == 0 )
					target[i].leftPlane = this->leftPlane;
				else
				{
					PrivateStatic::interpolatePlanes( target[i].leftPlane, this->leftPlane, invRight, interpolX );

					unsigned int iLeft = i - 1;
					target[iLeft].rightPlane.normal  = -target[i].leftPlane.normal;
					target[iLeft].rightPlane.phasing = -target[i].leftPlane.phasing;
				}
				if( ix == numX - 1 )
					target[i].rightPlane = this->rightPlane;

				if( iy == 0 )
					target[i].topPlane = this->topPlane;
				else
				{
					PrivateStatic::interpolatePlanes( target[i].topPlane, this->topPlane, invBottom, interpolY );

					unsigned int iAbove = i - stepY;
					target[iAbove].bottomPlane.normal  = -target[i].topPlane.normal;
					target[iAbove].bottomPlane.phasing = -target[i].topPlane.phasing;
				}
				if( iy == numY - 1 )
					target[i].bottomPlane = this->bottomPlane;

				if( iz == 0 )
					target[i].nearPlane = this->nearPlane;
				else
				{
					PrivateStatic::interpolatePlanes( target[i].nearPlane, this->nearPlane, invFar, interpolZ );

					unsigned int iCloser = i - stepZ;
					target[iCloser].farPlane.normal  = -target[i].nearPlane.normal;
					target[iCloser].farPlane.phasing = -target[i].nearPlane.phasing;					
				}
				if( iz == numZ - 1 )
					target[i].farPlane = this->farPlane;

				++i;
				interpolX += incrementX;
			}
			interpolY += incrementY;
		}
		interpolZ += incrementZ;
	}
}

void Frustrum::writeToByte( unsigned char targetMem[], unsigned int &nextIndex ) const
{
	Float *fMem = (Float*)&targetMem[nextIndex];
	for( int p = 0; p < 6; ++p )
	{
		fMem[0] = this->plane[p].element[0];
		fMem[1] = this->plane[p].element[1];
		fMem[2] = this->plane[p].element[2];
		fMem[3] = this->plane[p].element[3];
		fMem = &fMem[4];
	}
	nextIndex += 6 * ::Utility::StaticArray::numElementsOf( this->plane[0].byte );
}

ICollideable* Frustrum::clone( ) const
{ return new Frustrum(*this); }

bool Frustrum::Intersects( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return Utility::intersect( *this, *(Collision::Ray*)target, ((Collision::Ray*)target)->collisionDistance );
	case ICollideable::Sphere: return Utility::intersect( *this, *(Collision::Sphere*)target );
	case ICollideable::Plane: return Utility::intersect( *this, *(Collision::Plane*)target );
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return Utility::intersect( *this, *(Collision::BoxAxisAligned*)target );
	case ICollideable::Box: return Utility::intersect( *this, *(Collision::Box*)target );
	case ICollideable::Frustrum: return Utility::intersect( *this, *(Collision::Frustrum*)target );
	case ICollideable::Line: return false; // TODO
	default: return false;
	}
}

bool Frustrum::Contains( const ICollideable *target ) const
{
	switch( target->type )
	{
	case ICollideable::Point: return Utility::intersect( *this, *(Collision::Point*)target );
	case ICollideable::Ray: return false; // TODO
	case ICollideable::Sphere: return false; // TODO
	case ICollideable::Plane: return false; // TODO
	case ICollideable::Triangle: return false; // TODO
	case ICollideable::BoxAxisAligned: return false; // TODO
	case ICollideable::Box: return false; // TODO
	case ICollideable::Frustrum: return false; // TODO
	case ICollideable::Line: return false; // TODO
	default: return false;
	}
}

ICollideable::State Frustrum::Advanced( const ICollideable *target ) const
{ return ICollideable::Missed; } //Not supported returns 0