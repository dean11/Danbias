#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

////////////////////////////////////////////
// Thread safe queue implemented 
// with single linked list and template.
// uses mutex to lock the queue
// otherwise its a standard queue
// Created by Sam Svensson 2013  
/////////////////////////////////////////////

#include "IQueue.h" 
#include <mutex>

namespace Utility
{
	namespace Container
	{
		template <typename Type>
		class ThreadSafeQueue : public IQueue<Type>
		{
		public:
			ThreadSafeQueue<Type>();
			ThreadSafeQueue<Type>(const ThreadSafeQueue<Type>& obj);
			virtual ~ThreadSafeQueue<Type>();

			virtual void Push( Type item );
			virtual Type Pop();

			virtual Type Front();
			virtual Type Back();

			virtual int Size();
			virtual bool IsEmpty();
			virtual void Swap( IQueue<Type> &queue );

			virtual void Clear();

		private:
			class Node
			{
			public:
				Type item;
				Node *next;
				Node(){ this->next = NULL; };
				Node(Type item){ this->item = item; this->next = NULL; };
				~Node() {};
			};

			Node *front;
			Node *back;
			int nrOfNodes;
			std::mutex stdMutex;
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
		ThreadSafeQueue<Type>::ThreadSafeQueue(const ThreadSafeQueue<Type>& obj)
		{
			
		}

		template < typename Type >
		ThreadSafeQueue<Type>::~ThreadSafeQueue()
		{
			if(!nrOfNodes) return;

			stdMutex.lock();

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

			stdMutex.unlock();
		}


		template < typename Type >
		void ThreadSafeQueue<Type>::Push(Type item)
		{
			stdMutex.lock();
			Node *e = new Node(item);

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

			stdMutex.unlock();
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Pop()
		{
			stdMutex.lock();
			
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

			stdMutex.unlock();
			return item;
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Front()
		{
			stdMutex.lock();
			Type temp = this->front->item;
			stdMutex.unlock();
			
			return temp;
			
		}

		template < typename Type >
		Type ThreadSafeQueue<Type>::Back()
		{
			stdMutex.lock();
			Type temp = this->back->item;
			stdMutex.unlock();

			return temp;
			
		}

		template < typename Type >
		int ThreadSafeQueue<Type>::Size()
		{
			stdMutex.lock();
			int size = this->nrOfNodes;
			stdMutex.unlock();

			return size;

		}

		template < typename Type >
		bool ThreadSafeQueue<Type>::IsEmpty()
		{
			stdMutex.lock();
			if(nrOfNodes == 0 || this->front == NULL)
			{
				stdMutex.unlock();
				return true;
			}
			
			else
			{
				stdMutex.unlock();
			}

			return false;
		}

		template < typename Type >
		void ThreadSafeQueue<Type>::Swap(IQueue<Type> &queue )
		{
			stdMutex.lock();
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
			stdMutex.unlock();
		}		

		template < typename Type > 
		void ThreadSafeQueue<Type>::Clear()
		{
			while (!IsEmpty())
			{
				Pop();
			}
		}


	}
}
#endif

