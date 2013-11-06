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

namespace Utility
{
	namespace String
	{
		// note to self: add a whitespaceSplit method?
		::std::vector<::std::string> & split( ::std::vector<::std::string> &output, const ::std::string &str, char delim, ::std::string::size_type offset = 0 );
		::std::vector<::std::string> & split( ::std::vector<::std::string> &output, const ::std::string &str, const ::std::string &delim, ::std::string::size_type offset = 0 );
		::std::vector<::std::string> & split( ::std::vector<::std::string> &output, const ::std::string &str, const ::std::vector<::std::string> &delim, ::std::string::size_type offset = 0 );
		::std::string   trim( const ::std::string &str );
		::std::string & toLowerCase( ::std::string &output, const ::std::string &str );
		::std::string & toLowerCase( ::std::string &str );
		::std::string & toUpperCase( ::std::string &output, const ::std::string &str );
		::std::string & toUpperCase( ::std::string &str );
		::std::string & extractDirPath( ::std::string &output, const ::std::string &file, char dirDelimeter );
		::std::string & extractDirPath( ::std::string &output, const ::std::string &file, const ::std::string &dirDelimeter );
		::std::string & replaceCharacters( ::std::string &str, char characterToReplace, char newCharacter, const ::std::string::size_type &offset = 0, const ::std::string::size_type &end = ::std::string::npos );
	}

	namespace Stream
	{
		float* readFloats( float *output, ::std::istream &input, unsigned int numFloats );
	}

	namespace StaticArray
	{
		template<typename ElementType, unsigned int num>
		inline unsigned int numElementsOf( const ElementType(&)[num] )
		{ return num; }
	}

	namespace Element
	{
		template<typename ElementType>
		inline void swap( ElementType &elementA, ElementType &elementB, ElementType &swapSpace )
		{ swapSpace = elementA; elementA = elementB; elementB = swapSpace; }

		template<typename ElementType>
		inline void swap( ElementType &elementA, ElementType &elementB )
		{ ElementType swapSpace; swap( elementA, elementB, swapSpace ); }
	}

	namespace Value
	{
		template<typename ValueType>
		inline ValueType abs( const ValueType &value )
		{ return value < 0 ? value * -1 : value; }

		template<typename ValueType>
		inline ValueType max( const ValueType &valueA, const ValueType &valueB )
		{ return valueA > valueB ? valueA : valueB; }

		template<typename ValueType>
		inline ValueType min( const ValueType &valueA, const ValueType &valueB )
		{ return valueA < valueB ? valueA : valueB; }

		template<typename ValueType>
		inline ValueType radian( const ValueType &degree )
		{ return degree * (3.1415926535897932384626433832795f / 180.0f); }

		template<typename ValueType>
		inline ValueType degree( const ValueType &radian )
		{ return radian * (180.0f / 3.1415926535897932384626433832795f); }
	}
}

#endif