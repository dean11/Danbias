#ifndef INCLUDE_GUARD_OYSTER_THREAD_H
#define INCLUDE_GUARD_OYSTER_THREAD_H

#include "IOysterThread.h"
#include <thread>

namespace Oyster 
{ 
	namespace Thread
	{
		//class OysterThread :public IOysterThread
		//{
		//private:
		//	std::thread thread;			//<! The worker thread.
		//	IWorkerThread *owner;		//<! The owner of the thread as IThread
		//
		//public:
		//	bool Wait();
		//	bool Suspend();
		//	void Resume();
		//	bool Kill();
		//	bool IsActive();
		//	bool Reset();
		//	bool StartThread()
		//	{
		//		this->owner->ThreadEntry();
		//
		//		while (true)
		//		{
		//			this->owner->DoWork();
		//		}
		//
		//		this->owner->ThreadExit();
		//	}
		//};
	}
}



#endif // !INCLUDE_GUARD_OYSTER_THREAD_H