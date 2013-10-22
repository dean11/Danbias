/////////////////////////////////////////////////////////////////////
// Created by Dan Andersson 2013
/////////////////////////////////////////////////////////////////////

#include "Session.h"
#include "Bullet.h"
#include "GameLUA.h"
#include <fstream>
#include <sstream>
#include <random>

/*
// BENCHMARK BLOCK
#include "WinTimer.h"
#include <iostream>
namespace Benchmark
{
	struct
	{
		double averageTime, totalTime, minTime, maxTime; unsigned int numSamples;
	} timerData[10] = { 0.0f, 0.0f, ::std::numeric_limits<double>::max(), -::std::numeric_limits<double>::max(), 0 };

	void sampleTime( const ::Utility::WinTimer &timer, unsigned char ref )
	{
		double elapsedTime = timer.getElapsedSeconds();
		timerData[ref].totalTime += elapsedTime;
		timerData[ref].minTime = ::Utility::Value::min( timerData[ref].minTime, elapsedTime );
		timerData[ref].maxTime = ::Utility::Value::max( timerData[ref].maxTime, elapsedTime );
		++timerData[ref].numSamples;
		timerData[ref].averageTime = timerData[ref].totalTime / (double) timerData[ref].numSamples;
	}

	void print( )
	{
		::std::ofstream file;
		file.open( "BenchMarkData.txt", ::std::ios_base::app | ::std::ios_base::out );

		if( file.is_open() )
		{
			file << "minTime\t\t: maxTime\t: averageTime\t\ttotalTime\tnumSamples\n";
			for( unsigned char i = 0; i < 3; ++i )
				file << timerData[i].minTime << (timerData[i].minTime == 0.0f ? "\t\t: " : "\t: ") << timerData[i].maxTime << "\t: " << timerData[i].averageTime  << "\t\t" << timerData[i].totalTime << '\t' << timerData[i].numSamples <<'\n';
			file << ::std::endl;
			file.close();
			::std::cout << "Benchmark data saved." << ::std::endl;
		}
	}
}
// END BENCHMARK BLOCK /**/

using namespace ::GameLogic;
using namespace ::Oyster::Math;
using namespace ::Oyster::Collision;
using namespace ::Oyster::Game;
using ::std::string;
using ::std::set;
using ::std::vector;
using ::std::ifstream;
using ::std::istream;
using ::std::stringstream;

namespace PrivateStatic
{
	enum Channel { Player, Static };

	inline Float3 getPositionOf( const Object &object )
	{ return object.getOrientation().v[3].xyz; }

	inline Float getPhysicalRangeOf( const Object &object )
	{ return object.getSize().length() * 0.5f; }

	inline Sphere getBubbleOf( const Object &object )
	{ return Sphere( getPositionOf(object), getPhysicalRangeOf(object) ); }

	void append( vector<unsigned int> &destination, const vector<unsigned int> &source )
	{
		for( vector<unsigned int>::size_type i = 0; i < source.size(); ++i )
			destination.push_back( source[i] );
	}

	void readOrientation( Float4x4 &output, istream &stream, const Float4x4 &transform )
	{
		output = Float4x4::identity;

		::Utility::Stream::readFloats( output.v[3], stream, 3 ); // position
		::Utility::Stream::readFloats( output.v[2], stream, 3 ); // forward
		::Utility::Stream::readFloats( output.v[1], stream, 3 ); // up

		transformVector( output.v[3], output.v[3], transform ); // position

		if( output.v[2].xyz != Float3::null && output.v[1].xyz != Float3::null )
		{
			// forward
			transformVector( output.v[2], output.v[2], transform );
			output.v[2].xyz.normalize();
		
			// up
			transformVector( output.v[1], output.v[1], transform );
			output.v[1].xyz -= vectorProjection( output.v[1].xyz, output.v[2].xyz );
			output.v[1].xyz.normalize();

			// right
			output.v[0].xyz = output.v[1].xyz.cross( output.v[2].xyz ).getNormalized();
		}
		else
		{
			output.v[0].xyz = Float3::standardUnitX;
			output.v[1].xyz = Float3::standardUnitY;
			output.v[2].xyz = Float3::standardUnitZ;
		}
	}

