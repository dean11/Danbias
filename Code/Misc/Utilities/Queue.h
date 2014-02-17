#ifndef MISC_QUEUE_H
#define MISC_QUEUE_H

////////////////////////////////////////////
// Created by Sam Svensson 2013  
/////////////////////////////////////////////

#include "IQueue.h" 

namespace Utility
{
	namespace Container
	{
		template <typename Type>
		class Queue : public IQueue<Type>
		{
		public:
			Queue<Type>();
			virtual ~Queue<Type>();

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
		};

		


		//----------------------------------------------
		//implemented template functions
		//----------------------------------------------

		template < typename Type >
		Queue<Type>::Queue()
		{
			this->front = NULL;
			this->back = NULL;
			this->nrOfNodes = 0;
			
		}

		template < typename Type >
		Queue<Type>::~Queue()
		{
			if(!nrOfNodes) return;

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
		}


		template < typename Type >
		void Queue<Type>::Push(Type item)
		{
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
		}

		template < typename Type >
		Type Queue<Type>::Pop()
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

		template < typename Type >
		Type Queue<Type>::Front()
		{
			Type temp = this->front->item;
			
			return temp;
			
		}

		template < typename Type >
		Type Queue<Type>::Back()
		{
			Type temp = this->back->item;
			return temp;
		}

		template < typename Type >
		int Queue<Type>::Size()
		{
			int size = this->nrOfNodes;
			return size;

		}

		template < typename Type >
		bool Queue<Type>::IsEmpty()
		{
			if(nrOfNodes == 0 || this->front == NULL)
			{
				return true;
			}
			
			return false;
		}

		template < typename Type >
		void Queue<Type>::Swap(IQueue<Type> &queue )
		{
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
		}		


	}
}
#endif

