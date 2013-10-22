#include <cassert>

#include "IntoTheVoid.h"
#include "Player.h"
#include "UserInterface.h"
#include "RenderEngine.h"
#include "SoundSystem.h"
#include "NetworkConnection.h"
#include "NetworkUpdateStructs.h"
#include "IngameMenu.h"
#include "Misc\Utilities.h"
#include "Input\InputController.h"
#include "Protocoll.h"

#include "Session.h"

using namespace ::Oyster::Math;

void loadPlayerResources( Player **player, int shipID[], const ::Protocol::RenderData &renderData, const Float4x4 &transform );

IntoTheVoid::IntoTheVoid(NetworkConnection *connection)
	: updatePeriod(1.0f), elapsedUpdatePeriod(0.0f), readOnly(false),
	  camera(NULL), isSuspended(true)
{
	assert(connection->getConnectionStatus() == ONLINE_INGAME);

	this->connection = connection;
	this->connection->setIntoTheVoid(this);
	
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		this->players[i] = NULL;
	}

	this->staticObject.num = 0;
	this->staticObject.model = NULL;
	this->staticObject.world = NULL;

	Initialized = false;
}

IntoTheVoid::~IntoTheVoid()
{
	if( this->connection )
	{
		this->connection->disconnect();
		delete this->connection;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if (this->players[i])
			delete this->players[i];
	}

	if( this->staticObject.model )
	{
		/*for( unsigned int i = 0; i < this->staticObject.num; ++i )
		{
			::std::vector<ID3D11ShaderResourceView*>::size_type j = 0;
			for( ; j < this->staticObject.model[i].Material.size(); ++i )
				this->staticObject.model[i].Material[j]->Release();
		}*/

		delete [] this->staticObject.model;
	}
	if( this->staticObject.world )
		delete [] this->staticObject.world;

	if( this->camera ) delete this->camera;
}

void IntoTheVoid::Enter(GameStateStack *stateStack)
{
	Oyster::Input::Controller::OrigoCenter();
	Oyster::Input::Controller::RestrictMouse(true);

	gameRunning=true;
	this->isSuspended = false;
	this->stateStack = stateStack;

	// setting up the update periodicy
	this->updatePeriod = 1.0f / 120.0f;
	this->elapsedUpdatePeriod = 0.0f;
	::Oyster::Game::MoveAble::setDiscreteTimeSlice( this->updatePeriod );

	this->isFirstUpdate = true;

	this->SetRenderData( *this->connection->GetRenderData() );
	delete this->connection->GetRenderData();

	this->playerID = connection->getPlayerID();
	LobbyInitData lobbyData = this->connection->getLobbyData();

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		this->players[i]->setName(lobbyData.players[i].usrName);

		this->scores[i].id = i;
		this->scores[i].kills = 0;
		this->scores[i].deaths = 0;
		this->scores[i].teamkills = 0;
	}

	// Set the local player's weapon sound to always be where the listener is. Otherwise
	// if we are we are moving fast the listener's and weapon sound's positions will slightly
	// move relative to each other every update, causing crackling to be heard.
	//this->players[this->playerID]->getWeaponSound()->SetIsRelativeToListener(true);
	//this->players[this->playerID]->getWeaponSound()->SetPosition(Float3::null.element);

	//this->self.accessShip().movementProperty = players[this->playerID]->accessShip().movementProperty;
	//this->self.accessShip().rotationProperty = players[this->playerID]->accessShip().rotationProperty;

	if( this->camera == NULL )
	{ // only create a camera if there is none there already
		
		::Oyster::Math::Float distance,
							  nearClip = 128.0f,
							  farClip = 102400.0f;

		switch( this->playerID )
		{
		case 0: distance = nearClip + 256.0f; break;
		case 1: distance = nearClip + 256.0f; break;
		case 2: distance = nearClip + 256.0f; break;
		case 3: distance = nearClip + 256.0f; break;
		case 4: distance = nearClip + 256.0f; break;
		case 5: distance = nearClip + 256.0f; break;
		case 6: distance = nearClip + 256.0f; break;
		case 7: distance = nearClip + 256.0f; break;
		default: distance = nearClip; break;
		}
		
		Float4x4 projection;
		camera = new TailCamera( players[this->playerID]->accessShip().getOrientation(), Float3(0.0f, 0.0f, -distance), Float3(0.0f, 1.0f, 0.0f), Float4x4::identity );
		::Oyster::Math::projectionMatrix_Perspective( projection, 3.14f * 0.4f,( ::Oyster::Window::Size.right / (float)::Oyster::Window::Size.top), nearClip, farClip );
		camera->setProjection( projection );
		camera->setMaxOffsetDistance( 10.0f );
		camera->setMaxOffsetRadian( ::Utility::Value::radian(20.0f) ); // +-20 degrees
	}
	connection->doneLoading();
	this->Initialized=true;
}

