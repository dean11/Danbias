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
		OYSTER_THREAD_STATE	state;		//<! The current thread state.
		OYSTER_THREAD_PRIORITY prio;	//<! The thread priority
		OwnerContainer ownerObj;		//
		int msec;						//<! A timer in miliseconds.
	};
	
	/** A typical Oyster thread function */
	typedef void (*ThreadFunction)(ThreadData* w);

	struct RefData
	{
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

			if(std::this_thread::get_id() != this->workerThread.get_id())
			{
				//this->threadData->threadDataAcces.lock();
				//{
					this->threadData->state = OYSTER_THREAD_STATE_DEAD;
			
					if(this->workerThread.joinable())
					{
						this->workerThread.join();
					}

					this->isCreated = false;
					delete this->threadData;
					this->threadData = 0;
				//} this->threadData->threadDataAcces.unlock();
			}
			else
			{
				this->threadData->state = OYSTER_THREAD_STATE_DEAD;
				if(this->workerThread.joinable())
				{
					this->workerThread.join();
				}

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
			threadData->msec = 0;

			workerThread = std::thread(fnc, this->threadData);

			isCreated = true;

			return OYSTER_THREAD_ERROR_SUCCESS;
		}
	};
	
	struct OysterThread::PrivateData
	{
		SmartPointer<RefData> data;

		PrivateData()
		{
			data = new RefData();
		}
		~PrivateData()
		{ 
			data = 0;
		}
		OYSTER_THREAD_ERROR Create(ThreadFunction fnc, OwnerContainer worker, bool start, bool detach)
		{ 
			if(!data)	data = new RefData();
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
			
				Oyster::Thread::OYSTER_THREAD_PRIORITY temp = w->prio;

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
			{
				CheckPriority(w);
				std::this_thread::yield();
			}
		}
		static void ThreadingFunction(ThreadData* w)
		{
START:
			CheckStatus(w);
			if(w->ownerObj.value.obj)	w->ownerObj.value.obj->ThreadEntry();

			while (w->state == OYSTER_THREAD_STATE_NORMAL)		
			{
				//while (!w->threadDataAcces.try_lock());

					CheckPriority(w);
					if(!DoWork(w)) break;
					CheckStatus(w);

				//w->threadDataAcces.unlock();
			}

			IThreadObject::ThreadCode code = IThreadObject::ThreadCode_Exit;
			if(w->ownerObj.value.obj)	code = w->ownerObj.value.obj->ThreadExit();

			switch (code)
			{
				case Oyster::Thread::IThreadObject::ThreadCode_ResetNormal:
					w->msec = 0.0f;
					goto START;
				break;
				case Oyster::Thread::IThreadObject::ThreadCode_ResetIdle:
					w->state = OYSTER_THREAD_STATE_IDLE;
					w->msec = 0.0f;
					goto START;
				break;
			}


			w->state = OYSTER_THREAD_STATE_DEAD;
		}
	};

#pragma endregion


OysterThread::OysterThread()
	:privateData(0)
{ }
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
	if(!this->privateData)					this->privateData = new PrivateData();

	OwnerContainer c;
	c.type = Oyster::Callback::CallbackType_Object;
	c.value = worker;
	return this->privateData->Create(ThreadHelp::ThreadingFunction, c, start, detach);
}
OYSTER_THREAD_ERROR OysterThread::Create(ThreadFnc worker, bool start, bool detach)	  
{
	if(!this->privateData)					this->privateData = new PrivateData();

	OwnerContainer c;
	c.type = Oyster::Callback::CallbackType_Function;
	c.value = worker;
	return this->privateData->Create(ThreadHelp::ThreadingFunction, c, start, detach);
}

OYSTER_THREAD_ERROR OysterThread::Start()
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock();{

		if(!this->privateData->data->threadData->ownerObj)
			val = OYSTER_THREAD_ERROR_ThreadHasNoWorker;

		if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
			val = OYSTER_THREAD_ERROR_ThreadIsDead;

		this->privateData->data->threadData->state = OYSTER_THREAD_STATE_NORMAL;

	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;
}
OYSTER_THREAD_ERROR OysterThread::Stop()
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock(); {
		this->privateData->data->threadData->state = OYSTER_THREAD_STATE_IDLE;
	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;
}
OYSTER_THREAD_ERROR OysterThread::Stop(int msec)
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock(); {
		this->privateData->data->threadData->msec = msec;
	//} this->privateData->data->threadData->threadDataAcces.unlock();
	return val;
}
OYSTER_THREAD_ERROR OysterThread::Resume()
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock(); {
		if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
			val = OYSTER_THREAD_ERROR_ThreadIsDead;

		this->privateData->data->threadData->state = OYSTER_THREAD_STATE_NORMAL;
	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;
}
OYSTER_THREAD_ERROR OysterThread::SetWorker(IThreadObject* worker)
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock();{

		this->privateData->data->threadData->ownerObj.value = worker; 
		this->privateData->data->threadData->ownerObj.type = Oyster::Callback::CallbackType_Object;

		this->privateData->data->threadData->msec = 0;
	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;;
}
OYSTER_THREAD_ERROR OysterThread::SetWorker(ThreadFnc worker)
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;
	//this->privateData->data->threadData->threadDataAcces.lock();{

		this->privateData->data->threadData->ownerObj.value = worker; 
		this->privateData->data->threadData->ownerObj.type = Oyster::Callback::CallbackType_Function; 

		this->privateData->data->threadData->msec = 0;
	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;;
}
OYSTER_THREAD_ERROR OysterThread::Terminate()
{
	if(this->privateData)
		return this->privateData->Terminate();

	return OYSTER_THREAD_ERROR_SUCCESS;
}
OYSTER_THREAD_ERROR OysterThread::Wait()
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;

	//this->privateData->data->threadData->threadDataAcces.lock();{

		if(this->privateData->data->threadData->state == OYSTER_THREAD_STATE_DEAD)
			val = OYSTER_THREAD_ERROR_ThreadIsDead;

		if(	this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
			val = OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;

	//} this->privateData->data->threadData->threadDataAcces.unlock();

	return val;
}
OYSTER_THREAD_ERROR OysterThread::Wait(int msec)
{
	OYSTER_THREAD_ERROR val = OYSTER_THREAD_ERROR_SUCCESS;

	//this->privateData->data->threadData->threadDataAcces.lock();{
		if(this->privateData->data->workerThread.get_id() == std::this_thread::get_id())	
			val = OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe;
	//} this->privateData->data->threadData->threadDataAcces.unlock();
	return val;
}
OYSTER_THREAD_ERROR OysterThread::Swap(const OysterThread* other)
{
	//this->privateData->data->threadData->threadDataAcces.lock();{
		this->privateData->data->workerThread.swap(other->privateData->data->workerThread);
	//} this->privateData->data->threadData->threadDataAcces.unlock();
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
