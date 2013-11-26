/////////////////////////////////////////////////////////////////////
// Inline and template implementations for
// the Utility Collection of Miscellanious Handy Functions
// © Dan Andersson 2013
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
	}
}

#endif