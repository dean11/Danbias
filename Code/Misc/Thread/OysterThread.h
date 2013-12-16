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
			OYSTER_THREAD_ERROR_FAILED,
			OYSTER_THREAD_ERROR_SUCCESS,
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

			OYSTER_THREAD_ERROR Create(IThreadObject* worker, bool start);
			OYSTER_THREAD_ERROR Start();
			void Stop(bool wait = false);
			void Pause();
			void Pause(int mSec);
			void Resume();
			OYSTER_THREAD_ERROR Reset(IThreadObject* worker = 0);
			void Terminate(bool wait = false);
			void Wait();
			void Wait(int mSec);
			OYSTER_THREAD_ERROR Swap(const OysterThread* other);
			bool IsActive();
			void SetPriority(OYSTER_THREAD_PRIORITY priority);

			bool IsCreated() const;
		};
	} 
}

#endif // !MISC_OYSTER_THREAD_H
