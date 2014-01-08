//////////////////////////////
//	Dennis Andersen 2013	//
//////////////////////////////
#ifndef MISC_DYNAMIC_ARRAY_H
#define MISC_DYNAMIC_ARRAY_H

#include <chrono>
#include <mutex>

namespace Utility
{
	namespace DynamicMemory
	{
		struct DefaultLock
		{
			void Lock() {}
			void TimeLock() {}
			void Unluck() {}
		};
		template <typename Length = std::chrono::milliseconds(10)>
		struct ThreadSafeArray
		{
			std::timed_mutex mutex;
			void Lock() { mutex.lock(); }
			void TimeLock() { mutex.try_lock_for(Length); }
			void Unluck() { mutex.unlock(); }
		};
		template <typename T, typename ConcurencyOption = DefaultLock>
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

			void Remove(unsigned int index);
			void Remove(unsigned int first, unsigned int last);

			void Clear();

			void Expand(int elements = 0);

			unsigned int Size() const;
			unsigned int Capacity() const;

		private:
			T* data;
			int size;
			int capacity;
			ConcurencyOption safety;
		};

		#pragma region Implementation
			template <typename T, typename ConcurencyOption> DynamicArray<T, ConcurencyOption>::DynamicArray()
				:	data(0)
				,	size(0)
				,	capacity(0)
			{ }

			template <typename T, typename ConcurencyOption> DynamicArray<T, ConcurencyOption>::DynamicArray(unsigned int capacity)
				:	size(capacity)
				,	capacity(capacity)
				,	data(new T[capacity])
			{ }

			template <typename T, typename ConcurencyOption> DynamicArray<T, ConcurencyOption>::DynamicArray(const DynamicArray& orig)
				:	capacity(orig.capacity)
				,	size(orig.size)
			{
				this->data = new T[orig.capacity];
				for (int i = 0; i < orig.size; i++)
				{
					this->data[i] = orig.data[i];
				}
			}

			template <typename T, typename ConcurencyOption> const DynamicArray<T, ConcurencyOption>& DynamicArray<T, ConcurencyOption>::operator=(const DynamicArray& orig)
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
	
			template <typename T, typename ConcurencyOption> DynamicArray<T, ConcurencyOption>::~DynamicArray()
			{
				Clear();
			}
	
			template <typename T, typename ConcurencyOption> T& DynamicArray<T, ConcurencyOption>::operator[](unsigned int index)
			{
				assert((int)index < this->size);
		
				this->safety.TimeLock();
				return this->data[index];
			}
	
			template <typename T, typename ConcurencyOption> const T& DynamicArray<T, ConcurencyOption>::operator[](unsigned int index) const
			{
				assert(index < this->size);
		
				this->safety.TimeLock();
				return this->data[index];
			}
	
			template <typename T, typename ConcurencyOption> void DynamicArray<T, ConcurencyOption>::Push(const T& value)
			{
				this->safety.Lock();
					Expand(1);

					this->data[this->size] = value;
					this->size ++;
				this->safety.Unlock();
			}
	
			template <typename T, typename ConcurencyOption> void DynamicArray<T, ConcurencyOption>::Push(unsigned int index, const T& value)
			{
				this->safety.Lock();
					int newElem = 1;
					if((int)index >= this->size)
						newElem = (index + 1) - this->size;
		
					Expand(newElem);

					this->data[index] = value;
					this->size += newElem;
				this->safety.Lock();
			}
	
			template <typename T, typename ConcurencyOption> T& DynamicArray<T, ConcurencyOption>::PopFront()
			{
				return Pop(0);
			}

			template <typename T, typename ConcurencyOption> T& DynamicArray<T, ConcurencyOption>::PopBack()
			{
				return Pop(this->size-1);
			}
	
			template <typename T, typename ConcurencyOption> T& DynamicArray<T, ConcurencyOption>::Pop(unsigned int index)
			{
				assert((int)index < this->size);

				this->safety.Lock();
					T* temp = new T[this->capacity];

					for (int i = 0; i < this->size; i++)
					{
						if(i != index) temp[i] = this->data[i];
					}
					delete [] this->data;
					this->data = temp;
					this->size--;
				this->safety.Unlock();

				return this->data[index];
			}
	
			template <typename T, typename ConcurencyOption> void DynamicArray<T, ConcurencyOption>::Remove(unsigned int index)
			{
				assert(index > this->size);

				T* temp = new T[this->capacity - 1];
		
				for (int i = 0; i < this->size; i++)
				{
					if(i != index) temp[i] = this->data[i];
				}

				delete [] this->data;
				this->data = temp;
				this->size--;
			}
	
			template <typename T, typename ConcurencyOption> void DynamicArray<T, ConcurencyOption>::Clear()
			{
				delete [] this->data;

				this->data = 0;
				this->size = 0;
				this->capacity = 0;
			}

			template <typename T, typename ConcurencyOption> void DynamicArray<T, ConcurencyOption>::Expand(int elements)
			{
				int newSize = this->size + elements;

				if(newSize >= this->capacity)
				{
					T* temp = new T[newSize];

					for (int i = 0; i < this->size; i++)
					{
						temp[i] = this->data[i];
					}
					this->capacity = newSize;

					delete [] this->data;
					this->data = temp;
				}
			}

			template <typename T, typename ConcurencyOption> unsigned int DynamicArray<T, ConcurencyOption>::Size() const
			{
				return (unsigned int)this->size;
			}

			template <typename T, typename ConcurencyOption> unsigned int DynamicArray<T, ConcurencyOption>::Capacity() const
			{
				return (unsigned int)this->capacity;
			}

		#pragma endregion
	}
}

#endif