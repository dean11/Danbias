//////////////////////////////
//	Dennis Andersen 2013	//
//////////////////////////////
#ifndef MISC_DYNAMIC_ARRAY_H
#define MISC_DYNAMIC_ARRAY_H

#include <assert.h>

namespace Utility
{
	namespace DynamicMemory
	{
		template <typename T>
		class DynamicArray
		{
		public:
			DynamicArray();
			DynamicArray(unsigned int capacity);
			DynamicArray(const DynamicArray& orig);
			const DynamicArray& operator=(const DynamicArray& orig);
			virtual~DynamicArray();

			T& operator[](unsigned int index);
			const T& operator[](unsigned int index) const;

			void Push(const T& value);
			void Push(unsigned int index, const T& value);

			T& PopFront();
			T& PopBack();
			T& Pop(unsigned int index);

			void Remove(T value);
			void Remove(unsigned int index);
			void Remove(unsigned int first, unsigned int last);

			void Clear();

			void Resize(unsigned int size);

			void Reserve(unsigned int size);

			void Swap(unsigned int a, unsigned int b);

			unsigned int Size() const;
			unsigned int Capacity() const;

			bool IsEmpty() const;

			T* begin();
			T* end();

		private:
			void Expand(int elements = 0);

		private:
			T* data;
			int size;
			int capacity;

		};

		#pragma region Implementation
			template <typename T> DynamicArray<T>::DynamicArray()
				:	data(0)
				,	size(0)
				,	capacity(0)
			{ }

			template <typename T> DynamicArray<T>::DynamicArray(unsigned int capacity)
				:	size(capacity)
				,	capacity(capacity)
				,	data(new T[capacity])
			{ }

			template <typename T> DynamicArray<T>::DynamicArray(const DynamicArray& orig)
				:	capacity(orig.capacity)
				,	size(orig.size)
			{
				this->data = new T[orig.capacity];
				for (int i = 0; i < orig.size; i++)
				{
					this->data[i] = orig.data[i];
				}
			}

			template <typename T> const DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& orig)
			{
				if(this->data)
				{
					delete [] this->data;
				}
		
				this->capacity = orig.capacity;
				this->size = orig.size;
				if(orig.capacity > 0)
				{
					this->data = new T[orig.capacity];
					for (int i = 0; i < orig.size; i++)
					{
						this->data[i] = orig.data[i];
					}
				}
				return *this;
			}
	
			template <typename T> DynamicArray<T>::~DynamicArray()
			{
				Clear();
			}
	
			template <typename T> T& DynamicArray<T>::operator[](unsigned int index)
			{
				assert((int)index < this->size);
		
				return this->data[index];
			}
	
			template <typename T> const T& DynamicArray<T>::operator[](unsigned int index) const
			{
				assert((int)index < this->size);
		
				return this->data[index];
			}
	
			template <typename T> void DynamicArray<T>::Push(const T& value)
			{
				Expand(1);

				this->data[this->size] = value;
				this->size ++;
			}
	
			template <typename T> void DynamicArray<T>::Push(unsigned int index, const T& value)
			{
				int newElem = 1;
				if((int)index >= this->size)
					newElem = (index + 1) - this->size;
		
				Expand(newElem);

				this->data[index] = value;
				this->size += newElem;
			}
	
			template <typename T> T& DynamicArray<T>::PopFront()
			{
				return Pop(0);
			}

			template <typename T> T& DynamicArray<T>::PopBack()
			{
				return Pop(this->size-1);
			}
	
			template <typename T> T& DynamicArray<T>::Pop(unsigned int index)
			{
				assert((int)index < this->size);

				T* temp = new T[this->capacity];

				for (int i = 0; i < this->size; i++)
				{
					if(i != index) temp[i] = this->data[i];
				}
				delete [] this->data;
				this->data = temp;
				this->size--;
				return this->data[index];
			}
	
			template <typename T> void DynamicArray<T>::Remove(T value)
			{
				T* temp = new T[this->capacity - 1];
		
				for (int i = 0; i < this->size; i++)
				{
					if(this->data[i] == value) 
					{
						Remove(i);
						break;
					}
				}
			}

			template <typename T> void DynamicArray<T>::Remove(unsigned int index)
			{
				assert(index < (unsigned int) this->size);

				T* temp = new T[this->capacity];
		
				for (int i = 0; i < this->size; i++)
				{
					if(i != index) temp[i] = this->data[i];
				}

				delete [] this->data;
				this->data = temp;
				this->size--;
			}
	
			template <typename T> void DynamicArray<T>::Clear()
			{
				if(this->data)
					delete [] this->data;

				this->data = 0;
				this->size = 0;
				this->capacity = 0;
			}

			template <typename T> void DynamicArray<T>::Resize(unsigned int size)
			{
				if (size == this->capacity) return;	//No need to resize
				
				T* temp = new T[size];

				for (int i = 0; i < this->size; i++)
				{
					temp[i] = this->data[i];
				}
				this->capacity = size;
				this->size = size;

				delete [] this->data;
				this->data = temp;
			}

			template <typename T> void DynamicArray<T>::Reserve(unsigned int size)
			{
				Expand(size);
			}

			template <typename T> void DynamicArray<T>::Swap(unsigned int a, unsigned int b)
			{
				T temp = this->data[a];
				this->data[a] = this->data[b];
				this->data[b] = temp;
			}

			template <typename T> unsigned int DynamicArray<T>::Size() const
			{
				return (unsigned int)this->size;
			}

			template <typename T> unsigned int DynamicArray<T>::Capacity() const
			{
				return (unsigned int)this->capacity;
			}

			template <typename T> bool DynamicArray<T>::IsEmpty() const
			{
				return (this->size == 0);
			}

			template <typename T> void DynamicArray<T>::Expand(int elements)
			{
				if(elements < 1) return;

				int newSize = this->size + elements;

				if(newSize > this->capacity)
				{
					T* temp = new T[newSize];

					for (int i = 0; i < this->size; i++)
					{
						temp[i] = this->data[i];
					}
					this->capacity = newSize;

					// By temporary storing data pointer in a destroyer, the memory data points ad will 
					// never be invalid when other threads try to access. (Still not as safe as mutex or atomic)
					T* destroyer = this->data;
					this->data = temp;

					delete [] destroyer;
				}
			}

			template <typename T> T* DynamicArray<T>::begin()
			{
				if(this->size == 0) return 0;
				return &this->data[0];
			}

			template <typename T> T* DynamicArray<T>::end()
			{
				if(this->size == 0) return 0;
				return ((&this->data[this->size - 1]) + 1);
			}

		#pragma endregion
	}
}

#endif