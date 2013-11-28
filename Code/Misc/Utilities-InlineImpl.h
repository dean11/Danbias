/////////////////////////////////////////////////////////////////////
// Inline and template implementations for
// the Utility Collection of Miscellanious Handy Functions
// � Dan Andersson 2013
// � Dennis Andersen 2013 TODO: Is this correct?
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

		template<typename Type>
		UniquePointer<Type>::UniquePointer( Type *assignedInstance )
		{
			this->ownedInstance = assignedInstance;
		}

		template<typename Type>
		UniquePointer<Type>::~UniquePointer()
		{
			SafeDeleteInstance( this->ownedInstance );
		}

		template<typename Type>
		UniquePointer<Type> & UniquePointer<Type>::operator = ( Type *assignedInstance )
		{
			SafeDeleteInstance( this->ownedInstance );
			this->ownedInstance = assignedInstance;
			return *this;
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
		UniqueArray<Type>::~UniqueArray()
		{
			SafeDeleteArray( this->ownedArray );
		}

		template<typename Type>
		UniqueArray<Type> & UniqueArray<Type>::operator = ( Type assignedArray[] )
		{
			SafeDeleteArray( this->ownedArray );
			this->ownedArray = assignedArray;
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

		template<typename Type>Type* UniqueArray<Type>::Release()
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

		namespace SmartPointer
		{
			template<typename T> void StdSmartPointer<T>::Destroy()
			{
				delete this->_rc;
				this->_rc = NULL;
				delete this->_ptr;
				this->_ptr = NULL;
			}
			template<typename T> StdSmartPointer<T>::StdSmartPointer()
					:_rc(0), _ptr(0)
				{ }
			template<typename T> StdSmartPointer<T>::StdSmartPointer(T* p)
				:_ptr(p)
			{ 
				this->_rc = new ReferenceCount();
				this->_rc->Incref();
			}
			template<typename T> StdSmartPointer<T>::StdSmartPointer(const StdSmartPointer& d)
				:_ptr(d._ptr), _rc(d._rc)
			{
				if(this->_rc)
					this->_rc->Incref();
			}
			template<typename T> StdSmartPointer<T>::~StdSmartPointer()
			{
				if (this->_rc && this->_rc->Decref() == 0)
				{
					Destroy();
				}
			}
			template<typename T> StdSmartPointer<T>& StdSmartPointer<T>::operator= (const StdSmartPointer<T>& p)
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
					this->_rc->Incref();
				}
				return *this;
			}
			template<typename T> StdSmartPointer<T>& StdSmartPointer<T>::operator= (T* p)
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
							this->_rc = new ReferenceCount();
						}
					}
					else
						this->_rc = new ReferenceCount();
		
					this->_ptr = p;
					this->_rc->Incref();
				}
				return *this;
			}
			template<typename T> inline bool StdSmartPointer<T>::operator== (const StdSmartPointer<T>& d)
			{
				return d._ptr == this->_ptr;
			}
			template<typename T> inline bool StdSmartPointer<T>::operator== (const T& p)
			{
				return &p == this->_ptr;
			}
			template<typename T> inline T& StdSmartPointer<T>::operator* ()
			{
				return *this->_ptr;
			}
			template<typename T> inline T* StdSmartPointer<T>::operator-> ()
			{
				return this->_ptr;
			}
			template<typename T> inline StdSmartPointer<T>::operator T* ()
			{
				return this->_ptr;
			}
			template<typename T> inline StdSmartPointer<T>::operator bool()
			{
				return (this->_ptr != 0);
			}
			template<typename T> inline T* StdSmartPointer<T>::Get()
			{
				return this->_ptr;
			}
			template<typename T> inline bool StdSmartPointer<T>::IsValid()
			{
				return (this->_ptr != NULL)  ?	true : false;
			}
		}
	}
}

#endif