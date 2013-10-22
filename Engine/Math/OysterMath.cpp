/////////////////////////////////////////////////////////////////////
// by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "OysterMath.h"

namespace Oyster { namespace Math
{
	Float2 & operator *= ( Float2 &left, const Float2 &right )
	{
		left.x *= right.x;
		left.y *= right.y;
		return left;
	}

	Float3 & operator *= ( Float3 &left, const Float3 &right )
	{
		left.x *= right.x;
		left.y *= right.y;
		left.z *= right.z;
		return left;
	}

	Float4 & operator *= ( Float4 &left, const Float4 &right )
	{
		left.x *= right.x;
		left.y *= right.y;
		left.z *= right.z;
		left.w *= right.w;
		return left;
	}
} }