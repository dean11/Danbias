/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_OYSTER_THREAD_H
#define MISC_OYSTER_THREAD_H

#include "IThreadObject.h"

namespace Oyster 
{ 
	namespace Thread
	{
		/**
		*	Inherit this class to get threading compatibility.
		*/
		class IThreadObject
		{
		public:
			enum ThreadCode
			{
				ThreadCode_Exit,
				ThreadCode_ResetNormal,
				ThreadCode_ResetIdle,
			};

		public:
			/**
			*	Override this to get notified when the thread is started.
			*/
			virtual void ThreadEntry()	{ }
			/**
			*	Override this to get notified when the thread is about to exit.
			*/
			virtual ThreadCode ThreadExit()	{ return ThreadCode_Exit; }
			/**
			*	This function is required to get threading working. 
			*	Note that this function is NOT thread safe.
			*	OBS! Do not highjack the looping.
			*/
			virtual bool DoWork			( ) = 0;
		};
		typedef bool (*ThreadFnc)(void);

		enum OYSTER_THREAD_ERROR
		{
			OYSTER_THREAD_ERROR_SUCCESS,
			OYSTER_THREAD_ERROR_ThreadAlreadyCreated,
			OYSTER_THREAD_ERROR_ThreadCreationFailed,
			OYSTER_THREAD_ERROR_ThreadNotCreated,
			OYSTER_THREAD_ERROR_ThreadHasNoWorker,
			OYSTER_THREAD_ERROR_ThreadCannotWaintOnItselfe,
			OYSTER_THREAD_ERROR_ThreadCannotBeWaitedOn,
			OYSTER_THREAD_ERROR_ThreadIsDead,

			OYSTER_THREAD_ERROR_FAILED,
		};
		enum OYSTER_THREAD_PRIORITY
		{
			OYSTER_THREAD_PRIORITY_0,	//!< Realtime
			OYSTER_THREAD_PRIORITY_1,	//!< High
			OYSTER_THREAD_PRIORITY_2,	//!< Medium
			OYSTER_THREAD_PRIORITY_3,	//!< Low
		};

		class OysterThread
		{
		private:
			struct PrivateData;
			PrivateData *privateData;

		public:
			OysterThread();
			OysterThread(const OysterThread& original);
			const OysterThread& operator=(const OysterThread& original);
			virtual~OysterThread();

			OYSTER_THREAD_ERROR Create(IThreadObject* worker, bool start, bool detach = false);
			OYSTER_THREAD_ERROR Create(ThreadFnc worker, bool start, bool detach = false);
			//OYSTER_THREAD_ERROR Create(Oyster::Callback::CallbackObject<bool, void>* worker, bool start, bool detach = false);
			//OYSTER_THREAD_ERROR Create(Oyster::Callback::CallbackFunction<bool, void>::FNC worker, bool start, bool detach = false);
			OYSTER_THREAD_ERROR Start();
			OYSTER_THREAD_ERROR Stop();
			OYSTER_THREAD_ERROR Stop(int msec);
			OYSTER_THREAD_ERROR Resume();
			OYSTER_THREAD_ERROR SetWorker(IThreadObject* worker = 0);
			OYSTER_THREAD_ERROR SetWorker(ThreadFnc worker = 0);
			OYSTER_THREAD_ERROR Terminate();
			OYSTER_THREAD_ERROR Wait();
			OYSTER_THREAD_ERROR Wait(int mSec);
			OYSTER_THREAD_ERROR Swap(const OysterThread* other);

			void SetPriority(OYSTER_THREAD_PRIORITY priority);
			bool IsActive();
			bool IsCreated() const;

		};
	} 
}

#endif // !MISC_OYSTER_THREAD_H
