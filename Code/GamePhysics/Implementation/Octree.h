#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <map>
#include "Sphere.h"
#include "BoxAxisAligned.h"
#include "Utilities.h"
#include "../PhysicsAPI.h"

namespace Oyster
{
	namespace Physics
	{
		class Octree
		{
		public:
			struct Data
			{
				Data* prev;
				Data* next;

				Collision3D::Sphere container;

				Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef;

				unsigned int queueRef;
			};

			struct OctreeNode
			{

			};

			Octree(unsigned int bufferSize, unsigned char numLayers, Math::Float3 worldSize);
			virtual ~Octree();

			void AddObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void MoveToUpdateQueue(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void Update();

			void DestroyObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void Sample(Collision3D::ICollideable& collideable);
		private:
			std::vector < Data > leafData;

			std::map< ICustomBody*, unsigned int > mapReferences;

		};
	}

}

#endif