#ifndef NETWORK_DEPENDENCIES_POST_BOX_H
#define NETWORK_DEPENDENCIES_POST_BOX_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IPostBox.h"
#include "../../Misc/Thread/OysterMutex.h"
#include "../../Misc/ThreadSafeQueue.h"

namespace Oyster
{
	namespace Network
	{
		//With this class you can post items to it and then fetch them somewhere else.
		//It is thread safe beacause of the ThreadSafeQueue.
		template <class T>
		class PostBox : public IPostBox<T>
		{
		public:
			PostBox();
			virtual ~PostBox();

			virtual void PostMessage(T& message);
			virtual bool FetchMessage(T& message);
			virtual bool IsFull();

		private:
			Oyster::Queue::ThreadSafeQueue<T> messages;

		};

		//Implementation of PostBox
		template <class T>
		PostBox<T>::PostBox()
		{
		}

		template <class T>
		PostBox<T>::~PostBox()
		{
		}

		template <class T>
		void PostBox<T>::PostMessage(T& message)
		{
			messages.Push(message);
		}

		template <class T>
		bool PostBox<T>::FetchMessage(T& message)
		{
			if(IsFull())
			{
				message = messages.Pop(); 
				return true;
			}
			return false;
		}

		template <class T>
		bool PostBox<T>::IsFull()
		{
			return !messages.IsEmpty();
		}
	}
}

#endif