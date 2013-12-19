/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "OysterThread.h"
#include "..\Utilities.h"
#include <thread>
#include <assert.h>
#include <atomic>
#include <future>

using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;




#pragma region Declerations

	enum OYSTER_THREAD_STATE
	{
		OYSTER_THREAD_STATE_IDLE,
		OYSTER_THREAD_STATE_NORMAL,
		OYSTER_THREAD_STATE_DEAD,
	};

	struct ThreadData
	{
		OYSTER_THREAD_STATE	state;							//<! The current thread state.
		OYSTER_THREAD_PRIORITY prio;						//<! The thread priority
		IThreadObject *owner;									//<! The worker.
		std::atomic<int> msec;								//<! A timer in miliseconds.

		//std::timed_mutex threadFunctionLock;
		//std::mutex threadWaitFunctionLock;
	};

	/** A typical Oyster thread function */
	typedef void (*ThreadFunction)(ThreadData* w);

	struct RefData
	{
		bool isCreated;
		ThreadData *threadData;
		std::thread workerThread;

		RefData()   
		{ 
			threadData = 0; 
			isCreated = false; 
		}
		~RefData()  
		{ 
			Terminate(true); 
			delete threadData;
		}
		OYSTER_THREAD_ERROR Terminate(bool wait)
		{
			if(!threadData) return OYSTER_THREAD_ERROR_SUCCESS;

			this->threadData->state = OYSTER_THREAD_STATE_DEAD;
			
			if(wait)
			{
				if(std::this_thread::get_id() != this->workerThread.get_id())
					if(this->workerThread.joinable())
						this->workerThread.join();
			}
			else
			{
				if(this->workerThread.joinable())
					this->workerThread.detach();
			}
			return OYSTER_THREAD_ERROR_SUCCESS;
		}
		OYSTER_THREAD_ERROR Create(ThreadFunction fnc, IThreadObject* worker, bool start, bool detach)
		{
			if(this->isCreated )			return OYSTER_THREAD_ERROR_ThreadAlreadyCreated;

			threadData = new ThreadData();
			if(start)	
				this->threadData->state = OYSTER_THREAD_STATE_NORMAL;
			else		
				this->threadData->state = OYSTER_THREAD_STATE_IDLE;
			threadData->owner = worker;
			threadData->prio = OYSTER_THREAD_PRIORITY_3;

			workerThread = std::thread(fnc, this->threadData);

			if(detach) 
				this->workerThread.detach();

			isCreated = true;

			return OYSTER_THREAD_ERROR_SUCCESS;
		}
	};
	struct OysterThread::PrivateData
	{
		SmartPointer<RefData> data;

		OYSTER_THREAD_ERROR Create(ThreadFunction fnc, IThreadObject* worker, bool start, bool detach)
		{ 
			if(data) return  OYSTER_THREAD_ERROR_ThreadAlreadyCreated; 
			data = new RefData();
			return data->Create(fnc, worker, start, detach);
		}
		OYSTER_THREAD_ERROR Terminate(bool wait)
		{
			return data->Terminate(wait);
		}
	};

	class ThreadHelp
	{
	public:
		static void CheckPriority(ThreadData* w)
		{
			switch (w->prio)
			{
				case Oyster::Thread::OYSTER_THREAD_PRIORITY_1:
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				break;
				case Oyster::Thread::OYSTER_THREAD_PRIORITY_2:
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				break;
				case Oyster::Thread::OYSTER_THREAD_PRIORITY_3:
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				break;
			}
		}
		static bool DoWork(ThreadData* w)
		{
			if(w->owner)
				return w->owner->DoWork();
				
			return true;
		}
		static void CheckStatus(ThreadData* w)
		{
			if(w->msec > 0)
				std::this_thread::sleep_for(std::chrono::milliseconds(w->msec));
	
			while (w->state == OYSTER_THREAD_STATE_IDLE)	
				std::this_thread::yield();
		}
		static void ThreadingFunction(ThreadData* w)
		{
			CheckStatus(w);

			if(w->owner)	w->owner->ThreadEntry();

			while (w->state == OYSTER_THREAD_STATE_NORMAL)		
			{
				CheckPriority(w);
				if(!DoWork(w)) break;
				CheckStatus(w);
			}

			if(w->owner)	w->owner->ThreadExit();

			w->state = OYSTER_THREAD_STATE_DEAD;
		}
	};

