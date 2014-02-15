/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef MISC_LINKED_LIST_H
#define MISC_LINKED_LIST_H


namespace Utility
{
	namespace DynamicMemory
	{
		template <typename Type>
		class LinkedList
		{
		private:
			class Node
			{
			public:
				Type data;
				Node *next;
				Node() :next(0) {  }
				Node(Type t) :next(0), data(t) {  }
				~Node() { next = 0; }
			};

		public:
			LinkedList();
			~LinkedList();

			Type& operator[](int index);

			void PushFront(Type value);
			Type PopFront();
			void PushBack(Type value);
			Type PopBack();
			Type PeakFront();
			Type PeakBack();

			void Remove(Type value);
			bool IsEmpty();
			int Size();
			void Clear();

		private:
			void Destroy(Node* n);

		

		private:
			Node *first;
			Node *last;
			int nrOfNodes;
		};



		template <typename Type> LinkedList<Type>::LinkedList()
		{
			this->first = 0;
			this->last = 0;
			this->nrOfNodes = 0;
		}
		template <typename Type> LinkedList<Type>::~LinkedList()
		{
			this->last = 0;
			this->Destroy(this->first);
		}
		template <typename Type> Type& LinkedList<Type>::operator[](int index)
		{
			if(this->index >= this->nrofNodes || index < 0)
			{
				throw;
			}
			int i = 0;
			LinkedList<Type>::Node* walker = this->first;
			while (i != index)
			{
				walker = walker->next;
				i++;
			}

			return walker->data;
		}
		template <typename Type> void LinkedList<Type>::PushFront(Type value)
		{
			Node *n = new Node(value);
			n->next = this->first;
			this->first = n;

			if(!this->nrOfNodes)	
				this->back = this->front;

			this->nrOfNodes++;
		}
		template <typename Type> Type LinkedList<Type>::PopFront()
		{
			if(!this->first)
			{
				throw;
			}

			Type temp = this->first->data;
			Node* head = this->first;
			this->first = this->first->next;
			delete head;
			this->nrOfNodes--;
			return temp;
		}
		template <typename Type> void LinkedList<Type>::PushBack(Type value)
		{
			Node* n = new Node(value);

			if(!this->first)
			{
				this->first = n;
				this->last = n;
			}
			else
			{
		  		this->last->next = n;
				this->last = n;
			}
			this->nrOfNodes++;
		}
		template <typename Type> Type LinkedList<Type>::PopBack()
		{
			if(!this->first)
				throw;

			Type val = this->last->data;
			Node* walker = this->first;
			while (walker)
			{
				if(walker->next == this->last)
				{
					delete this->last;
					this->last = walker;
					walker = 0;
					this->nrOfNodes--;
				}
			}
		}
		template <typename Type> Type LinkedList<Type>::PeakFront()
		{
			if(!this->front)
				throw;
			return this->first->data;
		}
		template <typename Type> Type LinkedList<Type>::PeakBack()
		{
			if(!this->last)
				throw;
			return this->last->data;
		}
		template <typename Type> void LinkedList<Type>::Remove(Type value)
		{
			Node *w = this->first;
			if(this->nrOfNodes == 1)				//Sepcial case 
			{
				//First and last is equal
				if(this->first->data == value)
				{
					delete this->first;
					this->first = 0;
					this->last = 0;
					this->nrOfNodes--;
				}
			}
			else
			{
				Node *prev = 0;
				while (w)
				{
					if(w->data == value)
					{
						if(w == this->first)
						{
							this->first = this->first->next;
							delete w;
							w = 0;
						}
						else if (w == this->last)
						{
							this->last = prev;
							delete w;
							w = 0;
						}
						else
						{
							prev->next = w->next;
							delete w;
							w = 0;
						}
					}
					else
					{
						prev = w;
						w = w->next;
					}
				}
			}
		}
		template <typename Type> bool LinkedList<Type>::IsEmpty()
		{
			return (this->nrOfNodes == 0);
		}
		template <typename Type> int LinkedList<Type>::Size()
		{
			return this->nrOfNodes;
		}
		template <typename Type> void LinkedList<Type>::Clear()
		{
			this->Destroy(this->first);
		}
		template <typename Type> void LinkedList<Type>::Destroy(Node* n)
		{
			if(n)
			{
				Destroy(n->next);
				delete n;
			}
		}
	}
}
#endif // !MISC_LINKED_LIST_H
