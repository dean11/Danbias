/////////////////////////////////////////////////////////////////////
// Utility Collection of Miscellanious Handy Functions
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <istream>
#include <vector>
#include <locale>
#include <limits>

namespace Utility
{
	namespace Memory
	{
		template<typename Type>
		struct UniquePointer
		{
		public:
			UniquePointer( Type *assignedMemory = NULL );
			~UniquePointer();

			UniquePointer<Type> & operator = ( Type *assignedMemory );
			UniquePointer<Type> & operator = ( const UniquePointer<Type> &donor );

			operator Type* ();
			operator const Type* () const;
			Type * operator -> ();
			const Type * operator -> () const;
			template<typename Index> Type & operator [] ( Index i );
			template<typename Index> const Type & operator [] ( Index i ) const;

			operator bool () const;

			Type* Release();
			bool haveOwnership() const;

		private:
			mutable Type *ownedMemory;
		};

		// IMPLEMENTATIONS ////////////////////////////////////////////////

		template<typename Type>
		UniquePointer<Type>::UniquePointer( Type *assignedMemory )
		{ this->ownedMemory = assignedMemory; }

		template<typename Type>
		UniquePointer<Type>::~UniquePointer()
		{ if( this->ownedMemory ) delete this->ownedMemory; }

		template<typename Type>
		UniquePointer<Type> & UniquePointer<Type>::operator = ( Type *assignedMemory )
		{
			if( this->ownedPointer ) delete this->ownedMemory;
			this->ownedMemory = assignedMemory;
			return *this;
		}

		template<typename Type>
		UniquePointer<Type> & UniquePointer<Type>::operator = ( const UniquePointer<Type> &donor )
		{
			if( this->ownedMemory ) delete this->ownedMemory;
			this->ownedMemory = donor.ownedMemory;
			donor.ownedMemory = NULL;
			return *this;
		}

		template<typename Type>
		UniquePointer<Type>::operator Type* ()
		{ return this->ownedMemory; }

		template<typename Type>
		UniquePointer<Type>::operator const Type* () const
		{ return this->ownedMemory; }

		template<typename Type>
		Type * UniquePointer<Type>::operator -> ()
		{ return this->ownedMemory; }

		template<typename Type>
		const Type * UniquePointer<Type>::operator -> () const
		{ return this->ownedMemory; }

		template<typename Type> template<typename Index>
		Type & UniquePointer<Type>::operator [] ( Index i )
		{ return this->ownedMemory[i]; }

		template<typename Type> template<typename Index>
		const Type & UniquePointer<Type>::operator [] ( Index i ) const
		{ return this->ownedMemory[i]; }

		template<typename Type>
		UniquePointer<Type>::operator bool() const
		{ return this->ownedMemory != NULL; }

		template<typename Type>
		Type* UniquePointer<Type>::Release()
		{
			Type *copy = this->ownedMemory;
			this->ownedMemory = NULL;
			return copy;
		}
		
		template<typename Type>
		inline bool UniquePointer<Type>::haveOwnership() const
		{ return this->operator bool(); }
	}

	namespace String
	{
		// string
		
		::std::vector<::std::string> & Split( ::std::vector<::std::string> &output, const ::std::string &str, char delim, ::std::string::size_type offset = 0 );
		::std::vector<::std::string> & Split( ::std::vector<::std::string> &output, const ::std::string &str, const ::std::string &delim, ::std::string::size_type offset = 0 );
		::std::vector<::std::string> & Split( ::std::vector<::std::string> &output, const ::std::string &str, const ::std::vector<::std::string> &delim, ::std::string::size_type offset = 0 );
		::std::string   Trim( const ::std::string &str );
		::std::string & ToLowerCase( ::std::string &output, const ::std::string &str );
		::std::string & ToLowerCase( ::std::string &str );
		::std::string & ToUpperCase( ::std::string &output, const ::std::string &str );
		::std::string & ToUpperCase( ::std::string &str );
		::std::string & ExtractDirPath( ::std::string &output, const ::std::string &file, char dirDelimeter );
		::std::string & ExtractDirPath( ::std::string &output, const ::std::string &file, const ::std::string &dirDelimeter );
		::std::string & ReplaceCharacters( ::std::string &str, char characterToReplace, char newCharacter, const ::std::string::size_type &offset = 0, const ::std::string::size_type &end = ::std::string::npos );

		// wstring