	Session::InitResult loadWorldFile( vector<Float4x4> &spawnPoints, vector<Object> &mapObjects, string &name, string &description, const string &fileName, const Float4x4 &transform )
	{
		spawnPoints.resize( 0 );
		spawnPoints.reserve( 128 );
		mapObjects.resize( 0 );
		mapObjects.reserve( 1024 );

		string workingDir;
		::Utility::String::extractDirPath( workingDir, fileName, '\\' );

		ifstream file( fileName );
		if( file.is_open() )
		{
			string str;
			vector<string> splitString;
			stringstream subStream;

			if( !file.eof() ) ::std::getline( file, name );
			if( !file.eof() ) ::std::getline( file, str ); // gonna skip the handlername
			if( !file.eof() )
			{
				::std::getline( file, description );
				::Utility::String::split( splitString, description, "@@" );
				for( vector<string>::size_type i = 0; i < splitString.size(); ++i )
				{
					if( i != 0 ) subStream << '\n';
					subStream << ::Utility::String::trim( splitString[i] );
				}

				description = subStream.str();
				
				subStream = stringstream();
			}

			while( !file.eof() )
			{
				::std::getline( file, str );

				if( str.length() == 0 ) continue;
				if( str[0] == '#' ) continue;

				splitString = vector<string>();
				::Utility::String::split( splitString, str, '#' );
				
				if( splitString.size() == 0 ) continue;
				if( splitString[0].length() == 0 ) continue;

				subStream = stringstream( splitString[0] );
				subStream >> str;
				::Utility::String::toLowerCase( str );

				if( str == "obj" )
				{
					vector<unsigned int> notUsed;
					subStream >> str;
					Object::importEntities( notUsed, workingDir + ::Utility::String::replaceCharacters(str, '/', '\\') + ".entity", transform );
				}
				else if( str == "o" )
				{
					subStream >> str;
					string lowerCased = str;
					::Utility::String::toLowerCase( lowerCased );
					if( lowerCased == "spawnpoint" )
					{
						vector<Float4x4>::size_type i = spawnPoints.size();
						spawnPoints.resize( i + 1 );
						readOrientation( spawnPoints[i], subStream, transform );
					}
					else
					{
						Float4x4 orientation;
						readOrientation( orientation, subStream, transform );
						
						vector<Object>::size_type i = mapObjects.size();
						mapObjects.resize( i + 1 );
						
						mapObjects[i].setConfig( str );
						if( mapObjects[i].getConfigID() == Object::invalidID )
						{ // config handle not defined
							mapObjects.resize( i );
						}
						else
						{
							Float3 scale = Float3( 1.0f );
							::Utility::Stream::readFloats( scale, subStream, 3 );

							mapObjects[i].setScaling( scale );
							mapObjects[i].setOrientation( orientation );
						}
					}
				}
			}

			file.close();
			return Session::Success;
		}

		return Session::Failure;
	}
}

Session::Session( )
	: world(), effect(), objectIDLib(), state(MissingMap), player(), elapsedUpdatePeriod(0.0f),
	spawnPoint(), staticObjects(), name(), description(), spawnPointOBJFile(), eventList(), nrPlayers(1), killsRequired(1) {}

Session::~Session( )
{
	if ( this->effect.size() > 0 )
	{
		set<EffectCarrier*>::iterator i = this->effect.begin(),
									  end = this->effect.end();
		for( ; i != end; ++i )
			delete (*i);
	}
}

