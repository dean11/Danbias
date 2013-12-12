/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "OysterThread.h"
#include "OysterMutex.h"
#include "..\Utilities.h"
#include <thread>
#include <assert.h>
#include <atomic>

using namespace Oyster::Thread;
using namespace Utility::DynamicMemory;


#pragma region Declerations

	struct ThreadData;
	/** A typical Oyster thread function */
	typedef void (*ThreadFunction)(SmartPointer<ThreadData>&);

	enum OYSTER_THREAD_STATE
	{
		OYSTER_THREAD_STATE_RESET,
		OYSTER_THREAD_STATE_RUNNING,
		OYSTER_THREAD_STATE_PAUSED,
		OYSTER_THREAD_STATE_STOPED,
		OYSTER_THREAD_STATE_DEAD,
	};


	struct ThreadData
	{
		std::atomic<OYSTER_THREAD_STATE>	state;			//<! The current thread state.
		SmartPointer<std::thread> workerThread;				//<! The worker thread.
		std::thread::id callingThread;						//<! The owner thread.
		IThreadObject *owner;								//<! The owner of the thread as IThread.
		std::atomic<int> msec;								//<! A timer in miliseconds.

		//OysterMutex mutexLock;							//<! The lock, locking the member variabls.
		//OYSTER_THREAD_STATE	state;						//<! The current thread state.
	
		ThreadData() {}
		~ThreadData() {}
		ThreadData(const ThreadData&)	
		{};
		const ThreadData& operator =(const ThreadData& o)	
		{};
	};
	struct OysterThread::PrivateData
	{
		SmartPointer<ThreadData> threadData;
	
		PrivateData()
			:threadData(new ThreadData())
		{
			threadData->owner = 0;
			threadData->workerThread = 0;
			threadData->callingThread;
			threadData->state = OYSTER_THREAD_STATE_STOPED;
		}
		PrivateData(const PrivateData& o)
		{
			threadData = o.threadData;
		}
		const PrivateData& operator=(const PrivateData& o)
		{
			threadData = o.threadData;
		}
		~PrivateData()
		{
			//@todo TODO: Make detatch avalible.
			//if(!this->threadData->workerThread->joinable())
			this->threadData->workerThread->detach();

			this->threadData->owner = 0;

			this->threadData->state = OYSTER_THREAD_STATE_DEAD;
		}

	};

#pragma endregion


static void ThreadingFunction(SmartPointer<ThreadData> &origin)
{

	bool shouldContinue;
	SmartPointer<ThreadData> w = origin;

theBegining:

	while(w->state == OYSTER_THREAD_STATE_STOPED)
	{
		std::this_thread::yield();
	}

//	w->mutexLock.LockMutex();
	if(w->owner)
	{
		w->owner->ThreadEntry();
	}
//	w->mutexLock.UnlockMutex();

	while (w->state != OYSTER_THREAD_STATE_STOPED && w->state != OYSTER_THREAD_STATE_DEAD)		
	{
//		w->mutexLock.LockMutex();
		{
			if(w->owner)
			{
				shouldContinue = w->owner->DoWork();
			}
		}
//		w->mutexLock.UnlockMutex();	

		if(!shouldContinue)
		{
			goto theEnd;
		}
		if(w->state == OYSTER_THREAD_STATE_DEAD)
		{
			goto theEnd;	
		}
		else if(w->state == OYSTER_THREAD_STATE_RESET)
		{
			goto theBegining;	
		}
		else if(w->msec > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(w->msec));
		}

		while (w->state == OYSTER_THREAD_STATE_PAUSED)
		{
			std::this_thread::yield();
		}
	}

	if(w->state == OYSTER_THREAD_STATE_DEAD)
	{
		w->workerThread->detach();
		return;
	}

theEnd:

//	w->mutexLock.LockMutex();	
	if(w->owner)
	{
		w->owner->ThreadExit();
	}
//	w->mutexLock.UnlockMutex();

	w->state = OYSTER_THREAD_STATE_DEAD;
}

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
	this->privateData = new PrivateData(*original.privateData);
	return *this;
}
OysterThread::~OysterThread()											   
{
	delete this->privateData;
	this->privateData = 0;
}