		::std::vector<::std::wstring> & Split( ::std::vector<::std::wstring> &output, const ::std::wstring &str, char delim, ::std::wstring::size_type offset = 0 );
		::std::vector<::std::wstring> & Split( ::std::vector<::std::wstring> &output, const ::std::wstring &str, const ::std::wstring &delim, ::std::wstring::size_type offset = 0 );
		::std::vector<::std::wstring> & Split( ::std::vector<::std::wstring> &output, const ::std::wstring &str, const ::std::vector<::std::wstring> &delim, ::std::wstring::size_type offset = 0 );
	}

	namespace Stream
	{
		float* ReadFloats( float *output, ::std::istream &input, unsigned int numFloats );
	}

	namespace StaticArray
	{
		template<typename ScalarType, unsigned int num>
		inline unsigned int NumElementsOf( const ScalarType(&)[num] )
		{ return num; }

		template<typename ScalarType, unsigned int num>
		inline ScalarType & FirstElementOf( ScalarType (&arr)[num] )
		{ return arr[0]; }

		template<typename ScalarType, unsigned int num>
		inline ScalarType & LastElementOf( ScalarType (&arr)[num] )
		{ return arr[num-1]; }
	}

	namespace Element
	{
		template<typename ScalarType>
		inline void Swap( ScalarType &elementA, ScalarType &elementB, ScalarType &swapSpace )
		{ swapSpace = elementA; elementA = elementB; elementB = swapSpace; }

		template<typename ScalarType>
		inline void Swap( ScalarType &elementA, ScalarType &elementB )
		{ ScalarType swapSpace; Swap( elementA, elementB, swapSpace ); }
	}

	namespace Value
	{
		using ::std::numeric_limits;

		template<typename ValueType>
		inline ValueType Abs( const ValueType &value )
		{ return value < 0 ? value * -1 : value; }

		template<typename ValueType>
		inline ValueType Max( const ValueType &valueA, const ValueType &valueB )
		{ return valueA > valueB ? valueA : valueB; }

		template<typename ValueType>
		inline ValueType Min( const ValueType &valueA, const ValueType &valueB )
		{ return valueA < valueB ? valueA : valueB; }

		template<typename ValueType>
		inline ValueType Average( const ValueType &valueA, const ValueType &valueB )
		{ return (valueA + valueB) * 0.5f; }

		template<typename ValueType>
		inline ValueType AverageWithDelta( const ValueType &origin, const ValueType &delta )
		{ return origin + (delta * 0.5f); }

		template<typename ValueType>
		inline ValueType Radian( const ValueType &degree )
		{ return degree * (3.1415926535897932384626433832795f / 180.0f); }

		template<typename ValueType>
		inline ValueType Degree( const ValueType &radian )
		{ return radian * (180.0f / 3.1415926535897932384626433832795f); }

		// SPECIALIZATIONS //////////////////////////////////////////

		template<> inline char Average<char>( const char &valueA, const char &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline unsigned char Average<unsigned char>( const unsigned char &valueA, const unsigned char &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline int Average<int>( const int &valueA, const int &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline unsigned int Average<unsigned int>( const unsigned int &valueA, const unsigned int &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline long Average<long>( const long &valueA, const long &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline unsigned long Average<unsigned long>( const unsigned long &valueA, const unsigned long &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline long long Average<long long>( const long long &valueA, const long long &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline unsigned long long Average<unsigned long long>( const unsigned long long &valueA, const unsigned long long &valueB )
		{ return (valueA + valueB) >> 1; }

		template<> inline char AverageWithDelta<char>( const char &origin, const char &delta )
		{ return origin + (delta >> 1); }

		template<> inline unsigned char AverageWithDelta<unsigned char>( const unsigned char &origin, const unsigned char &delta )
		{ return origin + (delta >> 1); }

		template<> inline int AverageWithDelta<int>( const int &origin, const int &delta )
		{ return origin + (delta >> 1); }

		template<> inline unsigned int AverageWithDelta<unsigned int>( const unsigned int &origin, const unsigned int &delta )
		{ return origin + (delta >> 1); }

		template<> inline long AverageWithDelta<long>( const long &origin, const long &delta )
		{ return origin + (delta >> 1); }

		template<> inline unsigned long AverageWithDelta<unsigned long>( const unsigned long &origin, const unsigned long &delta )
		{ return origin + (delta >> 1); }

		template<> inline long long AverageWithDelta<long long>( const long long &origin, const long long &delta )
		{ return origin + (delta >> 1); }

		template<> inline unsigned long long AverageWithDelta<unsigned long long>( const unsigned long long &origin, const unsigned long long &delta )
		{ return origin + (delta >> 1); }
	}
}

#endif