Session::InitResult Session::load( const string &cMapFile )
{
	this->state = Loading;

	GameLUA::init();

	this->effect = std::set<EffectCarrier*>();
	this->objectIDLib = ::std::map<unsigned int, Object*>();

	
	Float4x4 transform;
	{
		Float scale = 0.1f;
		rotationMatrix_AxisY( transform, ::Utility::Value::radian(180.0f) );
		transformMatrix( transform, scale * Float4x4::identity, transform );
		transform.m44 = 1.0f;
	}

	string mapFile(cMapFile);
	::Utility::String::replaceCharacters( mapFile, '/', '\\' );

	// importing world content
	if( PrivateStatic::loadWorldFile( this->spawnPoint, this->staticObjects, this->name, this->description, mapFile, transform ) == Failure )
	{
		this->state = MissingMap;
		return Failure;
	}
	this->spawnPointOBJFile = Object::getConfig( "spawnpoint" ).objFileName;

	// importing player character content
	vector<unsigned int> terrariConfigs, eldarishConfigs;
	{
		srand(time(0));
		const char *terrariShipFiles[] = { "ship_TerrariCruiser.entity", "ship_TerrariInterceptor.entity", "ship_TerrariSniper.entity", "ship_TerrariSupport.entity" };
		const char *eldarishShipFiles[] = { "ship_EldarishCruiser.entity", "ship_EldarishInterceptor.entity", "ship_EldarishSniper.entity", "ship_EldarishSupport.entity" };

		string workingDir;
		::Utility::String::extractDirPath( workingDir, mapFile, '\\' );
		workingDir += "..\\entities\\";

		vector<unsigned int> configIDOutput;

		for( unsigned int i = 0; i < ::Utility::StaticArray::numElementsOf(terrariShipFiles); ++i )
		{
			if( Object::importEntities( configIDOutput, workingDir + terrariShipFiles[i], transform ) == Object::Failure )
				return Failure;
			PrivateStatic::append( terrariConfigs, configIDOutput );
		}

		for( unsigned int i = 0; i < ::Utility::StaticArray::numElementsOf(eldarishShipFiles); ++i )
		{
			if( Object::importEntities( configIDOutput, workingDir + eldarishShipFiles[i], transform ) == Object::Failure )
				return Failure;
			PrivateStatic::append( eldarishConfigs, configIDOutput );
		}
	}

	// resetting the physical world
	this->world.emptyAndReInit( ::Utility::StaticArray::numElementsOf(this->player) + (unsigned int)this->staticObjects.size(), 2 );

	// introduce players
	for( unsigned char i = 0; i < ::Utility::StaticArray::numElementsOf(this->player); ++i )
	{
		this->player[i].session = this;
		this->player[i].setPlayerID(i);
		this->objectIDLib[this->player[i].getObjectID()] = &this->player[i];

		if( i % 2 == 0 )
		{ // lives and dies for the eldarish henceforth!
			this->player[i].setConfig( eldarishConfigs[::std::rand() % eldarishConfigs.size()] );
			this->player[i].setTeam( 0 );
		}
		else
		{ // lives and dies for the terrari henceforth!
			this->player[i].setConfig( terrariConfigs[::std::rand() % terrariConfigs.size()] );
			this->player[i].setTeam( 1 );
		}
		
		this->player[i].setPhysicsID( this->world.addEntity( PrivateStatic::Player, &this->player[i], PrivateStatic::getBubbleOf(this->player[i]) ) );		
		this->player[i].moveToLimbo();

		this->player[i].setPrimaryWeapon(Weapon::Test);
		//this->player[i].setPrimaryWeapon(Weapon::Shotgun);
	}

	// introduce static objects
	for( vector<Object>::size_type i = 0; i < this->staticObjects.size(); ++i )
	{
		this->objectIDLib[this->staticObjects[i].getObjectID()] = &this->staticObjects[i];
		this->staticObjects[i].setPhysicsID( this->world.addEntity( PrivateStatic::Static, &this->staticObjects[i], PrivateStatic::getBubbleOf(this->staticObjects[i]) ) );
		this->staticObjects[i].releaseFromLimbo();
	}	

	this->state = Running;
	return Success;
}

unsigned char Session::getNumMaxPlayers( ) const
{ return ::Utility::StaticArray::numElementsOf(this->player); }

Player & Session::accessPlayer( unsigned int id )
{
	return this->player[id];
}

