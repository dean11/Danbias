/////////////////////////////////////////////////////////////////////
// Inline and template implementations for
// the Utility Collection of Miscellanious Handy Functions
//
// Created 2013 by Dan Andersson
// Edited 2013 by
// * Dan Andersson
// * Dennis Andersen
/////////////////////////////////////////////////////////////////////

#ifndef UTILITIES_INLINE_IMPL_H
#define UTILITIES_INLINE_IMPL_H

#include "Utilities.h"

namespace Utility
{
	namespace DynamicMemory
	{
		template<typename Type>
		inline void SafeDeleteInstance( Type *dynamicInstance )
		{
			if( dynamicInstance )
			{
				delete dynamicInstance;
			}
		}

		template<typename Type>
		void SafeDeleteArray( Type dynamicArray[] )
		{
			if( dynamicArray )
			{
				delete [] dynamicArray;
			}
		}

#pragma region UnuiqePointer
		template<typename Type>
		UniquePointer<Type>::UniquePointer( Type *assignedInstance )
		{
			this->ownedInstance = assignedInstance;
		}

		template<typename Type>
		UniquePointer<Type>::UniquePointer( const UniquePointer<Type> &donor )
		{
			this->ownedInstance = donor.ownedInstance;
			donor.ownedInstance = NULL;

		}

		template<typename Type>
		UniquePointer<Type>::~UniquePointer()
		{
			SafeDeleteInstance( this->ownedInstance );
		}

		template<typename Type>
		UniquePointer<Type> & UniquePointer<Type>::operator = ( const UniquePointer<Type> &donor )
		{
			SafeDeleteInstance( this->ownedInstance );
			this->ownedInstance = donor.ownedInstance;
			donor.ownedInstance = NULL;
			return *this;
		}

		template<typename Type>
		UniquePointer<Type>::operator Type* ()
		{
			return this->ownedInstance;
		}

		template<typename Type>
		UniquePointer<Type>::operator const Type* () const
		{
			return this->ownedInstance;
		}

		template<typename Type>
		Type * UniquePointer<Type>::operator -> ()
		{
			return this->ownedInstance;
		}

		template<typename Type>
		const Type * UniquePointer<Type>::operator -> () const
		{
			return this->ownedInstance;
		}

		template<typename Type>
		UniquePointer<Type>::operator bool() const
		{
			return this->ownedInstance != NULL;
		}

		template<typename Type>
		bool UniquePointer<Type>::operator == ( Type *stray ) const
		{
			return this->ownedInstance == stray;
		}

		template<typename Type>
		bool UniquePointer<Type>::operator != ( Type *stray ) const
		{
			return this->ownedInstance != stray;
		}

		template<typename Type>
		Type* UniquePointer<Type>::Release()
		{
			Type *copy = this->ownedInstance;
			this->ownedInstance = NULL;
			return copy;
		}
		
		template<typename Type>
		inline bool UniquePointer<Type>::HaveOwnership() const
		{
			return this->operator bool();
		}

		template<typename Type>
		UniqueArray<Type>::UniqueArray( Type assignedArray[] )
		{
			this->ownedArray = assignedArray;
		}

		template<typename Type>
		UniqueArray<Type>::UniqueArray( const UniqueArray<Type> &donor )
		{
			this->ownedArray = donor.ownedArray;
			donor.ownedArray = NULL;

		}

		template<typename Type>
		UniqueArray<Type>::~UniqueArray()
		{
			SafeDeleteArray( this->ownedArray );
		}

		template<typename Type>
		UniqueArray<Type> & UniqueArray<Type>::operator = ( const UniqueArray<Type> &donor )
		{
			SafeDeleteArray( this->ownedArray );
			this->ownedArray = donor.ownedInstance;
			donor.owned = NULL;
		}

		template<typename Type> template<typename Index>
		Type & UniqueArray<Type>::operator [] ( Index i )
		{
			return this->ownedArray[i];
		}

		template<typename Type> template<typename Index>
		const Type & UniqueArray<Type>::operator [] ( Index i ) const
		{
			return this->ownedArray[i];
		}

		template<typename Type>
		UniqueArray<Type>::operator bool () const
		{
			return this->ownedArray != NULL;
		}

		template<typename Type>
		bool UniqueArray<Type>::operator == ( Type *stray ) const
		{
			return this->ownedArray == stray;
		}

		template<typename Type>
		bool UniqueArray<Type>::operator != ( Type *stray ) const
		{
			return this->ownedArray != stray;
		}

		template<typename Type>
		Type* UniqueArray<Type>::Release()
		{
			Type *copy = this->ownedArray;
			this->ownedArray = NULL;
			return copy;
		}

