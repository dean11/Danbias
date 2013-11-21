#ifndef OBJREADER_H
#define OBJREADER_H
#include "..\..\Misc\Utilities.h"
#include "..\..\OysterMath\OysterMath.h"

//#include <fstream>



class OBJReader
{
	public:
		struct OBJFormat
		{
			Oyster::Math::Float3 _d3VertexCoord;
			Oyster::Math::Float2 _d3VertexTexture;
			Oyster::Math::Float3 _d3VertexNormal;
		};

		struct OBJMaterialData
		{
			std::string	_name;
			std::string	_mapKd;
			float	_kd[3];
			float	_ka[3];
			float	_tf[3];
			float	_ni;

			OBJMaterialData()
			{
				_name	= " ";
				_mapKd	= " ";
			}
		};
		std::vector<OBJFormat> _myOBJ;
	private:
		
		std::vector<Oyster::Math::Float3> _mVertexCoord, _mVertexNormal;
		std::vector<Oyster::Math::Float2> _mVertexTexture;

		int _mNrOfCoords, _mNrOfNormals, _mNrOfTexels, _mNrOfFaces;
		int _mPos, _mNormal, _mTexel;
		void stringSplit( std::string strToSplit );
		void addToOBJarray();

	public:
		OBJReader();
		~OBJReader();

		void readOBJFile( std::wstring fileName);

};
#endif