#pragma endregion


OysterThread::OysterThread()
{
	this->privateData = new PrivateData();
}
OysterThread::OysterThread(const OysterThread& original)
{
	this->privateData = new PrivateData(*original.privateData);
}
const OysterThread& OysterThread::operator=(const OysterThread& original)
{
	delete this->privateData;
	this->privateData = new PrivateData();
	this->privateData->data = original.privateData->data;
	return *this;
}
OysterThread::~OysterThread()											   
{
	delete this->privateData;
	this->privateData = 0;
}

OYSTER_THREAD_ERROR OysterThread::Create(IThreadObject* worker, bool start, bool detach)	  
{
	if(!this->privateData)
		this->privateData = new PrivateData();
	
	return this->privateData->Create(ThreadHelp::ThreadingFunction, worker, start, detach);
}
OYSTER_THREAD_ERROR OysterThread::Start()
{
	if(!this->privateData->data->threadData->owner)
		return OYSTER_THREAD_ERROR_ThreadHasNoWorker;

	if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_ThreadIsDead;

	this->privateData->data->threadData->state = OYSTER_THREAD_STATE_NORMAL;

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Stop(bool wait)
{
	return this->Terminate(wait);
}
OYSTER_THREAD_ERROR OysterThread::Pause()
{
	this->privateData->data->threadData->state = OYSTER_THREAD_STATE_IDLE;
	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Pause(int msec)
{
	this->privateData->data->threadData->msec = msec;
	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Resume()
{
	if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_ThreadIsDead;

	this->privateData->data->threadData->state = OYSTER_THREAD_STATE_NORMAL;

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Reset(IThreadObject* worker)
{
	this->privateData->data->threadData->owner = worker; 

	this->privateData->data->threadData->msec = 0;

	return OYSTER_THREAD_ERROR_SUCCESS;;
}
OYSTER_THREAD_ERROR OysterThread::Terminate(bool wait)
{
	//this->privateData->data->threadData->state = OYSTER_THREAD_STATE_DEAD;
	//
	//if(std::this_thread::get_id() == this->privateData->data->workerThread->get_id())
	//	return OYSTER_THREAD_ERROR_SUCCESS;
	//
	//if(wait)
	//{
	//	if(this->privateData->data->workerThread->joinable())
	//		this->privateData->data->workerThread->detach();
	//
	//	this->privateData->data->threadData->threadFunctionLock.lock();
	//	this->privateData->data->threadData->threadFunctionLock.unlock();
	//}

	return this->privateData->Terminate(wait);

	//return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Wait()
{
	if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_ThreadIsDead;

	if(	this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
	  return OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;
	
	//this->privateData->data->threadData->threadFunctionLock.lock();
	//this->privateData->data->threadData->threadFunctionLock.unlock();

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Wait(int msec)
{
	if(this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
		return OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;

	//if(this->privateData->data->threadData->threadFunctionLock.try_lock_for(std::chrono::milliseconds(msec)))
	//	this->privateData->data->threadData->threadFunctionLock.unlock();
	
	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Swap(const OysterThread* other)
{
	this->privateData->data->workerThread.swap(other->privateData->data->workerThread);
	return OYSTER_THREAD_ERROR_SUCCESS;
}

void OysterThread::SetPriority(OYSTER_THREAD_PRIORITY priority)
{
	this->privateData->data->threadData->prio = priority;
}
bool OysterThread::IsActive()
{
	if (this->privateData->data->threadData->state == OYSTER_THREAD_STATE_NORMAL)
		return true;

	return false;
}
bool OysterThread::IsCreated() const
{
	return privateData->data->isCreated;
}