void IntoTheVoid::Exit()
{
	if( this->connection )
	{
		this->connection->disconnect();
		delete this->connection;
		this->connection = NULL;
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if( this->players[i] )
		{
			delete this->players[i];
			this->players[i] = NULL;
		}
	}

	if( this->camera ) { delete this->camera; this->camera = NULL; }

	this->stateStack = NULL;
	this->isSuspended = true;
}

void IntoTheVoid::Suspend()
{
	this->isSuspended = true;
}

void IntoTheVoid::Resume()
{
	Oyster::Input::Controller::OrigoCenter();
	Oyster::Input::Controller::RestrictMouse(true);

	this->isSuspended = false;
}

bool IntoTheVoid::WantsBackgroundUpdates() const
{
	return true;
}

void IntoTheVoid::Update(float timeDelta)
{
	// ignorera den första uppdateringen då timeDelta är på flera sekunder för att vi har laddat in alla resurser.
	if (this->isFirstUpdate)
	{
		for(int i =0; i<PLAYER_MAX_COUNT;++i)
			this->players[i]->accessShip().setOrientation(connection->getPosition(i));
		this->isFirstUpdate = false;
		return;
	}

	if (Oyster::Input::Controller::isKeyPressed(VK_ESCAPE))
		this->stateStack->PushState(new IngameMenu());
	
	ClientToServerUpdateData update;

	if (!this->isSuspended)
		UserInterface::Update(*camera, timeDelta, update);

	int owner;
	//Event::BulletCreated *bulletCreated;
	//Event::BulletHit *bulletHit;
	std::vector<Event::GameEvent*> eventList;
	connection->fetchEvents(eventList);

	for(size_t i = 0; i < eventList.size(); i++)
	{
		switch(Event::getEventType(eventList[i]))
		{
		case Event::eBulletCreated:
			{
			Event::BulletCreated* bulletCreated = (Event::BulletCreated *)eventList[i];
			owner = ((Event::BulletCreated *)eventList[i])->GetOwner();/*bulletCreated->GetOwner();*/

			//if (!players[owner]->getWeaponSound()->IsPlaying()) 
				//players[owner]->getWeaponSound()->Play();
			
			//bulletCreated->getPosition();

			break;
			}
		case Event::eBulletHit:
			//bulletHit = (Event::BulletHit *)eventList[i];

			if (((Event::BulletHit *)eventList[i])->getHitTargetID() == playerID)
			{
				RenderEngine::Damaged();
			}
			break;
		case Event::eShipSpawned:
			break;
		case Event::eShipDestroyed:
			{
				Event::ShipDestroyed* e = (Event::ShipDestroyed*)eventList[i];
				Oyster::Math::Float4x4 m = players[e->getDestroyedID()]->accessShip().getOrientation();

				players[e->getDestroyedID()]->accessShip().setOrientation(m);

				for(int i = 0; i < MAX_PLAYERS; i++)
				{
					this->scores[i] = e->GetAsStruct().scoreboard[i];
				}
			}
			break;
		case Event::eGameEnded:
			if (gameRunning)
			{
  				connection->disconnect();
				gameRunning=false;
				{
					EventStruct::GameEndedStruct str= ((Event::GameEnded*)eventList[i])->GetAsStruct();
					stateStack->PushState(new GameEndedMenu(this->players, this->scores, str.winningTeam ));
				}
			}
			break;
		case Event::eScoreUpdate:
			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				this->scores[i] = ((Event::ScoreUpdate*)eventList[i])->GetAsStruct().scoreboard[i];
			}
			break;
		default:
			break;
		}

		delete eventList[i];
	}
	if(gameRunning)
	{
		// Fixed Update period
		bool atLeastOneWhileLoop = false;
		this->elapsedUpdatePeriod += timeDelta;

		while( this->elapsedUpdatePeriod >= this->updatePeriod )
		{
			atLeastOneWhileLoop = true;

			this->elapsedUpdatePeriod -= this->updatePeriod;

			{ // data critical section
				this->lockMutex();
				for(int i=0;i< PLAYER_MAX_COUNT;++i)
				{
					Network::ServerToClientUpdateData data = connection->getUpdate(i);
					Player* p = players[i];
					p->accessShip().setOrientation( data.position );

					if( data.pid == playerID && p->HP > data.hp )
						RenderEngine::Damaged();

					p->HP = data.hp;
					p->Shield = data.shield;
					p->Speed = data.dirVecLen;
				}
				this->releaseMutex();
			}
		}

		this->lockMutex();
		RenderEngine::update( camera->getView(),camera->getProjection(), timeDelta, connection );
		this->releaseMutex();

		if( atLeastOneWhileLoop )
		{
			connection->sendData(update);

			SoundSystem::setTransformation(players[this->playerID]->getOrientation());
		}
	}

}

