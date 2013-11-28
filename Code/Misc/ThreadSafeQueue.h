#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

////////////////////////////////////////////
// Thread safe queue implemented 
// with single linked list.      
// Created by Sam Svensson 2013  
/////////////////////////////////////////////

#include "IQueue.h" 
#include "Thread/OysterMutex.h"

namespace Oyster
{
	namespace Queue
	{
		template <typename Type>
		class ThreadSafeQueue : public IQueue<Type>
		{
		public:
			ThreadSafeQueue<Type>();
			virtual ~ThreadSafeQueue<Type>();

			virtual void Push( Type item );
			virtual Type Pop();

			virtual Type Front();
			virtual Type Back();

			virtual int Size();
			virtual bool IsEmpty();
			virtual void Swap( IQueue<Type> &queue );

		private:
			class Node
			{
			public:
				Type item;
				Node *next;
				Node(Type item){ this->item = item; this->next = NULL; };
				~Node() {};
			};

			Node *front;
			Node *back;
			int nrOfNodes;
			OysterMutex mutex; 
		};

		


		//----------------------------------------------
		//implemented template functions
		//----------------------------------------------

		template < typename Type >
		ThreadSafeQueue<Type>::ThreadSafeQueue()
		{
			this->front = NULL;
			this->back = NULL;
			this->nrOfNodes = 0;
			
		}

		template < typename Type >
		ThreadSafeQueue<Type>::~ThreadSafeQueue()
		{
			this->mutex.LockMutex();

			if(this->front != NULL)
			{
				Node *destroyer;
				Node *walker = this->front;
				
				for(int i = 0; i < this->nrOfNodes; i++)
				{
					destroyer = walker;
					walker = walker->next;
					
					delete destroyer;
				}

				this->front = NULL;
				this->back = NULL;
			}

			this->mutex.UnlockMutex();
		}


		template < typename Type >
		void ThreadSafeQueue<Type>::Push(Type item)
		{
			Node *e = new Node(item);

			mutex.LockMutex();
			if(this->front != NULL)
			{
				this->back->next = e;
				this->back = e;
			}

			else
			{
				this->front = e;
				this->back = e;
			}

			this->nrOfNodes++;

			mutex.UnlockMutex();
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Pop()
		{
			mutex.LockMutex();
			if(this->front != NULL)
			{
				Type item = this->front->item;
				Node *destroyer = this->front;
				this->front = front->next;
			
				delete destroyer;
				this->nrOfNodes--;

				if(nrOfNodes == 0)
				{
					this->front = NULL;
					this->back = NULL;
				}
				return item;
			}

			mutex.UnlockMutex();

			return NULL;
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Front()
		{
			mutex.LockMutex();
			if(front != NULL)
			{
				return this->front->item;
			}
			mutex.UnlockMutex();

			return NULL;
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Back()
		{
			mutex.LockMutex();
			if(back != NULL)
			{
				return this->back->item;
			}
			mutex.UnlockMutex();

			return NULL;
		}

		template < typename Type >
		int ThreadSafeQueue<Type>::Size()
		{
			//? beh�vs denna l�sas?
			mutex.LockMutex();
			return this->nrOfNodes;
			mutex.UnlockMutex();
		}

		template < typename Type >
		bool ThreadSafeQueue<Type>::IsEmpty()
		{
			mutex.LockMutex();
			if(nrOfNodes == 0 && this->front == NULL)
			{
				mutex.UnlockMutex();
				return true;
			}
			
			else
			{
				mutex.UnlockMutex();
			}
			return false;
		}

		template < typename Type >
		void ThreadSafeQueue<Type>::Swap(IQueue<Type> &queue )
		{
			mutex.LockMutex();
			int prevNrOfNodes = this->nrOfNodes;
			int size = queue.Size();

			for(int i = 0; i < size; i++)
			{
				this->Push(queue.Pop());
			}

			for(int i = 0; i < prevNrOfNodes; i++)
			{
				queue.Push(this->Pop());
			}
			mutex.UnlockMutex();
		}		


	}
}
#endif

