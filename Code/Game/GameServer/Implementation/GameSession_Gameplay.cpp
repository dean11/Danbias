/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
// Edited by Linda Andersson
/////////////////////////////////////////////////////////////////////
#include "..\GameSession.h"
#include "..\GameClient.h"
#include <WinTimer.h>

#include <Protocols.h>
#include <PostBox\PostBox.h>
#include <GameLogicStates.h>
#include <OysterMath.h>

#define NOMINMAX
#include <Windows.h>



using namespace Utility::DynamicMemory;
using namespace Oyster;
using namespace Oyster::Network;
using namespace Oyster::Thread;
using namespace GameLogic;
using namespace DanBias;

	
	bool GameSession::DoWork(  )
	{
		if(this->isRunning)
		{
			// GameLogic takes care of propper step time
			float ldt = (float)this->logicTimer.getElapsedSeconds();
			//ldt = Utility::Value::Max(ldt, 0.000f);
			this->logicTimer.reset();
			this->gameInstance.NewFrame(ldt);
			
			// Need to accumulate network time
			this->accumulatedNetworkTime += (float)this->networkTimer.getElapsedSeconds();
			//while (this->accumulatedNetworkTime >= this->networkFrameTime)
			if (this->networkTimer.getElapsedSeconds() >= this->networkFrameTime)
			{
				this->ProcessClients();
				this->accumulatedNetworkTime -= this->networkFrameTime;
				this->networkTimer.reset();
			}
			//this->networkTimer.reset();
		}

		return this->isRunning;
	}

	void GameSession::ClientEventCallback(NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e)
	{
		int temp = -1;
		//Find the idiot
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i] && this->gClients[i]->Equals(e.sender))
			{
				temp = i;
			}
		}

		if(temp == -1)		
		{
			this->Detach(e.sender)->Disconnect();
			return;
		}
		SmartPointer<GameClient> cl = this->gClients[temp];

		switch (e.args.type)
		{
			case NetworkClient::ClientEventArgs::EventType_Disconnect:
				{
					//Send disconnect message to all the other players so the player can be removed from the client.
					Protocol_ObjectDisconnectPlayer dp(cl->GetClient()->GetID());
					for(int i = 0; i < this->gClients.Size(); i++)
					{
						if(this->gClients[i] && this->gClients[i] != cl)
						{
							this->gClients[i]->GetClient()->Send(dp);
						}
					}
					printf("\t(%i : %s) - EventType_Disconnect\n", cl->GetClient()->GetID(), e.sender->GetIpAddress().c_str());	
					this->gameInstance.RemovePlayer(cl->GetPlayer());
					cl->Invalidate();
					
					clientCount--;
				}
				break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToRecieve:
				break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolFailedToSend:
				if(this->gClients[temp]->IncrementFailedProtocol() >= 5/*client->threshold*/)
				{
					//Send disconnect message to all the other players so the player can be removed from the client.
					Protocol_ObjectDisconnectPlayer dp(cl->GetClient()->GetID());
					for(int i = 0; i < this->gClients.Size(); i++)
					{
						if(this->gClients[i] && this->gClients[i] != cl)
						{
							this->gClients[i]->GetClient()->Send(dp);
						}
					}
					printf("\t(%i : %s) - EventType_ProtocolFailedToSend -> disconnecting\n", cl->GetClient()->GetID(), e.sender->GetIpAddress().c_str());	
					this->gameInstance.RemovePlayer(this->gClients[temp]->GetPlayer());
					this->gClients[temp]->Invalidate();
					clientCount--;
				}
				break;
			case NetworkClient::ClientEventArgs::EventType_ProtocolRecieved:
				this->ParseProtocol(e.args.data.protocol, cl);
				break;
		}
	}
	void GameSession::ProcessClients()
	{
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i] && !this->gClients[i]->IsInvalid())
			{
				this->gClients[i]->UpdateClient();
				if(this->gClients[i]->IsInvalid())
					this->gClients[i] = 0;
			}
		}
	}
	bool GameSession::Send(Oyster::Network::CustomNetProtocol& message)
	{
		bool returnValue = false;
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i] && !this->gClients[i]->IsReady())
			{
				this->gClients[i]->GetClient()->Send(message);
				returnValue = true;
			}
		}

		return returnValue;

	}
	bool GameSession::Send(Oyster::Network::CustomNetProtocol& protocol, int ID)
	{
		for (unsigned int i = 0; i < this->gClients.Size(); i++)
		{
			if(this->gClients[i] && !this->gClients[i]->IsInvalid() && this->gClients[i]->GetClient()->GetID() == ID)
			{
				this->gClients[i]->GetClient()->Send(protocol);
				return true;
			}
		}
		return false;
	}

	void GameSession::ObjectMove(GameLogic::IObjectData* movedObject)
	{
		//float dt = (float)GameSession::gameSession->networkTimer.getElapsedSeconds();
		//Duh... This was causing alot of problems, it's in the wrong place...
		//Need to figure out where to put this frame locker.
		//We only need to send network packages when necessary, ie not 120 times per frame. 
		//I think it would be enough with 60-70 packages per second due to the nature of
		//graphics update (60 fps) on the client side. To send more than this would be lost
		//bandwidth.
		//if( dt >= GameSession::gameSession->networkFrameTime )
		//{
		//	GameSession::gameSession->networkTimer.reset();
			int id = movedObject->GetID();
			//Protocol_ObjectPosition p(movedObject->GetPosition(), id);
			Protocol_ObjectPositionRotation p(movedObject->GetPosition(), movedObject->GetRotation(), id);

			Oyster::Math::Float3 temp = movedObject->GetPosition();

			GameSession::gameSession->Send(p.GetProtocol());
		//}	
	}
	void GameSession::ObjectDisabled( GameLogic::IObjectData* movedObject )
	{
		GameSession::gameSession->Send(Protocol_ObjectDisable(movedObject->GetID()).GetProtocol());
	}
	void GameSession::ObjectEnabled( GameLogic::IObjectData* movedObject )
	{
		GameSession::gameSession->Send(Protocol_ObjectEnable(movedObject->GetID()).GetProtocol());
	}
	void GameSession::ObjectDamaged( GameLogic::IObjectData* movedObject, float hp, int eventID )
	{
		GameSession::gameSession->Send(Protocol_ObjectDamage(movedObject->GetID(), hp, eventID).GetProtocol());
	}
	void GameSession::ObjectRespawned( GameLogic::IObjectData* movedObject, Oyster::Math::Float3 spawnPos )
	{
		GameSession::gameSession->Send(Protocol_ObjectRespawn(movedObject->GetID(), spawnPos).GetProtocol());
	}
	void GameSession::ObjectDead( IObjectData* victim, int deatchCount, IObjectData* killer, int killCount, float seconds )
	{
		GameSession::gameSession->Send(Protocol_ObjectDie(victim->GetID(), deatchCount, killer->GetID(), killCount, seconds).GetProtocol());
	}
	void GameSession::PickupEvent( GameLogic::IObjectData* movedObject, int pickupEffectID )
	{
		// send pickup protocol
		GameSession::gameSession->Send(Protocol_ObjectPickup(movedObject->GetID(), pickupEffectID).GetProtocol());
	}
	void GameSession::ActionEvent( GameLogic::IObjectData* movedObject , int actionID )
	{
		// send action protocol
		GameSession::gameSession->Send(Protocol_ObjectAction(movedObject->GetID(), actionID).GetProtocol());
	}
	void GameSession::CollisionEvent( GameLogic::IObjectData* movedObject , int collisionID )
	{
		// send action protocol
		GameSession::gameSession->Send(Protocol_ObjectCollision(movedObject->GetID(), collisionID).GetProtocol());
	}
	void GameSession::EnergyUpdate( GameLogic::IObjectData* movedObject , float energy )
	{
		// send action protocol
		GameSession::gameSession->Send(Protocol_ObjectWeaponEnergy(movedObject->GetID(), energy).GetProtocol());
	}
	void GameSession::GameOver(  )
	{
		GameSession::gameSession->reset = true; //Reset due to normal game conditions 
		GameSession::gameSession->Send( Protocol_General_GameOver(8.0f).GetProtocol() );
		GameSession::gameSession->isRunning = false;
	}
	void GameSession::BeamEffect( GameLogic::IObjectData* creator, const ::Oyster::Math::Float3 &start, const ::Oyster::Math::Float3 &end, ::Oyster::Math::Float radius, ::Oyster::Math::Float lifeTime )
	{
		GameSession::gameSession->Send
		( Protocol_EffectBeam
			(
				creator != nullptr ? creator->GetID() : -1,
				start,
				end,
				radius,
				lifeTime
			).GetProtocol()
		);
	}
	void GameSession::OnGameTimeTick(float gameTime)
	{
		GameSession::gameSession->timeLeft = gameTime;
		GameSession::gameSession->Send( Protocol_General_Timer(gameTime).GetProtocol() );
	}