void IntoTheVoid::Render() const
{
	this->lockMutex();

	RenderEngine::RenderGame(this->players,this->MAX_PLAYERS, this->playerID, this->staticObject.model, this->staticObject.num );

	if (Oyster::Input::Controller::isKeyDown(VK_TAB))
		RenderEngine::RenderScoreboard(this->scores, this->MAX_PLAYERS, this->players);

	this->releaseMutex();
}

void IntoTheVoid::lockMutex( ) const
{
	while( this->readOnly );
	this->readOnly = true;
}

void IntoTheVoid::releaseMutex( ) const
{ this->readOnly = false; }

void IntoTheVoid::SetPlayerPosition(unsigned int id, const Oyster::Math::Float4x4 &position)
{
	this->lockMutex();
	players[id]->accessShip().setOrientation( position );
	this->releaseMutex();

	if (id != playerID)
		players[id]->getWeaponSound()->SetPosition(position.v[3].xyz);
}

void IntoTheVoid::SetPlayerUpdate( Network::ServerToClientUpdateData &data )
{
	this->lockMutex();
	Player* p = this->players[data.pid];
	if (data.pid==0)
	{
		int a=0;
	}
	else if(data.pid==1)
	{
		int a=0;
	}
	p->accessShip().setOrientation( data.position );

	if( data.pid == playerID && p->HP > data.hp )
		RenderEngine::Damaged();

	p->HP = data.hp;
	p->Shield = data.shield;
	p->Speed = data.dirVecLen;

	this->releaseMutex();

	if ( data.pid != playerID )
		p->getWeaponSound()->SetPosition( data.position.v[3].xyz );
}

