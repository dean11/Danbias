#include "Protocoll.h"

KeyFrame::KeyFrame()
{
	numObjects=numParticles=0;
}

KeyFrame::KeyFrame(char* data)
{
	int i = 0;
	int size= sizeof(int);
	int* nums = &this->numObjects;
	memcpy(&numObjects,data,size);
	i+=size;
	memcpy(&numParticles,data+i,size);
	i+=size;
	size=sizeof(Player);

	memcpy(&Players,data+i,size*8);
	i+=size*8;

	size = sizeof(Object);
	memcpy(&Objects,data+i,size*numObjects);
	i+=size*numObjects;

	size = sizeof(Effect);
	memcpy(&Particle,&(data[i]),size*numParticles);
	i+=size;
}

int KeyFrame::GetSize()
{
	int i=1;
	i+=sizeof(int)*2;
	i+=sizeof(Player)*8;
	i+=sizeof(Object)*numObjects;
	i+=sizeof(Effect)*numParticles;
	return i;
}

void KeyFrame::Pack(char * startPointer)
{
	int i=0;
	i+=sizeof(int)*2;

	memcpy(startPointer,this,sizeof(int)*2);
	memcpy(startPointer+i,this->Players,sizeof(Player)*8);
	i+=sizeof(Player)*8;
	memcpy(startPointer+i,this->Objects,sizeof(Object)*numObjects);
	i+=sizeof(Object)*numObjects;
	memcpy(startPointer+i,this->Particle,sizeof(Effect)*numParticles);
}

namespace Protocol
{
	using ::std::string;
	using ::std::vector;
	using ::std::map;
	using ::Oyster::Math::Float4x4;

	Mesh::Mesh( const string &_objFile ) : objFile(_objFile) {}

	void Mesh::setObjFile( const string &fileName )
	{ this->objFile = fileName; }

	const string & Mesh::getObjFileName( ) const
	{ return this->objFile; }

	unsigned int Mesh::getRequiredBufferSize( ) const
	{ return (unsigned int)(this->objFile.length() + 1); }

	void Mesh::fillBuffer( void *targetBuffer ) const
	{
		char *buffer = (char*)targetBuffer;

		for( unsigned int i = 0; i < this->objFile.length(); ++i )
			buffer[i] = this->objFile[i];
		buffer[this->objFile.length()] = '\0';
	}

	void Mesh::readFromBuffer( const void *buffer )
	{ this->objFile = (const char*)buffer; }

	Object::Object( unsigned int _meshRef, const Float4x4 &_world )
		: meshRef(_meshRef), world(_world) {}

	void Object::setMeshRef( unsigned int ref )
	{ this->meshRef = ref; }

	void Object::setWorldMatrix( const Float4x4 &matrix )
	{ this->world = matrix; }

	unsigned int Object::getMeshRef( ) const
	{ return this->meshRef; }

	const Float4x4 & Object::getWorldMatrix( ) const
	{ return this->world; }

	unsigned int Object::getRequiredBufferSize( ) const
	{ return (unsigned int)(sizeof(unsigned int) + sizeof(Float4x4)); }

	void Object::fillBuffer( void *targetBuffer ) const
	{
		char *cBuffer = (char*)targetBuffer;
		unsigned int *uiBuffer = (unsigned int*)targetBuffer;
	
		uiBuffer[0] = this->meshRef;

		cBuffer += sizeof(unsigned int);

		for( unsigned int i = 0; i < sizeof(Float4x4); ++i )
			cBuffer[i] = this->world.byte[i];
	}

	void Object::readFromBuffer( const void *buffer )
	{
		const char *cBuffer = (const char*)buffer;
		const unsigned int *uiBuffer = (const unsigned int*)buffer;
	
		this->meshRef = uiBuffer[0];

		cBuffer += sizeof(unsigned int);

		for( unsigned int i = 0; i < sizeof(Float4x4); ++i )
			this->world.byte[i] = cBuffer[i];
	}

	MapRenderData::MapRenderData( )
		: mesh(), object(), index()
	{ this->requiredBufferSize = 2 * sizeof(unsigned int); }

	void MapRenderData::clear( )
	{
		this->requiredBufferSize = 2 * sizeof(unsigned int);
		this->mesh.clear();
		this->object.clear();
		this->index.clear();
	}

	void MapRenderData::add( const Float4x4 &worldMatrix, const string &objFile )
	{
		unsigned int ref;
		map<string, unsigned int>::const_iterator i = this->index.find( objFile );
		if( i == this->index.end() )
		{
			ref = this->index[objFile] = (unsigned int)this->mesh.size();
			this->mesh.push_back( Mesh(objFile) );
			this->requiredBufferSize += this->mesh[ref].getRequiredBufferSize();
		}
		else ref = i->second;

		this->object.push_back( Object(ref, worldMatrix) );
		this->requiredBufferSize += this->object[this->object.size()-1].getRequiredBufferSize();
	}

	const vector<Mesh> & MapRenderData::getMeshList( ) const
	{ return this->mesh; }

	const vector<Object> & MapRenderData::getObjectList( ) const
	{ return this->object; }

	unsigned int MapRenderData::getRequiredBufferSize( ) const
	{ return this->requiredBufferSize; }

