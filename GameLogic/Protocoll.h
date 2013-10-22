#pragma once
#include "OysterMath.h"

using Oyster::Math::Float3;

struct KeyFrame
{
	//header
	//numobj cap?500

	//per object
	//id+rotationvec+movevec+ forwardvec+upvec+pos+typeID
	
	//per particle
	//pos1,pos2,EffectID


	//per player
	//hp, shield, specialAmmo?
	int numObjects,numParticles;

	struct Player
	{
		unsigned char Hp,Shield,Special;
	}Players[8];


	struct Object
	{
		unsigned int id, TypeId;
		Float3 RotationVec;
		Float3 MoveVec;
		Oyster::Math::Float4x4 World;
	}Objects[500];

	struct Effect
	{
		Float3 Head,Tail;
		int EffectType;
	}Particle[160];

	//return number of bytes from start to send over network
	//set numofobjects
	void Pack(char * startPointer);

	int GetSize();



	KeyFrame();

	//unpacks the char array as if it was a compressed keyframe
	KeyFrame(char* data);
};

#include <string>
#include <vector>
#include <map>
#include "OysterMath.h"

namespace Protocol
{
	class Mesh
	{
	public:
		Mesh( const ::std::string &objFile = "" );

		void setObjFile( const ::std::string &fileName );

		const ::std::string & getObjFileName( ) const;
		unsigned int getRequiredBufferSize( ) const;

		void fillBuffer( void *targetBuffer ) const;
		void readFromBuffer( const void *buffer );

	private:
		::std::string objFile;
	};

	class Object
	{
	public:
		Object( unsigned int meshRef = 0, const ::Oyster::Math::Float4x4 &world = ::Oyster::Math::Float4x4::identity );

		void setMeshRef( unsigned int ref );
		void setWorldMatrix( const ::Oyster::Math::Float4x4 &matrix );

		unsigned int getMeshRef( ) const;
		const ::Oyster::Math::Float4x4 & getWorldMatrix( ) const;	
		unsigned int getRequiredBufferSize( ) const;

		void fillBuffer( void *targetBuffer ) const;
		void readFromBuffer( const void *buffer );

	private:
		unsigned int meshRef;
		::Oyster::Math::Float4x4 world;
	};

	class MapRenderData
	{
	public:
		MapRenderData( );

		void clear( );
		void add( const ::Oyster::Math::Float4x4 &worldMatrix, const ::std::string &objFile );

		const ::std::vector<Mesh> & getMeshList( ) const;
		const ::std::vector<Object> & getObjectList( ) const;
		unsigned int getRequiredBufferSize( ) const;

		void fillBuffer( void *targetBuffer ) const;
		void readFromBuffer( const void *buffer );

	private:
		unsigned int requiredBufferSize;
		::std::vector<Mesh> mesh;
		::std::vector<Object> object;
		::std::map<::std::string, unsigned int> index;
	};

	class PlayerRenderData
	{
	public:
		PlayerRenderData( );

		void clear( );
		void addPlayer( const ::std::string &objFile );

		const ::std::vector<Mesh> & getMeshList( ) const;
		const ::std::vector<unsigned int> & getPlayerMeshRefList( ) const;
		unsigned int getRequiredBufferSize( ) const;

		void fillBuffer( void *targetBuffer ) const;
		void readFromBuffer( const void *buffer );

	private:
		unsigned int requiredBufferSize;
		::std::vector<Mesh> mesh;
		::std::vector<unsigned int> playerMeshRef;
		::std::map<::std::string, unsigned int> index;
	};

	class RenderData
	{
	public:
		MapRenderData mapData;
		PlayerRenderData playerData;

		RenderData( );

		inline void RenderData::clear( )
		{ this->mapData.clear(); this->playerData.clear(); }

		inline unsigned int RenderData::getRequiredBufferSize( ) const
		{ return this->mapData.getRequiredBufferSize() + this->playerData.getRequiredBufferSize(); }

		void fillBuffer( void *targetBuffer ) const;
		void readFromBuffer( const void *buffer );
	};
}