		template<typename Type>
		inline bool UniqueArray<Type>::HaveOwnership() const
		{
			return this->operator bool();
		}
#pragma endregion

#pragma region SmartPointer
		template<typename T> void SmartPointer<T>::Destroy()
		{
			delete this->_rc;
			this->_rc = NULL;

			//Use default function for memory deallocation.
			SafeDeleteInstance<T>(this->_ptr);

			this->_ptr = NULL;
		}
		template<typename T> SmartPointer<T>::SmartPointer()
				:_rc(0), _ptr(0)
			{ }
		template<typename T> SmartPointer<T>::SmartPointer(UniquePointer<T>& p)
				:_ptr(p.Release())
		{ 
			this->_rc = new ReferenceCount();
			this->_rc->Incref();
		}
		template<typename T> SmartPointer<T>::SmartPointer(T* p)
			:_ptr(p)
		{ 
			this->_rc = new ReferenceCount();
			this->_rc->Incref();
		}
		template<typename T> SmartPointer<T>::SmartPointer(const SmartPointer& d)
			:_ptr(d._ptr), _rc(d._rc)
		{
			if(this->_rc)
				this->_rc->Incref();
		}
		template<typename T> SmartPointer<T>::~SmartPointer()
		{
			this->Release();
		}
		template<typename T> SmartPointer<T>& SmartPointer<T>::operator= (const SmartPointer<T>& p)
		{
			if (this != &p)
			{
				//Last to go?
				if(this->_rc && this->_rc->Decref() == 0)
				{
					//Call child specific
					Destroy();
				}

				this->_ptr = p._ptr;
				this->_rc = p._rc;
				if(this->_rc)	this->_rc->Incref();
			}
			return *this;
		}
		template<typename T> SmartPointer<T>& SmartPointer<T>::operator= (UniquePointer<T>& p)
		{
			//Last to go?
			if(this->_rc)
			{
				if(this->_rc->Decref() == 0)
				{
					//Call child specific
					Destroy();
					this->_rc = new ReferenceCount();
				}
			}
			else
			{
				if(p) this->_rc = new ReferenceCount();
			}

			if(this->_rc)
				this->_rc->Incref();

			this->_ptr = p.Release();

			return *this;
		}
		template<typename T> SmartPointer<T>& SmartPointer<T>::operator= (T* p)
		{
			if (this->_ptr != p)
			{
				//Last to go?
				if(this->_rc)
				{
					if(this->_rc->Decref() == 0)
					{
						//Call child specific
						Destroy();
						if(p)	this->_rc = new ReferenceCount();
					}
				}
				else if(p)
				{
					this->_rc = new ReferenceCount();
				}

				this->_ptr = p;

				if(p)	this->_rc->Incref();
				else	this->_rc = 0;
			}
			return *this;
		}
		template<typename T> inline bool SmartPointer<T>::operator== (const SmartPointer<T>& d) const
		{
			return d._ptr == this->_ptr;
		}
		template<typename T> inline bool SmartPointer<T>::operator== (const T& p) const
		{
			return &p == this->_ptr;
		}
		template<typename T> inline bool SmartPointer<T>::operator!= (const SmartPointer<T>& d) const
		{
			return d._ptr != this->_ptr;
		}
		template<typename T> inline bool SmartPointer<T>::operator!= (const T& p) const
		{
			return &p != this->_ptr;
		}
		template<typename T> inline T& SmartPointer<T>::operator* ()
		{
			return *this->_ptr;
		}
		template<typename T> inline const T& SmartPointer<T>::operator* () const
		{
			return *this->_ptr;
		}
		template<typename T> inline T* SmartPointer<T>::operator-> ()
		{
			return this->_ptr;
		}
		template<typename T> inline const T* SmartPointer<T>::operator-> () const
		{
			return this->_ptr;
		}
		template<typename T> inline SmartPointer<T>::operator T* () const
		{
			return this->_ptr;
		}
		template<typename T> inline SmartPointer<T>::operator const T* () const
		{
			return this->_ptr;
		}
		template<typename T> inline SmartPointer<T>::operator T& () const
		{
			return *this->_ptr;
		}
		template<typename T> inline SmartPointer<T>::operator bool() const
		{
			return (this->_ptr != 0);
		}
		template<typename T> inline T* SmartPointer<T>::Get()
		{
			return this->_ptr;
		}
		template<typename T> inline T* SmartPointer<T>::Get() const
		{
			return this->_ptr;
		}
		template<typename T> int SmartPointer<T>::Release()
		{
			int returnVal = 0;
			
			if(this->_rc && ((returnVal = this->_rc->Decref()) == 0))
			{
				Destroy();
			}
			return returnVal;
		}
		template<typename T> int SmartPointer<T>::ReleaseDummy()
		{
			int val = this->_rc->Decref();
			this->_rc->Incref();
			return val;
		}
		template<typename T> inline bool SmartPointer<T>::IsValid() const
		{
			return (this->_ptr != NULL)  ?	true : false;
		}
#pragma endregion

	}

