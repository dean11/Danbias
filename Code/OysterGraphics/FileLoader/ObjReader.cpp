#include "OBJReader.h"
#include <sstream>
#include <fstream>

using namespace std;
OBJReader::OBJReader() 
{
	_mPos		= 0;
	_mNormal	= 0;
	_mTexel		= 0;
}

OBJReader::~OBJReader()
{

}

void OBJReader::readOBJFile( wstring fileName )
{
	fstream inStream;
	string typeOfData = " ";
	float vertexData;
	string face1, face2, face3;

	inStream.open( fileName, fstream::in );
	
	if( inStream.is_open() )
	{
		while( !inStream.eof() )
		{
			inStream >> typeOfData;

			if( typeOfData == "v" )
			{
				Oyster::Math::Float3 position;

				inStream >> vertexData;
				position.x = vertexData;
				
				inStream >> vertexData;
				position.y = vertexData;

				inStream >> vertexData;
				position.z = vertexData;

				_mVertexCoord.push_back( position );

			}
			else if( typeOfData == "vt" )
			{
				Oyster::Math::Float2 texel;
				inStream >> vertexData;
				texel.x = vertexData;

				inStream >> vertexData;
				texel.y = 1 - vertexData;

				_mVertexTexture.push_back( texel );
			}
			else if( typeOfData == "vn" )
			{
				Oyster::Math::Float3 normal;
				inStream >> vertexData;
				normal.x = vertexData;

				inStream >> vertexData;
				normal.y = vertexData;

				inStream >> vertexData;
				normal.z = vertexData;

				_mVertexNormal.push_back( normal );
			}
			else if( typeOfData == "f" )
			{
				inStream >> face1;
				stringSplit( face1 );
				
				addToOBJarray();

				inStream >> face2;
				stringSplit( face2 );

				addToOBJarray();

				inStream >> face3;
				stringSplit( face3 );

				addToOBJarray();
			}
		}
	}

	inStream.close();
}

//Private functions
void OBJReader::stringSplit( string strToSplit )
{
	char delim = '/';
	string vPos, vNormal, vTexel;
	stringstream aStream(strToSplit);
	getline( aStream, vPos, delim );
	getline( aStream, vTexel, delim );
	getline( aStream, vNormal );

	_mPos		= atoi( vPos.c_str() );
	_mNormal	= atoi( vNormal.c_str() );
	_mTexel		= atoi( vTexel.c_str() );

}

void OBJReader::addToOBJarray()
{
	OBJFormat temp;

	temp._d3VertexCoord		= _mVertexCoord.at( _mPos - 1 );
	temp._d3VertexNormal	= _mVertexNormal.at( _mNormal - 1 );
	temp._d3VertexTexture	= _mVertexTexture.at( _mTexel - 1 );

	_myOBJ.push_back( temp );
}