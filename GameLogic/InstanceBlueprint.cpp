/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

/* // TODO
<object "painting_0">
	<displayName>Painting</displayName>
	hitPoints = 0
	armour = 0
	centerOfMass = 0 0 0
	mass = 0
	<model>../entities/models/painting_0.obj</model>
	<description>
A painting of a brunette.@@She has brown eyes...
	</description>
	<box -0.12763351202 0.566205918789 0.7363961339 0.12763351202 -0.566205918789 -0.7363961339>
</box>
</object>
*/

#include "InstanceBlueprint.h"
#include "Utilities.h"
#include <fstream>
#include <sstream>

using namespace ::GameLogic;
using namespace ::Oyster::Math;
using ::std::string;
using ::std::vector;
using ::std::ifstream;
using ::std::stringstream;

namespace PrivateStatic
{
	bool seekNextObject( string &idNameOutput, ::std::istream &source )
	{
		string str;
		vector<string> splitString;

		while( !source.eof() )
		{
			source >> str;
			if( str.size() == 0 ) continue;
			if( str[0] == '#' )
			{ // rest of line is all commentary
				::std::getline( source, str );
				continue;
			}
			if( str[0] != '<' ) continue;

			if( str.size() == 1 )
			{ if( !source.eof() ) source >> str; }
			else str = str.substr( 1 );

			::Utility::String::toLowerCase( str );
			if( str != "object" ) continue;
			
			if( source.eof() ) return false;
			source >> str;

			::Utility::String::split( splitString, str, '\"' );
			// since str have no prefix whitespaces, we can safely assume
			// splitString[0] is the string we want. the rest is garbage.
			idNameOutput = ::Utility::String::trim( splitString[0] );
			return true;
		}
		return false;
	}

	void fillInstanceBlueprint( InstanceBlueprint &targetMem, ::std::istream &source, const Float4x4 &transform )
	{
		string line;
		vector<string> splitString;
		vector<string> tagDelims(2);
		tagDelims[0] = "<";
		tagDelims[1] = ">";

		Float4 minPoint = Float4( Float3(::std::numeric_limits<Float>::max()), 1.0f ),
			   maxPoint = Float4( Float3(-::std::numeric_limits<Float>::max()), 1.0f ),
			   centerOfMass = Float4( Float3::null, 1.0f );

		while( !source.eof() )
		{
			::std::getline( source, line );

			line = ::Utility::String::trim( line );

			if( line.length() == 0 ) continue;
			if( line[0] == '#' ) continue;

			splitString = vector<string>();
			::Utility::String::split( splitString, line, '#' );
			line = ::Utility::String::trim( splitString[0] );

			if( line.length() == 0 )
				continue; // just to be sure

			if( line[0] == '<' )
			{ // it's possibly <displayName>, <model>, <box>, <description> or </object>
				splitString = vector<string>();
				::Utility::String::split( splitString, line, tagDelims );
				line = ::Utility::String::toLowerCase( ::Utility::String::trim(splitString[0]) );
				if( line == "displayname" )
				{ // <displayName>Painting</displayName>
					targetMem.name = ::Utility::String::trim(splitString[1]);
				}
				else if( line == "model" )
				{ // <model>../entities/models/painting_0.obj</model>
					targetMem.objFileName = ::Utility::String::replaceCharacters( ::Utility::String::trim(splitString[1]), '/', '\\' );
				}
				else if( line == "description" )
				{ /* <description>
					A painting of a brunette.@@She has brown eyes...
					</description> */

					if( splitString.size() > 1 )
						line = ::Utility::String::trim( splitString[1] );
					else
					{
						if( source.eof() ) return;
						::std::getline( source, line );
					}

					splitString = vector<string>();
					::Utility::String::split( splitString, line, "@@" );
					stringstream strBuilder( splitString[0] );
					
					vector<string>::size_type numSplits = splitString.size();
					for( vector<string>::size_type i = 1; i < numSplits; ++i )
					{
						if( i > 1 ) strBuilder << '\n'; 
						strBuilder << splitString[i];
					}

					targetMem.description = strBuilder.str();
				}
				else if( line.find_first_of("box") == 0 )
				{ /* <box -0.12763351202 0.566205918789 0.7363961339 0.12763351202 -0.566205918789 -0.7363961339>
					 </box> */

					stringstream stream( line );
					stream >> line;
					if( line != "box" )
						continue;

					Float3 point;
					::Utility::Stream::readFloats( point.element, stream, 3 );
					minPoint.xyz = ::Utility::Value::min( minPoint.xyz, point );
					maxPoint.xyz = ::Utility::Value::max( maxPoint.xyz, point );

					::Utility::Stream::readFloats( point.element, stream, 3 );
					minPoint.xyz = ::Utility::Value::min( minPoint.xyz, point );
					maxPoint.xyz = ::Utility::Value::max( maxPoint.xyz, point );
				}
				else if( line == "/object" )
					break;

				continue; // no point to continue further down
			} // end of "if( line[0] == '<' )"

			splitString = vector<string>();

			::Utility::String::split( splitString, line, '=' );
			if( splitString.size() > 1 )
			{
				splitString[0] = ::Utility::String::toLowerCase( ::Utility::String::trim(splitString[0]) );
				if( splitString[0] == "hitpoints" )
				{ // hitPoints = 0
					splitString[1] = ::Utility::String::trim(splitString[1]);
					targetMem.hullPoints = ::std::atoi( splitString[1].c_str() );
				}
				else if( splitString[0] == "armour" ) // ??
				{ // armour = 0
					splitString[1] = ::Utility::String::trim(splitString[1]);
					targetMem.shieldPoints = ::std::atoi( splitString[1].c_str() );
				}
				else if( splitString[0] == "mass" )
				{ // mass = 0
					splitString[1] = ::Utility::String::trim(splitString[1]);
					targetMem.mass = (float)::std::atof( splitString[1].c_str() );
					if( targetMem.mass == 0.0f )
						targetMem.mass = ::std::numeric_limits<float>::max();
				}
				else if( splitString[0] == "centerofmass" )
				{ // centerOfMass = 0 0 0
					stringstream stream( splitString[1] );
					::Utility::Stream::readFloats( centerOfMass.element, stream, 3 );
				}
				else if( splitString[0] == "maxspeed" )
				{ // maxspeed = [  float of top speed: default 400.0 ]
					stringstream stream( splitString[1] );
					string value;
					stream >> value;
					targetMem.movementProperty.maxSpeed = (float)::std::atof( value.c_str() );
				}
				else if( splitString[0] == "deacceleration" )
				{ // deacceleration = [  float of acceleration : default 50.0 ]
					stringstream stream( splitString[1] );
					string value;
					stream >> value;
					targetMem.movementProperty.deAcceleration = (float)::std::atof( value.c_str() );
				}
				else if( splitString[0] == "forward" )
				{ // forward = [ float of acceleration : default 50.0 ]
					stringstream stream( splitString[1] );
					string value;
					stream >> value;
					targetMem.movementProperty.acceleration.forward = (float)::std::atof( value.c_str() );
				}
				else if( splitString[0] == "backward" )
				{ // backward = [ float of acceleration : default 25.0 ]
					stringstream stream( splitString[1] );
					string value;
					stream >> value;
					targetMem.movementProperty.acceleration.backward = (float)::std::atof( value.c_str() );
				}
				else if( splitString[0] == "strafe" )
				{ // strafe = [  float of acceleration : default 25.0 ]
					stringstream stream( splitString[1] );
					string sValue;
					stream >> sValue;
					float fValue = (float)::std::atof( sValue.c_str() );
					targetMem.movementProperty.acceleration.horizontal = fValue;
					targetMem.movementProperty.acceleration.vertical = fValue;
				}
				else if( splitString[0] == "turnspeed" )
				{ // turnspeed = [ float in degrees : default 90.0 ]
					stringstream stream( splitString[1] );
					string sValue;
					stream >> sValue;
					float fValue = ::Utility::Value::radian( (float)::std::atof(sValue.c_str()) );
					targetMem.rotationProperty.maxSpeed = fValue;
					targetMem.rotationProperty.deAcceleration = 2.0f * fValue;
					targetMem.rotationProperty.acceleration.pitch = fValue;
					targetMem.rotationProperty.acceleration.yaw = fValue;
					targetMem.rotationProperty.acceleration.roll = fValue;
				}
			}
		}
		
		targetMem.centerOfMass = transformVector( centerOfMass, centerOfMass, transform ).xyz; 

		if( minPoint != Float4( Float3(::std::numeric_limits<Float>::max()), 1.0f ) )
			if( maxPoint != Float4( Float3(-::std::numeric_limits<Float>::max()), 1.0f ) )
		{
			transformVector( minPoint, minPoint, transform );
			transformVector( maxPoint, maxPoint, transform );

			targetMem.hitBox.position  = (maxPoint.xyz - minPoint.xyz) * 0.5f;
			targetMem.hitBox.halfSize  = ::Utility::Value::abs( targetMem.hitBox.position );
			targetMem.hitBox.position += minPoint.xyz;
		}
	}
}