	namespace Thread
	{
#pragma region ThreadSafeSmartPointer
		template<typename T> void ThreadSafeSmartPointer<T>::Destroy()
		{
			delete this->_rc.load();
			this->_rc = NULL;

			//Use default function for memory deallocation.
			SafeDeleteInstance<T>(this->_ptr.load());

			this->_ptr = NULL;
		}
		template<typename T> ThreadSafeSmartPointer<T>::ThreadSafeSmartPointer()
				:_rc(0), _ptr(0)
			{ }
		template<typename T> ThreadSafeSmartPointer<T>::ThreadSafeSmartPointer(UniquePointer<T>& p)
				:_ptr(p.Release())
		{ 
			this->_rc = new ReferenceCount();
			this->_rc->Incref();
		}
		template<typename T> ThreadSafeSmartPointer<T>::ThreadSafeSmartPointer(T* p)
			:_ptr(p)
		{ 
			this->_rc.store = new ReferenceCount();
			this->_rc->Incref();
		}
		template<typename T> ThreadSafeSmartPointer<T>::ThreadSafeSmartPointer(const ThreadSafeSmartPointer& d)
			:_ptr(d._ptr), _rc(d._rc)
		{
			if(this->_rc)
				this->_rc->Incref();
		}
		template<typename T> ThreadSafeSmartPointer<T>::~ThreadSafeSmartPointer()
		{
			this->Release();
		}
		template<typename T> ThreadSafeSmartPointer<T>& ThreadSafeSmartPointer<T>::operator= (const ThreadSafeSmartPointer<T>& p)
		{
			if (this != &p)
			{
				//Last to go?
				if(this->_rc.load() && this->_rc.load()->Decref() == 0)
				{
					//Call child specific
					Destroy();
				}

				this->_ptr.store(p._ptr.load());
				this->_rc.store(p._rc.load());
				if(this->_rc.load())	this->_rc.load()->Incref();
			}
			return *this;
		}
		template<typename T> ThreadSafeSmartPointer<T>& ThreadSafeSmartPointer<T>::operator= (UniquePointer<T>& p)
		{
			//Last to go?
			if(this->_rc)
			{
				if(this->_rc->Decref() == 0)
				{
					//Call child specific
					Destroy();
					this->_rc = new ReferenceCount();
				}
			}
			else
			{
				if(p) this->_rc = new ReferenceCount();
			}

			if(this->_rc)
				this->_rc->Incref();

			this->_ptr = p.Release();

			return *this;
		}
		template<typename T> ThreadSafeSmartPointer<T>& ThreadSafeSmartPointer<T>::operator= (T* p)
		{
			if (this->_ptr != p)
			{
				//Last to go?
				if(this->_rc.load())
				{
					if(this->_rc.load()->Decref() == 0)
					{
						//Call child specific
						Destroy();
						if(p)	this->_rc = new ReferenceCount();
					}
				}
				else if(p)
				{
					this->_rc = new ReferenceCount();
				}

				this->_ptr = p;

				if(p)	this->_rc.load()->Incref();
				else	this->_rc = 0;
			}
			return *this;
		}
		template<typename T> inline bool ThreadSafeSmartPointer<T>::operator== (const ThreadSafeSmartPointer<T>& d) const
		{
			return d._ptr == this->_ptr;
		}
		template<typename T> inline bool ThreadSafeSmartPointer<T>::operator== (const T& p) const
		{
			return &p == this->_ptr;
		}
		template<typename T> inline bool ThreadSafeSmartPointer<T>::operator!= (const ThreadSafeSmartPointer<T>& d) const
		{
			return d._ptr != this->_ptr;
		}
		template<typename T> inline bool ThreadSafeSmartPointer<T>::operator!= (const T& p) const
		{
			return &p != this->_ptr;
		}
		template<typename T> inline T& ThreadSafeSmartPointer<T>::operator* ()
		{
			return *this->_ptr;
		}
		template<typename T> inline const T& ThreadSafeSmartPointer<T>::operator* () const
		{
			return *this->_ptr;
		}
		template<typename T> inline T* ThreadSafeSmartPointer<T>::operator-> ()
		{
			return this->_ptr;
		}
		template<typename T> inline const T* ThreadSafeSmartPointer<T>::operator-> () const
		{
			return this->_ptr;
		}
		template<typename T> inline ThreadSafeSmartPointer<T>::operator T* () const
		{
			return this->_ptr;
		}
		template<typename T> inline ThreadSafeSmartPointer<T>::operator const T* () const
		{
			return this->_ptr;
		}
		template<typename T> inline ThreadSafeSmartPointer<T>::operator T& () const
		{
			return *this->_ptr;
		}
		template<typename T> inline ThreadSafeSmartPointer<T>::operator bool() const
		{
			return (this->_ptr != 0);
		}
		template<typename T> inline T* ThreadSafeSmartPointer<T>::Get()
		{
			return this->_ptr;
		}
		template<typename T> inline T* ThreadSafeSmartPointer<T>::Get() const
		{
			return this->_ptr;
		}
		template<typename T> int ThreadSafeSmartPointer<T>::Release()
		{
			int returnVal = 0;
			
			if(this->_rc.load() && ((returnVal = this->_rc.load()->Decref()) == 0))
			{
				Destroy();
			}
			return returnVal;
		}
		template<typename T> int ThreadSafeSmartPointer<T>::ReleaseDummy()
		{
			int val = this->_rc->Decref();
			this->_rc->Incref();
			return val;
		}
		template<typename T> inline bool ThreadSafeSmartPointer<T>::IsValid() const
		{
			return (this->_ptr != NULL)  ?	true : false;
		}
#pragma endregion
	}
}

#endif