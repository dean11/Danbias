/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_I_THREAD_OBJECT_H
#define MISC_I_THREAD_OBJECT_H



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
			/**
			*	Override this to get notified when the thread is started.
			*/
			virtual void ThreadEntry()	{ }
			/**
			*	Override this to get notified when the thread is about to exit.
			*/
			virtual void ThreadExit()	{ }
			/**
			*	This function is required to get threading working. 
			*	Note that this function is NOT thread safe.
			*	OBS! Do not highjack the looping.
			*/
			virtual bool DoWork			( ) = 0;
		};
	}
}

#endif // !MISC_I_THREAD_OBJECT_H

