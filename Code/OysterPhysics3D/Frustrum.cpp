/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Frustrum.h"
#include "OysterCollision3D.h"

using namespace ::Oyster::Math;
using namespace ::Oyster::Collision3D;

namespace PrivateStatic
{
	inline void VP_ToPlanes( Plane &lp, Plane &rp, Plane &bp, Plane &tp, Plane &np, Plane &fp, const Float4x4 &vp )
	{ /// TODO: : not verified
		Float4x4 m = vp.GetTranspose();

		// left
		lp.normal  = Float4(m.v[3].xyz + m.v[0].xyz,0);
		lp.phasing = lp.normal.GetMagnitude();
		lp.normal /= lp.phasing;
		lp.phasing = (m.v[3].w + m.v[0].w) / lp.phasing;

		// right
		rp.normal  = Float4(m.v[3].xyz - m.v[0].xyz,0);
		rp.phasing = rp.normal.GetMagnitude();
		rp.normal /= rp.phasing;
		rp.phasing = (m.v[3].w - m.v[0].w) / rp.phasing;
		
		// bottom
		bp.normal  = Float4(m.v[3].xyz + m.v[1].xyz,0);
		bp.phasing = bp.normal.GetMagnitude();
		bp.normal /= bp.phasing;
		bp.phasing = (m.v[3].w + m.v[1].w) / bp.phasing;

		// top
		tp.normal  = Float4(m.v[3].xyz - m.v[1].xyz,0);
		tp.phasing = tp.normal.GetMagnitude();
		tp.normal /= tp.phasing;
		tp.phasing = (m.v[3].w - m.v[1].w) / tp.phasing;
		
		// near leftHanded DirectX
		np.normal  = Float4(m.v[2].xyz,0);
		np.phasing = np.normal.GetMagnitude();
		np.normal /= np.phasing;
		np.phasing = m.v[2].w / np.phasing;

		// far lefthanded
		fp.normal  = Float4(m.v[3].xyz - m.v[2].xyz,0);
		fp.phasing = fp.normal.GetMagnitude();
		fp.normal /= fp.phasing;
		fp.phasing = (m.v[3].w - m.v[2].w) / fp.phasing;

		/*/ near rightHanded openGL
		np.normal  = m.v[3].xyz + m.v[2].xyz;
		np.phasing = np.normal.GetMagnitude();
		np.normal /= np.phasing;
		np.phasing = -(m.v[3].w + m.v[2].w) / np.phasing;

		// far rightHanded
		fp.normal  = m.v[3].xyz - m.v[2].xyz;
		fp.phasing = fp.normal.GetMagnitude();
		fp.normal /= fp.phasing;
		fp.phasing = -(m.v[3].w - m.v[2].w) / fp.phasing;*/
	}

	void InterpolatePlanes( Plane &target, const Plane &planeA, const Plane &planeB, float interpolation )
	{
		float counterInterpol = 1.0f - interpolation;
		target.normal   = counterInterpol * planeA.normal;
		target.normal  += interpolation * planeB.normal;
		target.phasing  = counterInterpol * planeA.phasing;
		target.phasing += interpolation * planeB.phasing;
		target.normal.Normalize();
	}
}

Frustrum::Frustrum() : ICollideable(Type_frustrum)
{
	this->leftPlane   = Plane( Float4::standard_unit_x, -0.5f );
	this->rightPlane  = Plane(-Float4::standard_unit_x,  0.5f ),
	this->bottomPlane = Plane( Float4::standard_unit_y, -0.5f );
	this->topPlane	  = Plane(-Float4::standard_unit_y,  0.5f );
	this->nearPlane	  = Plane( Float4::standard_unit_z, -0.5f );
	this->farPlane	  = Plane(-Float4::standard_unit_z,  0.5f );
}

Frustrum::Frustrum( const Float4x4 &vp ) : ICollideable(Type_frustrum)
{
	PrivateStatic::VP_ToPlanes( this->leftPlane, this->rightPlane, this->bottomPlane,
								this->topPlane,  this->nearPlane,  this->farPlane,
								vp );
}

