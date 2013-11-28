#ifndef NETWORK_DEPENDENCIES_POST_BOX_H
#define NETWORK_DEPENDENCIES_POST_BOX_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IPostBox.h"
#include <queue>
#include "../../Misc/Thread/OysterMutex.h"

namespace Oyster
{
	namespace Network
	{
		template <class T>
		class PostBox : public IPostBox<T>
		{
		public:
			PostBox();
			virtual ~PostBox();

			virtual void PostMessage(T& message);
			virtual void FetchMessage(T& message);
			virtual bool IsFull();

		private:
			std::queue<T> messages;
			OysterMutex mutex;

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
			mutex.LockMutex();
			messages.push(message);
			mutex.UnlockMutex();
		}

		template <class T>
		void PostBox<T>::FetchMessage(T& message)
		{
			mutex.LockMutex();
			if(IsFull())
			{
				message = messages.front();
				messages.pop();
			}
			mutex.UnlockMutex();
		}

		template <class T>
		bool PostBox<T>::IsFull()
		{
			return !messages.empty();
		}
	}
}

#endif