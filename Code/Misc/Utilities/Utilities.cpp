/////////////////////////////////////////////////////////////////////
// Utility Collection of Miscellanious Handy Functions
// © Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Utilities.h"
#include <sstream>

using ::std::vector;
using ::std::string;
using ::std::wstring;

namespace Utility
{
// PRIVATE STATIC ////////////////////////////////////////////////////
	namespace PrivateStatic
	{
		const ::std::locale system_default_locale = ::std::locale();
	}

// STRING ////////////////////////////////////////////////////////////
	namespace String
	{
		// string

		vector<string> & Split( vector<string> &output, const string &str, char delim, string::size_type offset )
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
					String::Split( output, str, delim, delimPos + 1 );
				}
			}
			return output;
		}

		vector<string> & Split( vector<string> &output, const string &str, const string &delim, string::size_type offset )
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
					String::Split( output, str, delim, delimPos + delim.length() );
				}
			}
			return output;
		}

		vector<string> & Split( vector<string> &output, const string &str, const vector<string> &delim, string::size_type offset )
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
					String::Split( output, str, delim, firstDelimPos + delim[delimRef].length() );
				}
			}
			return output;
		}
		
		string Trim( const string &str )
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

		string & ToLowerCase( string &output, const string &str )
		{
			int length = (int)str.length();
			output.resize( length );
			for( int i = 0; i < length; ++i )
				output[i] = ::std::tolower( str[i], ::std::locale() );
			return output;
		}

		string & ToLowerCase( string &str )
		{
			int length = (int)str.length();
			for( int i = 0; i < length; ++i )
				str[i] = ::std::tolower( str[i], ::std::locale() );
			return str;
		}

		string & ToUpperCase( string &output, const string &str )
		{
			int length = (int)str.length();
			output.resize( length );
			for( int i = 0; i < length; ++i )
				output[i] = ::std::toupper( str[i], ::std::locale() );
			return output;
		}

		string & ToUpperCase( string &str )
		{
			int length = (int)str.length();
			for( int i = 0; i < length; ++i )
				str[i] = ::std::toupper( str[i], ::std::locale() );
			return str;
		}

		string & ExtractDirPath( string &output, const string &file, char dirDelimeter )
		{
			string d = " ";
			d[0] = dirDelimeter;
			return String::ExtractDirPath( output, file, d );
		}

		string & ExtractDirPath( string &output, const string &file, const string &dirDelimeter )
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

		string & ReplaceCharacters( string &str, char characterToReplace, char newCharacter, const string::size_type &offset, const string::size_type &end )
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

		// wstring

		vector<wstring> & Split( vector<wstring> &output, const wstring &str, char delim, wstring::size_type offset )
		{
			if( str.length() > 0 )
			{
				while( offset < str.length() ) // trim
				{
					if( str[offset] == delim )
						++offset;
					else break;
				}

				wstring::size_type delimPos = str.find_first_of( delim, offset );
				if( delimPos == wstring::npos )
				{
					if( str.length() > offset )
						output.push_back( str.substr( offset, str.length() - offset ) );
				}
				else
				{
					if( delimPos > offset )
						output.push_back( str.substr( offset, delimPos - offset ) );
					String::Split( output, str, delim, delimPos + 1 );
				}
			}
			return output;
		}

		vector<wstring> & Split( vector<wstring> &output, const wstring &str, const wstring &delim, wstring::size_type offset )
		{
			if( str.length() > 0 )
			{
				wstring::size_type delimPos = str.find_first_of( delim, offset );
				if( delimPos == wstring::npos )
				{
					if( str.length() > offset )
						output.push_back( str.substr( offset, str.length() - offset ) );
				}
				else
				{
					if( delimPos > offset )
						output.push_back( str.substr( offset, delimPos - offset ) );
					String::Split( output, str, delim, delimPos + delim.length() );
				}
			}
			return output;
		}

		vector<wstring> & Split( vector<wstring> &output, const wstring &str, const vector<wstring> &delim, wstring::size_type offset )
		{
			if( str.length() > 0 )
			{
				wstring::size_type firstDelimPos = str.length(), delimPos;
				
				vector<wstring>::size_type numDelims = delim.size(), delimRef = 0;
				for( vector<wstring>::size_type i = 0; i < numDelims ; ++i )
				{
					delimPos = str.find_first_of( delim[i], offset );
					if( delimPos != wstring::npos ) if( delimPos < firstDelimPos )
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
					String::Split( output, str, delim, firstDelimPos + delim[delimRef].length() );
				}
			}
			return output;
		}

		::std::wstring & wToLowerCase( ::std::wstring &output, const ::std::wstring &str )
		{
			int length = (int)str.length();
			output.resize( length );
			for( int i = 0; i < length; ++i )
				output[i] = ::std::tolower( str[i], ::std::locale() );
			return output;
		}

		::std::wstring & wToLowerCase( ::std::wstring &str )
		{
			int length = (int)str.length();
			for( int i = 0; i < length; ++i )
				str[i] = ::std::tolower( str[i], ::std::locale() );
			return str;
		}

		//To wstring

		::std::wstring & StringToWstring( const ::std::string &str, ::std::wstring &wstr )
		{
			const char *orig = str.c_str();

			// Convert to a wchar_t*
			size_t origsize = strlen(orig) + 1;
			const size_t newsize = 255;
			size_t convertedChars = 0;
			wchar_t wcstring[newsize];
			mbstowcs_s(&convertedChars, wcstring, origsize, orig, _TRUNCATE);
			wstr = wcstring;
			//wcscat_s(wcstring, L" (wchar_t *)");
			return wstr;
		}

		::std::string & WStringToString( const ::std::wstring &wstr, ::std::string &str )
		{
			const wchar_t* orig = wstr.c_str();
			// Convert to a char*
			size_t origsize = wcslen(orig) + 1;
			const size_t newsize = 255;
			size_t convertedChars = 0;
			char nstring[newsize];
			wcstombs_s(&convertedChars, nstring, origsize, orig, _TRUNCATE);
			str = nstring;
			//strcat_s(nstring, " (char *)");
			return str;
		}
	}

	// STREAM ////////////////////////////////////////////////////////////
	namespace Stream
	{
		float* ReadFloats( float *output, ::std::istream &input, unsigned int numFloats )
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