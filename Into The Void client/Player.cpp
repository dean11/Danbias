#include "Player.h"

using namespace ::Oyster::Math;

Player::Result Player::init( )
{ /* TODO */ return Player::Success; }

Player::Player( const std::string &objFilename, const Float4x4 &transform ) : renderAvatar(), logicAvatar(Oyster::Math::Float4x4::identity), orientationPointer(NULL)
{
	this->orientationPointer = const_cast<Float4x4*>(&this->logicAvatar.getOrientation());
	this->renderAvatar = new Oyster::Render::Model;
	this->renderAvatar->World = this->orientationPointer;

	if( objFilename == "" ) return;

	//Oyster::FileLoaders::ObjReader *reader = Oyster::FileLoaders::ObjReader::LoadFile(objFilename, transform);
	//Oyster::FileLoaders::ObjReader::Vertex** vertex = new Oyster::FileLoaders::ObjReader::Vertex*[1];
	//int vcount;
	//std::map<std::string,ID3D11ShaderResourceView *> textures;
	//reader->GetVertexData( vertex, vcount, textures );

	//Oyster::Buffer::BUFFER_INIT_DESC desc;
	//desc.ElementSize=sizeof(Oyster::FileLoaders::ObjReader::Vertex);
	//desc.NumElements = vcount;
	//desc.InitData = *vertex;
	//desc.Type = Oyster::Buffer::VERTEX_BUFFER;
	//desc.Usage = Oyster::Buffer::BUFFER_DEFAULT;
	//
	//ID3D11ShaderResourceView *srv = textures["Diffuse"];
	//
	//renderAvatar.Vertices = *(Oyster::Engine::Init::Buffers::CreateBuffer(desc));
	//renderAvatar.VertexCount = vcount;
	//renderAvatar.Material.push_back(srv);
	//srv = textures["Specular"];
	//renderAvatar.Material.push_back(srv);
	//srv = textures["Glow"];
	//renderAvatar.Material.push_back(srv);
	//renderAvatar.Indexed=false;
	//renderAvatar.Visible=true;

	renderAvatar = Oyster::Resources::Manager::LoadModel(objFilename, transform);

	weaponSound = SoundEngine::CreateSoundSource();
	weaponSound->SetResource("..\\Content\\Sounds\\pew_pew.wav");
	weaponSound->SetLooping(false);
	weaponSound->SetVolume(1.0f);

	this->HP=255;
	this->Score=0;
}

Player::~Player() {/* TODO ? */ }

Game::Ship & Player::accessShip( )
{
	return this->logicAvatar;
}

const Game::Ship & Player::accessShip( ) const
{
	return this->logicAvatar;
}

void Player::update( const ::Oyster::Math::Float &deltaTime, ClientToServerUpdateData &update )
{
	/*if(update.forward>0)
		this->logicAvatar.thrustForward();
	if(update.forward<0)
		this->logicAvatar.thrustBackward();

	if(update.straferight>0)
		this->logicAvatar.strafeRight();
	if(update.straferight<0)
		this->logicAvatar.strafeLeft();

	if(update.strafeup>0)
		this->logicAvatar.climb();
	if(update.strafeup<0)
		this->logicAvatar.dive();
	*/

	if(update.roll>0)
	{
		::Oyster::Math::Float baseAcceleration = this->logicAvatar.rotationProperty.acceleration.roll;
		this->logicAvatar.rotationProperty.acceleration.roll /= ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
			
		this->logicAvatar.rollLeft();
		this->logicAvatar.rotationProperty.acceleration.roll = baseAcceleration;
	}
	if(update.roll<0)
	{
		::Oyster::Math::Float baseAcceleration = this->logicAvatar.rotationProperty.acceleration.roll;
		this->logicAvatar.rotationProperty.acceleration.roll /= ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
			
		this->logicAvatar.rollRight();
		
		this->logicAvatar.rotationProperty.acceleration.roll = baseAcceleration;
	}

	/*
	if(update.braking)
		this->logicAvatar.enableMovementReduction( true );
	else
		this->logicAvatar.disableMovementReduction();
	*/
	
	this->logicAvatar.enableRotationReduction( true );
	this->logicAvatar.update( deltaTime );
}

void Player::render( ) const
{
}

SoundSource *Player::getWeaponSound() const
{
	return weaponSound;
}

const Oyster::Math::Float4x4 *Player::getOrientation()
{
	return this->orientationPointer;
}

void Player::setMutation(const Oyster::Math::Float4x4 &transform)
{
	this->logicAvatar.setMutation(transform);
}

void Player::copyLogicFrom( const Player &player )
{ this->logicAvatar = player.logicAvatar; }

std::string Player::getName() const
{
	return this->name;
}

void Player::setName(const std::string &name)
{
	this->name = name;
}
