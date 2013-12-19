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
			OYSTER_THREAD_ERROR Start();
			OYSTER_THREAD_ERROR Stop(bool wait = false);
			OYSTER_THREAD_ERROR Pause();
			OYSTER_THREAD_ERROR Pause(int mSec);
			OYSTER_THREAD_ERROR Resume();
			OYSTER_THREAD_ERROR Reset(IThreadObject* worker = 0);
			OYSTER_THREAD_ERROR Terminate(bool wait = true);
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