InstanceBlueprint::Result InstanceBlueprint::loadFromFile( vector<InstanceBlueprint*> &output, vector<string> &idOutput, const string &entityFile, const Float4x4 &transform )
{
	ifstream file( entityFile );

	if( file.is_open() )
	{
		string idName, workingDir;
		::Utility::String::extractDirPath( workingDir, entityFile, '\\' );

		while( PrivateStatic::seekNextObject(idName, file) )
		{
			InstanceBlueprint* blueprint = new InstanceBlueprint();
			PrivateStatic::fillInstanceBlueprint( *blueprint, file, transform );
			if( blueprint->objFileName.length() > 0 )
				blueprint->objFileName = workingDir + blueprint->objFileName;
			output.push_back( blueprint );
			idOutput.push_back( idName );
		}
		file.close( );

		return Success;
	}
	else return Failure;
}

InstanceBlueprint::InstanceBlueprint( )
	: name(), objFileName(), description(), mass(::std::numeric_limits<float>::max()), centerOfMass(Float3::null),
	  hullPoints(255), shieldPoints(255)
{
	this->hitBox.halfSize = Float3::null;
	this->hitBox.position = Float3::null;

	this->movementProperty.maxSpeed					= 400.0f;
	this->movementProperty.deAcceleration			= 50.0f;
	this->movementProperty.acceleration.forward		= 50.0f;
	this->movementProperty.acceleration.backward	= 25.0f;
	this->movementProperty.acceleration.horizontal	= 25.0f;
	this->movementProperty.acceleration.vertical	= 25.0f;

	this->rotationProperty.maxSpeed					= ::Utility::Value::radian( 90.0f );
	this->rotationProperty.deAcceleration			= ::Utility::Value::radian( 180.0f );
	this->rotationProperty.acceleration.pitch		= ::Utility::Value::radian( 90.0f );
	this->rotationProperty.acceleration.yaw			= ::Utility::Value::radian( 90.0f );
	this->rotationProperty.acceleration.roll		= ::Utility::Value::radian( 90.0f );
}