void Session::putInWorld( unsigned char playerID )
{ this->player[playerID].releaseFromLimbo(); }

void Session::putInLimbo( unsigned char playerID )
{ this->player[playerID].moveToLimbo(); }

void Session::addEffect( const EffectCarrier &effect )
{ this->effect.insert( effect.clone() ); }

const Session::State & Session::update( const Float &deltaTime )
{
	float timeSlice = MoveAble::getDiscreteTimeSlice();
	this->elapsedUpdatePeriod += deltaTime;
	this->state = Idle;

	// BENCHMARK BLOCK
	//::Utility::WinTimer updateTimer;
	// END BENCHMARK BLOCK

	while( this->elapsedUpdatePeriod >= timeSlice )
	{ // only perform update at a certain discrete updatePeriodicy. Several times if needing to catch up
		this->elapsedUpdatePeriod -= timeSlice;
		this->state = Updated;

		// BENCHMARK BLOCK
		//::Utility::WinTimer physicsTimer;
		// END BENCHMARK BLOCK

		{ // Update Effects
			bool effectErased;
			set<EffectCarrier*>::iterator i = this->effect.begin(),
										  end = this->effect.end();
			while(i != end)
			{
				effectErased = false;

				switch( (*i)->update( timeSlice ) )
				{
				case EffectCarrier::Dead:
					delete (*i);
					i = this->effect.erase( i );
					effectErased = true;

					break;
				case EffectCarrier::Armed:
					{
						switch( (*i)->getType() )
						{
						case EffectCarrier::Bullet:
							{
								Bullet::BulletCollector collector( (const Line*)(*i)->getVolumeOfEffect(), ::Utility::StaticArray::numElementsOf(this->player) );
								unsigned int channel[] = { (unsigned int)Object::Player, (unsigned int)Object::Static };
					
								this->world.visit( channel, ::Utility::StaticArray::numElementsOf(channel), (*i)->getVolumeOfEffect(), &collector ); 
								if( collector.hitObjects.size() > 0 )
									(*i)->onHit( collector.hitObjects, collector.hitPosW );
							}
							break;
						default: case EffectCarrier::Undefined: break;
						}
					}
					break;
				default: break;
				}

				if (!effectErased)
					i++;
			}
		}

		// BENCHMARK BLOCK
		//Benchmark::sampleTime( physicsTimer, 1 );
		//physicsTimer.reset();
		// END BENCHMARK BLOCK

		{ // Update Players
			// Iterating Player vs Object Collision physics
			unsigned int channel[] = { (unsigned int)Object::Player, (unsigned int)Object::Static };
			for( unsigned int i = 0; i < ::Utility::StaticArray::numElementsOf(this->player); ++i )
				if( this->player[i].isActive() )
					this->world.visit( channel, ::Utility::StaticArray::numElementsOf(channel), &this->player[i].body, &this->player[i], &Object::onCollisionAction ); 

			// Apply accumulated physics and move players
			for( unsigned int i = 0; i < ::Utility::StaticArray::numElementsOf(this->player); ++i )
			{
				if( this->player[i].isActive() )
					this->player[i].update( timeSlice );
				else if( i < nrPlayers )
					this->player[i].spawn();
			}
		}

		// BENCHMARK BLOCK
		//Benchmark::sampleTime( physicsTimer, 2 );
		// END BENCHMARK BLOCK

		// TODO: recommended placement for powerUp manager update here

		// move everything to their proper collision positions
		this->world.update();
	}
	// How do we know when the game is over?

	//This is how you end the game
	/*
	//GameEnded constructors takes the winning team ID as a variable
	Event::GameEnded* test = new Event::GameEnded(0);
	for (int i=0; i<PLAYERE_MAX_COUNT; i++)
	{
		//Get score from players
		//test->setScore(i, player[i].getNumKills(), player[i].getNumDeaths(), player[i].getNumTeamKills());
		test->setScore(i, rand()%15, rand()%10, rand()%5);
	}
	//Sort the score properly (should be done clientside instead really)
	test->sortScore();
	//Add it onto the event queue
	eventList.push_back(test);
	*/

	//Client will now realize that the game is over once this packet is received
	
	//printf("Exiting from update\n");

	// BENCHMARK BLOCK
	//Benchmark::sampleTime( updateTimer, 0 );	
	// END BENCHMARK BLOCK

	// BENCHMARK BLOCK
	//if( Benchmark::timerData[0].numSamples % 1000 == 0 )
	//	Benchmark::print();
	// END BENCHMARK BLOCK
	//Has a team won? Return -1 if not
	int ct = CheckWinConditions();
	if( -1 < ct )
	{
		Event::GameEvent *evt = new Event::GameEnded( ct );
		addEvent( evt );
		
		this->state = Over;
	}
	return this->state;
}

