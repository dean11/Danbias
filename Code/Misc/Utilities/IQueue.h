#ifndef MISC_I_QUEUE_H
#define MISC_I_QUEUE_H

/////////////////////////////////
// Created by Sam Svensson 2013//
/////////////////////////////////

namespace Utility
{
	namespace Container
	{
		template <typename Type>
		class IQueue
		{
		public:

			//---------------------------------------------
			//standard operations of the std::queue
			//---------------------------------------------
			virtual ~IQueue() {};
			virtual void Push( Type item )				= 0;
			virtual Type Pop()							= 0;
			virtual Type Front()						= 0;
			virtual Type Back()							= 0;
			virtual int Size()							= 0;
			virtual bool IsEmpty()						= 0;
			virtual void Swap( IQueue<Type> &queue )	= 0;
		};
	}
}

#endif