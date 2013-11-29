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
			static const unsigned int invalid_ref;

			typedef void(*VistorAction)(Octree&, unsigned int, unsigned int);

			struct Data
			{
				Data* prev;
				Data* next;

				Collision3D::Sphere container;

				::Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef;

				unsigned int queueRef;
			};

			struct OctreeNode
			{
				OctreeNode* children[8];
				Data* dataPtr;
				Collision3D::BoxAxisAligned container;
			};

			Octree(unsigned int bufferSize = 0, unsigned char numLayers = 0, Math::Float3 worldSize = Math::Float3::null);
			virtual ~Octree();

			Octree& operator=(const Octree& orig);

			void AddObject(::Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void MoveToUpdateQueue(::Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			void DestroyObject(::Utility::DynamicMemory::UniquePointer< ICustomBody > customBodyRef);

			std::vector<ICustomBody*>& Sample(ICustomBody* customBodyRef, std::vector<ICustomBody*>& updateList);
			std::vector<ICustomBody*>& Sample(Oyster::Collision3D::ICollideable* collideable, std::vector<ICustomBody*>& updateList);
			void Visit(ICustomBody* customBodyRef, VistorAction hitAction );
			void Visit(Oyster::Collision3D::ICollideable* collideable, VistorAction hitAction );

			ICustomBody* GetCustomBody(const unsigned int tempRef);

			::Utility::DynamicMemory::UniquePointer<ICustomBody> Extract( const ICustomBody* objRef );
			::Utility::DynamicMemory::UniquePointer<ICustomBody> Extract( unsigned int tempRef ); // Dan vill ha
			unsigned int GetTemporaryReferenceOf( const ICustomBody* objRef ) const; // Dan vill ha
			void SetAsAltered( unsigned int tempRef ); // Dan vill ha
			void EvaluatePosition( unsigned int tempRef ); // Dan vill ha

		private:
			std::vector < Data > leafData;
			std::vector < Data* > updateQueue;

			std::map< const ICustomBody*, unsigned int > mapReferences;

			OctreeNode worldNode;
		};
	}

}

#endif