OYSTER_THREAD_ERROR OysterThread::Create(IThreadObject* worker, bool start)	  
{
	if(!this->privateData)							return OYSTER_THREAD_ERROR_FAILED;
	if(this->privateData->threadData->workerThread)	return OYSTER_THREAD_ERROR_FAILED;

	this->privateData->threadData->owner = worker;

	ThreadFunction fnc = ThreadingFunction;

	//Maby move this thread creation to a seperate Start() function because std::thread fires the thread when it is created. :(
	this->privateData->threadData->workerThread = new std::thread(fnc, this->privateData->threadData);

	if(!this->privateData->threadData->workerThread)
		return OYSTER_THREAD_ERROR_FAILED;

	if(start)
	{
		this->privateData->threadData->state = OYSTER_THREAD_STATE_RUNNING;
	}
	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Start()
{
	if(!this->privateData->threadData->owner)
		return OYSTER_THREAD_ERROR_FAILED;
	if(!this->privateData->threadData->workerThread)
		return OYSTER_THREAD_ERROR_FAILED;
	if(this->privateData->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_FAILED;

	this->privateData->threadData->state = OYSTER_THREAD_STATE_RUNNING;
	return OYSTER_THREAD_ERROR_SUCCESS;
}
void OysterThread::Stop()
{
	//this->privateData->threadData->mutexLock.LockMutex();
	this->privateData->threadData->state = OYSTER_THREAD_STATE_STOPED;
	//this->privateData->threadData->mutexLock.UnlockMutex();
}
void OysterThread::Pause()
{
	//this->privateData->threadData->mutexLock.LockMutex();
	this->privateData->threadData->state = OYSTER_THREAD_STATE_PAUSED;
	//this->privateData->threadData->mutexLock.UnlockMutex();
}
void OysterThread::Pause(int msec)
{

	if(std::this_thread::get_id() == this->privateData->threadData->workerThread->get_id())
	{
		this->privateData->threadData->msec = msec;
	}
	else
	{
		//this->privateData->threadData->mutexLock.LockMutex();
		this->privateData->threadData->state = OYSTER_THREAD_STATE_PAUSED;
		this->privateData->threadData->msec = msec;
		//this->privateData->threadData->mutexLock.UnlockMutex();
	}
}
void OysterThread::Resume()
{
//	this->privateData->threadData->mutexLock.LockMutex();
		this->privateData->threadData->state = OYSTER_THREAD_STATE_RUNNING;
//	this->privateData->threadData->mutexLock.UnlockMutex();
}
OYSTER_THREAD_ERROR OysterThread::Reset(IThreadObject* worker)
{
//	this->privateData->threadData->mutexLock.LockMutex();
	if(worker) 
	{
		this->privateData->threadData->owner = worker;
	}
		this->privateData->threadData->callingThread = std::this_thread::get_id();
		this->privateData->threadData->msec = 0;
//	this->privateData->threadData->mutexLock.UnlockMutex();

	return OYSTER_THREAD_ERROR_SUCCESS;
}
void OysterThread::Terminate()
{
	this->privateData->threadData->state = OYSTER_THREAD_STATE_DEAD;
}
void OysterThread::Wait()
{
	if(this->privateData->threadData->state == OYSTER_THREAD_STATE_DEAD)
	{
		return;
	}

	if(this->privateData->threadData->workerThread->get_id() == std::this_thread::get_id())	return;
	
	this->privateData->threadData->workerThread->join();
}
void OysterThread::Wait(int msec)
{
	if(this->privateData->threadData->workerThread->get_id() == std::this_thread::get_id())	return;

	std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}
OYSTER_THREAD_ERROR OysterThread::Swap(const OysterThread* other)
{
	this->privateData->threadData->workerThread->swap(*other->privateData->threadData->workerThread);
	return OYSTER_THREAD_ERROR_SUCCESS;
}
bool OysterThread::IsActive()
{
	if (this->privateData->threadData->state == OYSTER_THREAD_STATE_RUNNING)
		return true;

	return false;
}

