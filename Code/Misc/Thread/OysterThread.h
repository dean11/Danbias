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

		class OysterThread
		{
		private:
			struct PrivateData;
			PrivateData *privateData;

			OysterThread(const OysterThread& original);
			const OysterThread& operator=(const OysterThread& original);

		public:
			OysterThread();
			virtual~OysterThread();

			OYSTER_THREAD_ERROR Create(IThreadObject* worker, bool start);
			OYSTER_THREAD_ERROR Start();
			void Stop();
			void Pause();
			void Pause(int mSec);
			void Resume();
			OYSTER_THREAD_ERROR Reset(IThreadObject* worker = 0);
			void Terminate();
			void Wait();
			void Wait(int mSec);
			OYSTER_THREAD_ERROR Swap(const OysterThread* other);
			bool IsActive();
		};
	} 
}

#endif // !MISC_I_OYSTER_THREAD_H