	void MapRenderData::fillBuffer( void *targetBuffer ) const
	{
		unsigned int *uiBuffer = (unsigned int*)targetBuffer;
		uiBuffer[0] = (unsigned int)this->mesh.size();
		uiBuffer[1] = (unsigned int)this->object.size();

		char *cBuffer = (char*)&uiBuffer[2];

		for( vector<Mesh>::size_type i = 0; i < this->mesh.size(); ++i )
		{
			this->mesh[i].fillBuffer( cBuffer );
			cBuffer += this->mesh[i].getRequiredBufferSize();
		}

		for( vector<Object>::size_type i = 0; i < this->object.size(); ++i )
		{
			this->object[i].fillBuffer( cBuffer );
			cBuffer += this->object[i].getRequiredBufferSize();
		}
	}

	void MapRenderData::readFromBuffer( const void *buffer )
	{
		this->clear();

		const unsigned int *uiBuffer = (const unsigned int*)buffer;
		const char *cBuffer = (const char*)&uiBuffer[2];

		this->mesh.reserve( uiBuffer[0] );
		for( unsigned int i = 0; i < uiBuffer[0]; ++i )
		{
			vector<Mesh>::size_type ref = this->mesh.size();
			this->mesh.push_back( Mesh() );
			this->mesh[ref].readFromBuffer( cBuffer );
			this->requiredBufferSize += this->mesh[ref].getRequiredBufferSize();
			cBuffer += this->mesh[ref].getRequiredBufferSize();

			this->index[this->mesh[ref].getObjFileName()] = (unsigned int)ref;
		}

		this->object.reserve( uiBuffer[1] );
		for( unsigned int i = 0; i < uiBuffer[1]; ++i )
		{
			vector<Object>::size_type ref = this->object.size();
			this->object.push_back( Object() );
			this->object[ref].readFromBuffer( cBuffer );
			this->requiredBufferSize += this->object[ref].getRequiredBufferSize();
			cBuffer += this->object[ref].getRequiredBufferSize();
		}
	}


	PlayerRenderData::PlayerRenderData( )
		: mesh(), playerMeshRef(), index()
	{ this->requiredBufferSize = (unsigned int)(2 * sizeof(unsigned int)); }

	void PlayerRenderData::clear( )
	{
		this->requiredBufferSize = (unsigned int)(2 * sizeof(unsigned int));
		this->mesh.clear();
		this->playerMeshRef.clear();
		this->index.clear();
	}

	void PlayerRenderData::addPlayer( const string &objFile )
	{
		unsigned int ref;
		map<string, unsigned int>::const_iterator i = this->index.find( objFile );
		if( i == this->index.end() )
		{
			ref = this->index[objFile] = (unsigned int)this->mesh.size();
			this->mesh.push_back( Mesh(objFile) );		
			this->requiredBufferSize += this->mesh[ref].getRequiredBufferSize();
		}
		else ref = i->second;

		this->playerMeshRef.push_back( ref );
		this->requiredBufferSize += sizeof(unsigned int);
	}

	const vector<Mesh> & PlayerRenderData::getMeshList( ) const
	{ return this->mesh; }

	const vector<unsigned int> & PlayerRenderData::getPlayerMeshRefList( ) const
	{ return this->playerMeshRef; }

	unsigned int PlayerRenderData::getRequiredBufferSize( ) const
	{ return this->requiredBufferSize; }

	void PlayerRenderData::fillBuffer( void *targetBuffer ) const
	{
		unsigned int *uiBuffer = (unsigned int*)targetBuffer;
		uiBuffer[0] = (unsigned int)this->mesh.size();
		uiBuffer[1] = (unsigned int)this->playerMeshRef.size();

		char *cBuffer = (char*)&uiBuffer[2];

		for( vector<Mesh>::size_type i = 0; i < this->mesh.size(); ++i )
		{
			this->mesh[i].fillBuffer( cBuffer );
			cBuffer += this->mesh[i].getRequiredBufferSize();
		}

		uiBuffer = (unsigned int*)cBuffer;
		for( vector<unsigned int>::size_type i = 0; i < this->playerMeshRef.size(); ++i )
		{
			*uiBuffer = this->playerMeshRef[i];
			++uiBuffer;
		}
	}

	void PlayerRenderData::readFromBuffer( const void *buffer )
	{
		this->clear();

		const unsigned int *uiBuffer = (const unsigned int*)buffer;
		const char *cBuffer = (const char*)&uiBuffer[2];

		this->mesh.reserve( uiBuffer[0] );
		for( unsigned int i = 0; i < uiBuffer[0]; ++i )
		{
			vector<Mesh>::size_type ref = this->mesh.size();
			this->mesh.push_back( Mesh() );
			this->mesh[ref].readFromBuffer( cBuffer );
			this->requiredBufferSize += this->mesh[ref].getRequiredBufferSize();
			cBuffer += this->mesh[ref].getRequiredBufferSize();

			this->index[this->mesh[ref].getObjFileName()] = (unsigned int)ref;
		}

		const unsigned int *uiBuffer2 = (const unsigned int*)cBuffer;
		for( unsigned int i = 0; i < uiBuffer[1]; ++i )
		{
			this->playerMeshRef.push_back( uiBuffer2[i] );
			this->requiredBufferSize += sizeof(unsigned int);
		}
	}

	RenderData::RenderData( ) : mapData(), playerData() {}

	void RenderData::fillBuffer( void *targetBuffer ) const
	{
		char *cBuffer = (char*)targetBuffer;
		this->mapData.fillBuffer( cBuffer );
		cBuffer += this->mapData.getRequiredBufferSize();
		this->playerData.fillBuffer( cBuffer );
	}

	void RenderData::readFromBuffer( const void *buffer )
	{
		const char *cBuffer = (const char*)buffer;
		this->mapData.readFromBuffer( cBuffer );
		cBuffer += this->mapData.getRequiredBufferSize();
		this->playerData.readFromBuffer( cBuffer );
	}
}