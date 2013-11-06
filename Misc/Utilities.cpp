/////////////////////////////////////////////////////////////////////
// Utility Collection of Miscellanious Handy Functions
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Utilities.h"
#include <sstream>
#include <locale>

using ::std::vector;
using ::std::string;

namespace Utility
{
// PRIVATE STATIC ////////////////////////////////////////////////////
	namespace PrivateStatic
	{
		const ::std::locale systemDefaultLocale = ::std::locale();
	}

// STRING ////////////////////////////////////////////////////////////
	namespace String
	{
		vector<string> & split( vector<string> &output, const string &str, char delim, string::size_type offset )
		{
			if( str.length() > 0 )
			{
				while( offset < str.length() ) // trim
				{
					if( str[offset] == delim )
						++offset;
					else break;
				}

				string::size_type delimPos = str.find_first_of( delim, offset );
				if( delimPos == string::npos )
				{
					if( str.length() > offset )
						output.push_back( str.substr( offset, str.length() - offset ) );
				}
				else
				{
					if( delimPos > offset )
						output.push_back( str.substr( offset, delimPos - offset ) );
					String::split( output, str, delim, delimPos + 1 );
				}
			}
			return output;
		}

		vector<string> & split( vector<string> &output, const string &str, const string &delim, string::size_type offset )
		{
			if( str.length() > 0 )
			{
				string::size_type delimPos = str.find_first_of( delim, offset );
				if( delimPos == string::npos )
				{
					if( str.length() > offset )
						output.push_back( str.substr( offset, str.length() - offset ) );
				}
				else
				{
					if( delimPos > offset )
						output.push_back( str.substr( offset, delimPos - offset ) );
					String::split( output, str, delim, delimPos + delim.length() );
				}
			}
			return output;
		}

		vector<string> & split( vector<string> &output, const string &str, const vector<string> &delim, string::size_type offset )
		{
			if( str.length() > 0 )
			{
				string::size_type firstDelimPos = str.length(), delimPos;
				
				vector<string>::size_type numDelims = delim.size(), delimRef = 0;
				for( vector<string>::size_type i = 0; i < numDelims ; ++i )
				{
					delimPos = str.find_first_of( delim[i], offset );
					if( delimPos != string::npos ) if( delimPos < firstDelimPos )
					{
						delimRef = i;
						firstDelimPos = delimPos;
					}
				}
								
				if( firstDelimPos == str.length() )
				{
					if( str.length() > offset )
						output.push_back( str.substr( offset, str.length() - offset ) );
				}
				else
				{
					if( firstDelimPos > offset )
						output.push_back( str.substr( offset, firstDelimPos - offset ) );
					String::split( output, str, delim, firstDelimPos + delim[delimRef].length() );
				}
			}
			return output;
		}
		
		string trim( const string &str )
		{
			string::size_type first = 0,
							  last = str.length();

			if( last == 0 ) return str;

			while( first < last )
			{
				if( str[first] == ' ' || str[first] == '\t' || str[first] == '\r' || str[first] == '\n' )
					++first;
				else break;
			}

			--last;
			while( last > first )
			{
				if( str[last] == ' ' || str[last] == '\t' || str[last] == '\r' || str[last] == '\n' )
					--last;
				else break;
			}

			if( first == last ) if( str[first] == ' ' || str[first] == '\t' || str[first] == '\r' || str[first] == '\n' )
				return "";

			return str.substr( first, (++last) - first );
		}

		string & toLowerCase( string &output, const string &str )
		{
			int length = (int)str.length();
			output.resize( length );
			for( int i = 0; i < length; ++i )
				output[i] = ::std::tolower( str[i], ::std::locale() );
			return output;
		}

		string & toLowerCase( string &str )
		{
			int length = (int)str.length();
			for( int i = 0; i < length; ++i )
				str[i] = ::std::tolower( str[i], ::std::locale() );
			return str;
		}

		string & toUpperCase( string &output, const string &str )
		{
			int length = (int)str.length();
			output.resize( length );
			for( int i = 0; i < length; ++i )
				output[i] = ::std::toupper( str[i], ::std::locale() );
			return output;
		}

		string & toUpperCase( string &str )
		{
			int length = (int)str.length();
			for( int i = 0; i < length; ++i )
				str[i] = ::std::toupper( str[i], ::std::locale() );
			return str;
		}

		string & extractDirPath( string &output, const string &file, char dirDelimeter )
		{
			string d = " ";
			d[0] = dirDelimeter;
			return String::extractDirPath( output, file, d );
		}

		string & extractDirPath( string &output, const string &file, const string &dirDelimeter )
		{
			string::size_type end = file.find_last_of( dirDelimeter );
			if( end == string::npos )
				output = "";
			else
			{
				++end;
				output.resize( end );
				for( string::size_type i = 0; i < end; ++i )
					output[i] = file[i];
			}

			return output;
		}

		string & replaceCharacters( string &str, char characterToReplace, char newCharacter, const string::size_type &offset, const string::size_type &end )
		{
			string::size_type i = offset;
			while( true )
			{
				i = str.find_first_of( characterToReplace, i );
				if( i >= end ) break;
				
				str[i++] = newCharacter;
			}
			return str;
		}
	}

	// STREAM ////////////////////////////////////////////////////////////
	namespace Stream
	{
		float* readFloats( float *output, ::std::istream &input, unsigned int numFloats )
		{
			string str;
			for( unsigned int i = 0; i < numFloats; ++i )
			{
				input >> str;
				output[i] = (float)::std::atof( str.c_str() );
			}
			return output;
		}
	}
}