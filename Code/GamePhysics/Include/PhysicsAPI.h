#ifndef PHYSICS_API_H
#define PHYSICS_API_H

#include "OysterMath.h"

namespace Oyster
{
	namespace Physics
	{
		class API;
		class IRigidBody;
		class IParticle;	

		class API
		{
		public:
			static API & Instance();
		};

		class IRigidBody
		{
		public:

		};

		class IParticle
		{
		public:

		};
	}

	namespace Collision
	{}
}
#endif