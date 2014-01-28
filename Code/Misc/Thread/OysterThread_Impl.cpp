/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#include "OysterThread.h"
#include "..\Utilities.h"
#include "..\OysterCallback.h"

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
	struct OwnerContainer
	{
		Oyster::Callback::CallbackType type;
		union OwnerValue
		{
			IThreadObject*	obj;
			ThreadFnc		fnc;
			OwnerValue()					{ memset(this, 0, sizeof(OwnerValue)); }
			OwnerValue(IThreadObject* v)	{ obj = v; }
			OwnerValue(ThreadFnc v)			{ fnc = v; }
		} value;

		operator bool()
		{
			return (value.fnc ||value.obj);
		}
	};
	struct ThreadData
	{
		OYSTER_THREAD_STATE	state;									//<! The current thread state.
		OYSTER_THREAD_PRIORITY prio;								//<! The thread priority
		//IThreadObject *owner;										//<! The worker object.
		//Oyster::Callback::OysterCallback<bool, void> ownerObj;	//
		OwnerContainer ownerObj;									//
		std::atomic<int> msec;										//<! A timer in miliseconds.

		//std::timed_mutex threadFunctionLock;
		std::mutex threadStopMutex;
	};
	
	/** A typical Oyster thread function */
	typedef void (*ThreadFunction)(ThreadData* w);

	struct RefData
	{
		std::mutex threadWaitFunctionLock;
		bool isCreated;
		bool isAlive;
		ThreadData *threadData;
		std::thread workerThread;

		RefData()   
		{ 
			threadData = 0; 
			isCreated = false; 
		}
		~RefData()  
		{ 
			//threadWaitFunctionLock.lock();
				Terminate(); 
			//threadWaitFunctionLock.unlock();
		}
		OYSTER_THREAD_ERROR Terminate()
		{
			if(!threadData) return OYSTER_THREAD_ERROR_SUCCESS;

			this->threadData->state = OYSTER_THREAD_STATE_DEAD;
			
			if(this->workerThread.joinable())
			{
				this->workerThread.join();
				this->isCreated = false;
				delete this->threadData;
				this->threadData = 0;
			}
			
			return OYSTER_THREAD_ERROR_SUCCESS;
		}
		OYSTER_THREAD_ERROR Create(ThreadFunction fnc, OwnerContainer worker, bool start, bool detach)
		{
			if(this->isCreated )			return OYSTER_THREAD_ERROR_ThreadAlreadyCreated;
			
			threadData = new ThreadData();
			if(start)	
				this->threadData->state = OYSTER_THREAD_STATE_NORMAL;
			else		
				this->threadData->state = OYSTER_THREAD_STATE_IDLE;
			threadData->ownerObj = worker;
			threadData->prio = OYSTER_THREAD_PRIORITY_2;

			workerThread = std::thread(fnc, this->threadData);

			//if(detach) 
			//	this->workerThread.detach();

			isCreated = true;

			return OYSTER_THREAD_ERROR_SUCCESS;
		}
	};
	
	struct OysterThread::PrivateData
	{
		SmartPointer<RefData> data;

		PrivateData(){}
		~PrivateData()
		{ 
			data.Release();
		}
		OYSTER_THREAD_ERROR Create(ThreadFunction fnc, OwnerContainer worker, bool start, bool detach)
		{ 
			if(data) return  OYSTER_THREAD_ERROR_ThreadAlreadyCreated; 
			data = new RefData();
			return data->Create(fnc, worker, start, detach);
		}
		OYSTER_THREAD_ERROR Terminate()
		{
			if(!data) 
				return OYSTER_THREAD_ERROR_FAILED;

			return data->Terminate();
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
			switch (w->ownerObj.type)
			{
				case Oyster::Callback::CallbackType_Function:
					if(w->ownerObj.value.fnc) return w->ownerObj.value.fnc();
				break;
				
				case Oyster::Callback::CallbackType_Object:
					if(w->ownerObj.value.obj) return w->ownerObj.value.obj->DoWork();
				break;
			}
			

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

			if(w->ownerObj.value.obj)	w->ownerObj.value.obj->ThreadEntry();

			while (w->state == OYSTER_THREAD_STATE_NORMAL)		
			{
				CheckPriority(w);
				if(!DoWork(w)) break;
				CheckStatus(w);
			}

			if(w->ownerObj.value.obj)	w->ownerObj.value.obj->ThreadExit();

			w->state = OYSTER_THREAD_STATE_DEAD;

			//delete w;
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
	if(!this->privateData) this->privateData = new PrivateData();

	if(this->privateData->data->isCreated) return OYSTER_THREAD_ERROR_ThreadAlreadyCreated;
	OwnerContainer c;
	c.type = Oyster::Callback::CallbackType_Object;
	c.value = worker;
	return this->privateData->Create(ThreadHelp::ThreadingFunction, c, start, detach);
}
OYSTER_THREAD_ERROR OysterThread::Create(ThreadFnc worker, bool start, bool detach)	  
{
	if(!this->privateData) this->privateData = new PrivateData();

	OwnerContainer c;
	c.type = Oyster::Callback::CallbackType_Function;
	c.value = worker;
	return this->privateData->Create(ThreadHelp::ThreadingFunction, c, start, detach);
}

OYSTER_THREAD_ERROR OysterThread::Start()
{
	if(!this->privateData->data->threadData->ownerObj)
		return OYSTER_THREAD_ERROR_ThreadHasNoWorker;

	if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_ThreadIsDead;

	this->privateData->data->threadData->state = OYSTER_THREAD_STATE_NORMAL;

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Stop()
{
	this->privateData->data->threadData->state = OYSTER_THREAD_STATE_IDLE;
	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Stop(int msec)
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
OYSTER_THREAD_ERROR OysterThread::SetWorker(IThreadObject* worker)
{
	this->privateData->data->threadData->ownerObj.value = worker; 
	this->privateData->data->threadData->ownerObj.type = Oyster::Callback::CallbackType_Object;

	this->privateData->data->threadData->msec = 0;

	return OYSTER_THREAD_ERROR_SUCCESS;;
}
OYSTER_THREAD_ERROR OysterThread::SetWorker(ThreadFnc worker)
{
	this->privateData->data->threadData->ownerObj.value = worker; 
	this->privateData->data->threadData->ownerObj.type = Oyster::Callback::CallbackType_Function; 

	this->privateData->data->threadData->msec = 0;

	return OYSTER_THREAD_ERROR_SUCCESS;;
}
OYSTER_THREAD_ERROR OysterThread::Terminate()
{
	return this->privateData->Terminate();
}
OYSTER_THREAD_ERROR OysterThread::Wait()
{
	if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
		return OYSTER_THREAD_ERROR_ThreadIsDead;

	if(	this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
	  return OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Wait(int msec)
{
	if(this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
		return OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;
	
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
	if(!privateData->data) return false;

	return privateData->data->isCreated;
}