void Session::writeToKeyFrame( KeyFrame &buffer )
{
	// resetting buffer
	buffer.numObjects = buffer.numParticles = 0;

	// adding the Players
	int numPlayers = ::Utility::Value::min( ::Utility::StaticArray::numElementsOf( this->player ),
											::Utility::StaticArray::numElementsOf( buffer.Players ) );
	for( int i = 0; i < numPlayers; ++i )
	{
		buffer.Players[i].Hp = this->player[i].hullPoints;
		buffer.Players[i].Shield = this->player[i].shieldPoints;
		buffer.Players[i].Special = this->player[i].specialID;

		if( this->player[i].writeToKeyFrame( buffer ) == Object::Failure )
			break;
	}

	// adding the particles
	set<EffectCarrier*>::const_iterator i = this->effect.begin(),
										end = this->effect.end();
	for( ; i != end; ++i )
		if( (*i)->writeToKeyFrame( buffer ) == EffectCarrier::Failure )
			break;
}

void Session::writeToRenderResourceData( ::Protocol::RenderData &data )
{
	data.clear();
	if( this->state == Session::Running )
	{
		for( vector<Object>::size_type i = 0; i < this->staticObjects.size(); ++i )
		{ if( Object::getConfig( this->staticObjects[i].getConfigID() ).objFileName.length() > 0 )
			data.mapData.add( this->staticObjects[i].getWorldPointMatrix(), Object::getConfig(this->staticObjects[i].getConfigID()).objFileName );
		}

		if( this->spawnPointOBJFile.length() > 0 )
		{ for( vector<Float4x4>::size_type i = 0; i < this->spawnPoint.size(); ++i )
			data.mapData.add( this->spawnPoint[i], this->spawnPointOBJFile );
		}

		for( unsigned int i = 0; i < ::Utility::StaticArray::numElementsOf(this->player); ++i )
			data.playerData.addPlayer( Object::getConfig( this->player[i].getConfigID() ).objFileName );
	}
}

void Session::addEvent(Event::GameEvent *gameEvent)
{
	eventList.push_back(gameEvent);
}

void Session::fetchEvents(std::vector<Event::GameEvent*> &list)
{
	list=eventList;
	eventList.resize(0);
}
void Session::fetchEffectData(std::vector<Network::EffectData> &effData)
{
	// adding the effects
	//int test= effect.size();
	//effData.resize(effect.size());
	//effData.
	//int count=0;
	effData.resize(0);
	Network::EffectData tmp;
	set<EffectCarrier*>::const_iterator i = this->effect.begin(),
										end = this->effect.end();
	for( ; i != end; ++i )
	{
		if( (*i)->writeToNetEffect( tmp ) != EffectCarrier::Failure )
		{
			//effData[count]=tmp;
			effData.push_back(tmp);
			//count++;
			//break;
		}
	}
}
int Session::CheckWinConditions()
{
	int count=0;
	for (int i=0; i<nrPlayers; i+=2)
	{
		count+=player[i].killCount;
	}
	if (count>=killsRequired)
	{
		return 0;
	}
	count=0;
	for (int i=1; i<nrPlayers; i+=2)
	{
		count+=player[i].killCount;
	}
	if (count>=killsRequired)
	{
		return 1;
	}
	return -1;
}