Frustrum::~Frustrum() {}

Frustrum & Frustrum::operator = ( const Frustrum &frustrum )
{
	this->plane[0] = frustrum.plane[0];
	this->plane[1] = frustrum.plane[1];
	this->plane[2] = frustrum.plane[2];
	this->plane[3] = frustrum.plane[3];
	this->plane[4] = frustrum.plane[4];
	this->plane[5] = frustrum.plane[5];
	return *this;
}

Frustrum & Frustrum::operator = ( const Float4x4 &vp )
{
	PrivateStatic::VP_ToPlanes( this->leftPlane, this->rightPlane, this->bottomPlane, this->topPlane, this->nearPlane, this->farPlane, vp );
	return *this;
}

void Frustrum::Split( unsigned int numX, unsigned int numY, unsigned int numZ, Frustrum target[] ) const
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

	Plane invRight( -this->rightPlane.normal, -this->rightPlane.phasing ),
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
					PrivateStatic::InterpolatePlanes( target[i].leftPlane, this->leftPlane, invRight, interpolX );

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
					PrivateStatic::InterpolatePlanes( target[i].topPlane, this->topPlane, invBottom, interpolY );

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
					PrivateStatic::InterpolatePlanes( target[i].nearPlane, this->nearPlane, invFar, interpolZ );

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

void Frustrum::WriteToByte( unsigned int &nextIndex, unsigned char targetMem[] ) const
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
	nextIndex += 6 * ::Utility::StaticArray::NumElementsOf( this->plane[0].byte );
}

::Utility::DynamicMemory::UniquePointer<ICollideable> Frustrum::Clone( ) const
{
	return ::Utility::DynamicMemory::UniquePointer<ICollideable>( new Frustrum(*this) );
}

bool Frustrum::Intersects( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_universe:			return true;
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	case Type_ray:				return Utility::Intersect( *this, (const Ray&)target, ((const Ray&)target).collisionDistance );
	case Type_sphere:			return Utility::Intersect( *this, (const Sphere&)target );
	case Type_plane:			return Utility::Intersect( *this, (const Plane&)target );
	//case Type_triangle:			return false; // TODO: 
	case Type_box_axis_aligned:	return Utility::Intersect( *this, (const BoxAxisAligned&)target );
	case Type_box:				return Utility::Intersect( *this, (const Box&)target );
	case Type_frustrum:			return Utility::Intersect( *this, (const Frustrum&)target );
	//case Type_line:				return false; // TODO: 
	default:					return false;
	}
}

bool Frustrum::Intersects( const ICollideable &target, Float4 &worldPointOfContact ) const
{
	//! @todo TODO: implement stub properly
	return this->Intersects( target );
}

bool Frustrum::Contains( const ICollideable &target ) const
{
	switch( target.type )
	{
	case Type_point:			return Utility::Intersect( *this, (const Point&)target );
	//case Type_ray:				return false; // TODO: 
	//case Type_sphere:				return false; // TODO: 
	//case Type_plane:				return false; // TODO: 
	//case Type_triangle:			return false; // TODO: 
	//case Type_box_axis_aligned:	return false; // TODO: 
	//case Type_box:				return false; // TODO: 
	//case Type_frustrum:			return false; // TODO: 
	//case Type_line:				return false; // TODO: 
	default:					return false;
	}
}

Float Frustrum::TimeOfContact( const ICollideable &deuterStart, const ICollideable &deuterEnd ) const
{
	if( deuterStart.type != deuterEnd.type )
		return -1.0f;

	switch( deuterStart.type )
	{ // TODO: more to implement
	case Type_universe:			return 0.0f;
	default:					return 1.0f;
	}
}

::Oyster::Math::Float3 Frustrum::ExtractForwad()
{
	return this->bottomPlane.normal.xyz;
}