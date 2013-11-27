/////////////////////////////////////////////////////////////////////
// Utility Collection of Miscellanious Handy Functions
// © Dan Andersson 2013
// © Dennis Andersen 2013
/////////////////////////////////////////////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <istream>
#include <vector>
#include <locale>
#include <limits>

namespace Utility
{
	namespace DynamicMemory
	{
		/// If dynamicInstance is not NULL, then delete
		template<typename Type> void SafeDeleteInstance( Type *dynamicInstance );

		/// If dynamicArray is not NULL, then delete []
		template<typename Type> void SafeDeleteArray( Type dynamicArray[] );

		template<typename Type>
		struct UniquePointer
		{ /// Wrapper to safely transfer dynamic ownership/responsibility
		public:
			/// Assigns assignedInstance ownership to this UniquePonter, old owned instance will be deleted.
			/// If NULL is assigned is equavalent with clearing all responsibilities from this UniquePointer.
			UniquePointer( Type *assignedInstance = NULL );

			/// Will auto delete assigned dynamic instance.
			~UniquePointer();

			/// Assigns assignedInstance ownership to this UniquePonter, old owned instance will be deleted.
			/// If NULL is assigned is equavalent with clearing all responsibilities from this UniquePointer.
			UniquePointer<Type> & operator = ( Type *assignedInstance );

			/// Transfers assignedInstance ownership from donor to this UniquePonter, old owned instance will be deleted.
			/// If donor had nothing, is equavalent with clearing all responsibilities from this UniquePointer.
			UniquePointer<Type> & operator = ( const UniquePointer<Type> &donor );

			/// Access the assigned dynamic instance. Will crash if nothing there
			operator Type* ();

			/// Access the assigned dynamic instance. Will crash if nothing there
			operator const Type* () const;
			
			/// Access members of the assigned dynamic instance. Will crash if nothing there
			Type * operator -> ();
			
			/// Access members of the assigned dynamic instance. Will crash if nothing there
			const Type * operator -> () const;

			/// If true, this UniquePointer have a current ownership/responsibility of a dynamic instance.
			operator bool () const;

			/// This UniquePointer drops all claims of ownership/responsibility and returns the dynamic instance. Now it is your responsibility to delete.
			Type* Release();
			
			/// (inline) If true, this UniquePointer have a current ownership/responsibility of a dynamic instance.
			bool HaveOwnership() const;
			
		private:
			mutable Type *ownedInstance;
		};

		template<typename Type>
		struct UniqueArray
		{ /// Wrapper to safely transfer dynamic ownership/responsibility
		public:
			/// Assigns assignedInstance ownership to this UniquePonter, old owned array will be deleted.
			/// If NULL is assigned is equavalent with clearing all responsibilities from this UniqueArray.
			UniqueArray( Type assignedArray[] = NULL );
			
			/// Will auto delete assigned dynamic array.
			~UniqueArray();

			/// Assigns assignedInstance ownership to this UniquePonter, old owned array will be deleted.
			/// If NULL is assigned is equavalent with clearing all responsibilities from this UniqueArray.
			UniqueArray<Type> & operator = ( Type assignedArray[] );
			
			/// Transfers assignedInstance ownership from donor to this UniquePonter, old owned array will be deleted.
			/// If donor had nothing, is equavalent with clearing all responsibilities from this UniqueArray.
			UniqueArray<Type> & operator = ( const UniqueArray<Type> &donor );

			/// Accesses the instance at index i of this UniqeArray's owned dynamic array.
			/// Will crash if out-of-bound or there is no assigned array.
			template<typename Index> Type & operator [] ( Index i );
			
			/// Accesses the instance at index i of this UniqeArray's owned dynamic array.
			/// Will crash if out-of-bound or there is no assigned array.
			template<typename Index> const Type & operator [] ( Index i ) const;

			/// If true, this UniqueArray have a current ownership/responsibility of a dynamic instance.
			operator bool () const;

			/// This UniqueArray drops all claims of ownership/responsibility and returns the dynamic array. Now it is your responsibility to delete.
			Type* Release();

			/// (inline) If true, this UniqueArray have a current ownership/responsibility of a dynamic array.
			bool HaveOwnership() const;

		private:
			mutable Type *ownedArray;
		};

		struct ReferenceCount
		{
			private:
				int count;

			public:
				ReferenceCount()		:count(0)									{ }
				ReferenceCount(const ReferenceCount& o)								{ count = o.count; }
				inline const ReferenceCount& operator=(const ReferenceCount& o)		{ count = o.count;  return *this;}
				inline void Incref()												{ this->count++; }
				inline void Incref(int c)											{ this->count += c; }
				inline int  Decref()												{ return --this->count;}
				inline void Reset()													{ this->count = 0; }
		};

		namespace SmartPointer
		{
			//! Smart pointer for a regular object.
			/** 
			*	Regular objects, objects that is deleted normaly (ie not COM objects, or array pointers) 
			*	can use this class to easy the use of dynamic memory 
			*/
			template<typename T>
			struct StdSmartPointer
			{
				private:
					ReferenceCount	*_rc;
					T				*_ptr;

					/** Destroys the pointer and returns the memory allocated. */
					void Destroy();

				public:
					StdSmartPointer();
					StdSmartPointer(T* p);
					StdSmartPointer(const StdSmartPointer& d);
					virtual~StdSmartPointer();
					StdSmartPointer<T>& operator= (const StdSmartPointer<T>& p);
					StdSmartPointer<T>& operator= (T* p);
					bool operator== (const StdSmartPointer<T>& d);
					bool operator== (const T& p);
					T& operator* ();
					T* operator-> ();
					operator T* ();
					operator bool();

					/**
					*	Returns the connected pointer */
					T* Get();

					/** Checks if the pointer is valid (not NULL)
						Returns true for valid, else false. */
					bool IsValid();
			};
		}


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
		::std::wstring & wToLowerCase( ::std::wstring &output, const ::std::wstring &str );
		::std::wstring & wToLowerCase( ::std::wstring &str );

		//To wstring

		::std::wstring & StringToWstring( const ::std::string &str, ::std::wstring &wstr );
		::std::string & WStringToString( const ::std::wstring &wstr, ::std::string &str );
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

#include "Utilities-InlineImpl.h"

#endif