void IntoTheVoid::SetRenderData( const Protocol::RenderData &renderData )
{
	Float scale = 0.1f;
	Float4x4 transform;
	rotationMatrix_AxisY( transform, ::Utility::Value::radian(180.0f) );
	transformMatrix( transform, scale * Float4x4::identity, transform );
	transform.m44 = 1.0f;

	// setup Players
	loadPlayerResources( this->players, connection->teamID, renderData, transform );

	// setup Static Objects, starting with clearing away any old stuff
	if( this->staticObject.model )
	{
		/*for( unsigned int i = 0; i < this->staticObject.num; ++i )
		{
			::std::vector<ID3D11ShaderResourceView*>::size_type j = 0;
			for( ; j < this->staticObject.model[i].Material.size(); ++i )
				this->staticObject.model[i].Material[j]->Release();
		}*/

		delete [] this->staticObject.model;
	}
	if( this->staticObject.world )
		delete [] this->staticObject.world;

	::std::vector<Protocol::Object>::size_type i = 0, num = renderData.mapData.getObjectList().size();
	if( num > 0 )
	{
		this->staticObject.num = (unsigned int) num;
		this->staticObject.model = new ::Oyster::Render::Model[num];
		this->staticObject.world = new Float4x4[num];

		for( ; i < num; ++i )
		{

			this->staticObject.model[i] = *Oyster::Resources::Manager::LoadModel(
				renderData.mapData.getMeshList()[ renderData.mapData.getObjectList()[i].getMeshRef() ].getObjFileName(),
							transform );

			//::Oyster::FileLoaders::ObjReader *reader = Oyster::FileLoaders::ObjReader::LoadFile(
			//				renderData.mapData.getMeshList()[ renderData.mapData.getObjectList()[i].getMeshRef() ].getObjFileName(),
			//				transform );
	
			//int vcount;
			//::Oyster::FileLoaders::ObjReader::Vertex** vertex = new Oyster::FileLoaders::ObjReader::Vertex*[1];
			//::std::map<std::string,ID3D11ShaderResourceView *> textures;
			//reader->GetVertexData( vertex, vcount, textures );

			//Oyster::Buffer::BUFFER_INIT_DESC desc;
			//{
			//	desc.ElementSize= sizeof(Oyster::FileLoaders::ObjReader::Vertex);
			//	desc.NumElements = vcount;
			//	desc.InitData = *vertex;
			//	desc.Type = Oyster::Buffer::VERTEX_BUFFER;
			//	desc.Usage = Oyster::Buffer::BUFFER_DEFAULT;
			//}
	
			//this->staticObject.model[i].Vertices = *(Oyster::Engine::Init::Buffers::CreateBuffer(desc));
			//this->staticObject.model[i].VertexCount = vcount;
			//this->staticObject.model[i].Material.push_back( textures["Diffuse"] );
			//this->staticObject.model[i].Material.push_back( textures["Specular"] );
			//this->staticObject.model[i].Material.push_back( textures["Glow"] );

			//this->staticObject.model[i].Visible = true;
			//this->staticObject.model[i].Indexed = false;

			this->staticObject.model[i].World = &this->staticObject.world[i];
			this->staticObject.world[i] = renderData.mapData.getObjectList()[i].getWorldMatrix();
		}
	}
	else
	{
		this->staticObject.num = 0;
		this->staticObject.model = NULL;
		this->staticObject.world = NULL;
	}
}

void IntoTheVoid::RecievedKeyFrame(const KeyFrame &keyframe)
{
	if (!this->Initialized)
		return;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		this->players[i]->HP = keyframe.Players[i].Hp;
	}
}

////////////////////////////////

void loadPlayerResources( Player **player, int shipID[], const ::Protocol::RenderData &renderData, const Float4x4 &transform )
{ // Loading player resources

	::Game::Ship *ship;
	player[0] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[0] ].getObjFileName(), transform );
	{
		ship = &player[0]->accessShip();
		ship->movementProperty.maxSpeed = 400.0f;
		ship->movementProperty.deAcceleration = 50.0f;
		ship->movementProperty.acceleration.forward = 50.0f;
		ship->movementProperty.acceleration.backward = 25.0f;
		ship->movementProperty.acceleration.horizontal = 25.0f;
		ship->movementProperty.acceleration.vertical = 25.0f;
		ship->rotationProperty.maxSpeed = ::Utility::Value::radian( 90.0f );
		ship->rotationProperty.deAcceleration = ::Utility::Value::radian( 180.0f );
		ship->rotationProperty.acceleration.pitch = ::Utility::Value::radian( 90.0f );
		ship->rotationProperty.acceleration.yaw = ::Utility::Value::radian( 90.0f );
		ship->rotationProperty.acceleration.roll = ::Utility::Value::radian( 90.0f );
	}
	player[1] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[1] ].getObjFileName(), transform );
	{
		ship = &player[1]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[2] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[2] ].getObjFileName(), transform );
	{
		ship = &player[2]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[3] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[3] ].getObjFileName(), transform );
	{
		ship = &player[3]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[4] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[4] ].getObjFileName(), transform );
	{
		ship = &player[4]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[5] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[5] ].getObjFileName(), transform );
	{
		ship = &player[5]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[6] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[6] ].getObjFileName(), transform );
	{
		ship = &player[6]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
	player[7] = new Player( renderData.playerData.getMeshList()[ renderData.playerData.getPlayerMeshRefList()[7] ].getObjFileName(), transform );
	{
		ship = &player[7]->accessShip();
		ship->movementProperty = player[0]->accessShip().movementProperty;
		ship->rotationProperty = player[0]->accessShip().rotationProperty;
	}
}