//*****************************************************//
//****************** Protocol methods *****************//
//******************************************************************************************************************//
	void GameSession::ParseProtocol(Oyster::Network::CustomNetProtocol& p, DanBias::GameClient* c)
	{
		//TODO: Update response timer

		switch (p[0].value.netShort)
		{
			case protocol_Gameplay_PlayerMovementBackward:		this->Gameplay_PlayerMovementBack		( c );
			break;
			case protocol_Gameplay_PlayerMovementForward:		this->Gameplay_PlayerMovementForth		( c );
			break;
			case protocol_Gameplay_PlayerMovementLeft:			this->Gameplay_PlayerMovementLeft		( c );
			break;
			case protocol_Gameplay_PlayerMovementRight:			this->Gameplay_PlayerMovementRight		( c );
			break;
			case protocol_Gameplay_PlayerJump:					this->Gameplay_PlayerJump				( c );
			break;
			case protocol_Gameplay_PlayerLeftTurn:				this->Gameplay_PlayerLeftTurn			( Protocol_PlayerLeftTurn		(p), c );
			break;
			case protocol_Gameplay_PlayerChangeWeapon:			this->Gameplay_PlayerChangeWeapon		( Protocol_PlayerChangeWeapon	(p), c );
			break;
			case protocol_Gameplay_PlayerShot:					this->Gameplay_PlayerShot				( Protocol_PlayerShot			(p), c );
			break;
			case protocol_Gameplay_ObjectPickup:				this->Gameplay_ObjectPickup				( Protocol_ObjectPickup			(p), c );
			break;
			case protocol_Gameplay_ObjectDamage:				this->Gameplay_ObjectDamage				( Protocol_ObjectDamage			(p), c );
			break;
			case protocol_Gameplay_ObjectPosition:				this->Gameplay_ObjectPosition			( Protocol_ObjectPosition		(p), c );
			break;
			case protocol_Gameplay_ObjectEnabled:				this->Gameplay_ObjectEnabled			( Protocol_ObjectEnable			(p), c );
			break;
			case protocol_Gameplay_ObjectDisabled:				this->Gameplay_ObjectDisabled			( Protocol_ObjectDisable		(p), c );
			break;
			case protocol_Gameplay_ObjectCreate:				this->Gameplay_ObjectCreate				( Protocol_ObjectCreate			(p), c );
			break;
			case protocol_General_Status:						this->General_Status					( Protocol_General_Status		(p), c );
			break;
			case protocol_General_Text:							this->General_Text						( Protocol_General_Text			(p), c );
			break;
			case protocol_Lobby_QuerryGameType:					this->General_QuerryGameData				( c );
				break;
		}
	}

	void GameSession::Gameplay_PlayerMovementBack		( DanBias::GameClient* c )
	{
		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_BACKWARD);
	}
	void GameSession::Gameplay_PlayerMovementForth		( DanBias::GameClient* c )
	{
		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_FORWARD);
	}
	void GameSession::Gameplay_PlayerMovementLeft		( DanBias::GameClient* c )
	{
		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_LEFT);
	}
	void GameSession::Gameplay_PlayerMovementRight		( DanBias::GameClient* c )
	{
		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_RIGHT);
	}
	void GameSession::Gameplay_PlayerJump				( DanBias::GameClient* c )
	{
		c->GetPlayer()->Move(GameLogic::PLAYER_MOVEMENT_JUMP);
	}
	void GameSession::Gameplay_PlayerLeftTurn			( Protocol_PlayerLeftTurn& p, DanBias::GameClient* c )
	{
		c->GetPlayer()->TurnLeft( p.deltaRadian );
		c->GetPlayer()->SetLookDir( p.lookdir ) ;
	}
	void GameSession::Gameplay_PlayerChangeWeapon		( Protocol_PlayerChangeWeapon& p, DanBias::GameClient* c )
	{
		c->GetPlayer()->SelectWeaponAttatchment(p.weaponId);
	}
	void GameSession::Gameplay_PlayerShot				( Protocol_PlayerShot& p, DanBias::GameClient* c )
	{ 
		switch (p.value)
		{
			case Protocol_PlayerShot::ShootValue_PrimaryPress:			c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_PRESS);
			break;
			case Protocol_PlayerShot::ShootValue_PrimaryRelease:		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_PRIMARY_RELEASE);
			break;
			case Protocol_PlayerShot::ShootValue_SecondaryPress:		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_SECONDARY_PRESS);
			break;
			case Protocol_PlayerShot::ShootValue_SecondaryRelease:		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_SECONDARY_RELEASE);
			break;
			case Protocol_PlayerShot::ShootValue_UtilityPress:			c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_UTILLITY_PRESS);
			break;
			case Protocol_PlayerShot::ShootValue_UtilityRelease:		c->GetPlayer()->UseWeapon(GameLogic::WEAPON_USE_UTILLITY_RELEASE);
			break;
			case Protocol_PlayerShot::ShootValue_DropItem:				c->GetPlayer()->UseWeapon(GameLogic::WEAPON_INTERRUPT);
			break;
		}
	}
	void GameSession::Gameplay_ObjectPickup				( Protocol_ObjectPickup& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectDamage				( Protocol_ObjectDamage& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectPosition			( Protocol_ObjectPosition& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectEnabled			( Protocol_ObjectEnable& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectDisabled			( Protocol_ObjectDisable& p, DanBias::GameClient* c )
	{

	}
	void GameSession::Gameplay_ObjectCreate				( Protocol_ObjectCreate& p, DanBias::GameClient* c )
	{

	}
	void GameSession::General_Status					( Protocol_General_Status& p, DanBias::GameClient* c )
	{
		switch (p.status)
		{
			case GameLogic::Protocol_General_Status::States_disconected:
				printf("Client with ID [%i] dissconnected\n", c->GetClient()->GetID());
				//TODO: Tell other clients
				//Protocol_
				this->Detach(c->GetClient()->GetID());
			break;

			case GameLogic::Protocol_General_Status::States_idle:

			break;

			case GameLogic::Protocol_General_Status::States_ready:
				c->SetReadyState(true);
			break;

			case GameLogic::Protocol_General_Status::States_leave:

			break;
		}
	}
	void GameSession::General_Text						( Protocol_General_Text& p, DanBias::GameClient* c )
	{
		printf("Message recieved from (%i):\t %s\n", c->GetClient()->GetID(), p.text.c_str());
	}
	void GameSession::General_QuerryGameData			( DanBias::GameClient* c )
	{
		if(this->reset && !c->IsReady())
		{
			c->SetReadyState(true);
		}
	}







