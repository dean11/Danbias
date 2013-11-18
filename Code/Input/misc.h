#ifndef MISC_H
#define MISC_H


#include <assert.h>


template<typename T>
class List
{
	private:
		class Node
		{
			public:
				T value;
				Node *next;
				Node(T value){ this->value = value; this->next = NULL; }
				~Node() {}
		};
	
		Node *first;
		int nrOfNodes;

	public:
		List::List()
		{
			this->first = NULL;
			this->nrOfNodes = 0;
		}
		
		List::~List()
		{
			Node *walker = this->first;

			for(int i = 0; i<this->nrOfNodes; i++)
			{
				walker = walker->next;
				delete this->first;
				this->first = walker;
			}
		}

		List& List::operator=(const List& origObj)
		{
			if(this->nrOfNodes > 0)
			{
				Node *walker = this->first;

				for(int i = 0; i<this->nrOfNodes; i++)
				{
					walker = walker->next;
					delete this->first;
					this->first = walker;
				}
			}

			this->nrOfNodes = 0;
			if(origObj.nrOfNodes > 0)
			{
				Node *walker = origObj.first;

				for(int i = 0; i<origObj.nrOfNodes; i++)
				{
					insertLast(walker->value);
					walker = walker->next;
				}
			}

			return *this;
		}

		void List::push(T value)
		{
			Node *e = new Node(value);
			e->next = this->first;
			this->first = e;
			e = NULL;
			this->nrOfNodes++;
		}

		T List::pop()
		{
			T removed;
			memset(&removed, 0, sizeof(T));

			if(this->nrOfNodes > 0)
			{
				Node *temp = first;
				this->first = first->next;
				memcpy(&removed, &temp->value, sizeof(T));
				delete temp;
				this->nrOfNodes--;
			}

			return removed;
		}

		int List::size() const
		{
			return this->nrOfNodes;
		}

		void clear()
		{
			Node *w = this->first;
			Node *p = 0;
			while (w)
			{
				p = w;
				w = w->next;
				delete p;
			}

			this->first = 0;
		}
		
};


#endif
