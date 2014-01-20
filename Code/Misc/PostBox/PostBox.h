#ifndef NETWORK_DEPENDENCIES_POST_BOX_H
#define NETWORK_DEPENDENCIES_POST_BOX_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "IPostBox.h"
#include "../ThreadSafeQueue.h"

namespace Oyster
{
	//With this class you can post items to it and then fetch them somewhere else.
	//It is thread safe beacause of the ThreadSafeQueue.
	template <class T>
	class PostBox : public IPostBox<T>
	{
	public:
		PostBox();
		virtual ~PostBox();

		virtual void Post(T message);
		virtual T Fetch();
		virtual bool IsEmpty();

	private:
		Utility::Container::ThreadSafeQueue<T> messages;
		//Utility::Container::ThreadSafeQueue<T> messages;

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
	void PostBox<T>::Post(T message)
	{
		messages.Push(message);
	}

	template <class T>
	T PostBox<T>::Fetch()
	{
		return messages.Pop();
	}

	template <class T>
	bool PostBox<T>::IsEmpty()
	{
		return messages.IsEmpty();
	}
}

#endif