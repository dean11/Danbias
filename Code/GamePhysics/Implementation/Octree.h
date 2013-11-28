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
				OctreeNode* children[8];
				Data* dataPtr;
				Collision3D::BoxAxisAligned container;
			};

			Octree(unsigned int bufferSize, unsigned char numLayers, Math::Float3 worldSize);
			virtual ~Octree();

			void AddObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void MoveToUpdateQueue(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void DestroyObject(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			std::vector<ICustomBody*> Sample(Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);
		private:
			std::vector < Data > leafData;
			std::vector < Data* > updateQueue;

			std::map< ICustomBody*, unsigned int > mapReferences;

			OctreeNode worldNode;
		};
	}

}

#endif