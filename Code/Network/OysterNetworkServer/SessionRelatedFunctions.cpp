#include "SocketServer.h"

/*// BENCHMARK BLOCK
#include "WinTimer.h"
#include <iostream>
#include <fstream>
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
			for( unsigned char i = 0; i < 1; ++i )
				file << timerData[i].minTime << (timerData[i].minTime == 0.0f ? "\t\t: " : "\t: ") << timerData[i].maxTime << "\t: " << timerData[i].averageTime  << "\t\t" << timerData[i].totalTime << '\t' << timerData[i].numSamples <<'\n';
			file << ::std::endl;
			file.close();
			::std::cout << "Benchmark data saved." << ::std::endl;
		}
	}
}
// END BENCHMARK BLOCK/**/

void SocketServer::updateServers()
{
	for(int i=0; i<nrActiveSessions; i++)
	{
		if(games[i].allReady())
		{
			timer[i].tick();
			//printf("%f seconds since last update\n", timer[i].getDeltaTime());
			//printf("%f seconds since the timer started\n", timer[i].getGameTime());
			
			//timeTillUpdate[i]-=timer[i].getDeltaTime();

			DEBUGCTR++;
			//Sleep(timeTillUpdate[i]*1000);

			// BENCHMARK BLOCK
			//::Utility::WinTimer processTimer;
			// END BENCHMARK BLOCK

			switch( session->update( timer[i].getDeltaTime() ) )
			{
			case ::GameLogic::Session::Updated:
				// BENCHMARK BLOCK
				//processTimer.reset();
				// END BENCHMARK BLOCK
				
				processSessionPlayerData(i);
				processAllSessionEvents(i);
				processAllSessionEffects(i);

				// BENCHMARK BLOCK
				//Benchmark::sampleTime( processTimer, 0 );				
				// END BENCHMARK BLOCK

				DEBUGCTR=0;
				updateCount[i]++;
			default:
				break;
			case ::GameLogic::Session::Over:
				processAllSessionEvents(i);
				nrActiveSessions=0;
				if(users.size()==0)
				{
					printf("Game with id %d done, shutting down the game.\n", 0);
					Sleep(10);
					
				}
				break;
			}

			// BENCHMARK BLOCK
			//if( Benchmark::timerData[0].numSamples % 1000 == 1 )
			//	Benchmark::print();
			// END BENCHMARK BLOCK
		}
	}
	if(nrActiveSessions==0)
	{
		Sleep(50);
	}
}
void SocketServer::processSessionPlayerData(int serverID)
{
	sendGameDataStruct.updateCount=updateCount[serverID];
	int offset=1;
	for (int i=0; i<playersPerSessionCount/*games[serverID].getPlayerCount()*/; i++)
	{
		sendGameDataStruct.position=session->accessPlayer(i).getOrientation();
		sendGameDataStruct.hp=session->accessPlayer(i).getHullPoints();
		sendGameDataStruct.shield=session->accessPlayer(i).getShieldPoints();
		sendGameDataStruct.dirVecLen=session->accessPlayer(i).getMovement().length();
		sendGameDataStruct.pid=i;
		memcpy(sendGameDataBuffer+offset, &sendGameDataStruct, SERVER_PLAYER_DATA_SIZE);
		offset+=SERVER_PLAYER_DATA_SIZE;
	}
	sendData(-1,sendGameDataBuffer, sendGameDataBufferSize);
}
void SocketServer::processAllSessionEvents(int serverID)
{
	session->fetchEvents(sessionEvents);
	for (int i=0; i<(int)sessionEvents.size(); i++)
	{
		sendEventData(serverID, i);
		delete sessionEvents[i];
	}
	sessionEvents.resize(0);
}
bool SocketServer::sendGameData(int serverID)
{
	//data[0]=1;
	for (int i=0; i<games[serverID].getPlayerCount(); i++)
	{
		iResult = sendto(UDPSocket, sendGameDataBuffer, SERVER_PLAYER_DATA_SIZE+1, 0, (SOCKADDR *) & users[games[serverID].getUserID(i)].getAddr(), addrSize);
		if (iResult == SOCKET_ERROR) 
		{
			wprintf(L"UDP gameData sendto failed with error: %d\n", WSAGetLastError());
			closesocket(UDPSocket);
			WSACleanup();
			return false;
		}
	}
	return true;
}
void SocketServer::sendEventData(int gid, int sid)
{
	int size=sessionEvents[sid]->GetSize();
	int size1=sizeof(Event::BulletCreated);
	int tst=sizeof(Event::Type);
	char* ed=new char[size+1+tst];
	ed[0]=6;
	sessionEvents[sid]->SaveRawData(ed+(1+tst));

	Event::Type eTest=Event::getEventType(sessionEvents[sid]);
	memcpy(ed+1, &eTest, sizeof(Event::Type));

	sendData(-1, ed, size+1+tst);
	delete ed;
}
void SocketServer::sendRenderData(int gid)
{
	Protocol::RenderData data;
	session->writeToRenderResourceData(data);
	int size=data.getRequiredBufferSize()+1;
	char* sendChar=new char[size];
	data.fillBuffer(sendChar+1);
	sendChar[0]=8;
	sendData(-1, sendChar, size);
	delete sendChar;
}
void SocketServer::processAllSessionEffects(int gid)
{
	session->fetchEffectData(sessionEffects);

	if (sessionEffects.size()>0)
	{
		int size=(int)sessionEffects.size()*sizeof(Network::EffectData) + 1;
		delete sendEffectDataBuffer;
		sendEffectDataBuffer=new char[size];
		for (size_t i=0; i<sessionEffects.size(); i++)
		{
			memcpy(sendEffectDataBuffer+1+sizeof(Network::EffectData)*i, &sessionEffects[i], sizeof(Network::EffectData));
			//sessionEffects.
		}
		sendEffectDataBuffer[0]=7;
		sendData(-1, sendEffectDataBuffer, size);
	}

}
//HACK PLAYER UPDATE
void ControlPlayer( GameLogic::Player& p,const ClientToServerUpdateData &update)
{

	if(update.braking)
		p.enableMovementReduction( true );
	else
		p.disableMovementReduction();
	p.enableRotationReduction( true );

	if(update.forward>0)
		p.thrustForward();
	if(update.forward<0)
		p.thrustBackward();

	if(update.straferight>0)
		p.strafeRight();
	if(update.straferight<0)
		p.strafeLeft();

	if(update.strafeup>0)
		p.climb();
	if(update.strafeup<0)
		p.dive();

	if(update.roll>0)
	{
		::Oyster::Math::Float baseAcceleration = p.rotationProperty.acceleration.roll;
		p.rotationProperty.acceleration.roll /= ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
			
		p.rollLeft();
		p.rotationProperty.acceleration.roll = baseAcceleration;
	}
	if(update.roll<0)
	{
		::Oyster::Math::Float baseAcceleration = p.rotationProperty.acceleration.roll;
		p.rotationProperty.acceleration.roll /= ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		p.rollRight();
		p.rotationProperty.acceleration.roll = baseAcceleration;
	}
	if(update.roll==0)
	{
		p.stopRotation();
	}

	if(update.TurnVer!=0.0f)
	{
		::Oyster::Math::Float baseAcceleration = p.rotationProperty.acceleration.pitch;
		p.rotationProperty.acceleration.pitch *= -update.TurnVer / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		p.pitchUp( );
		p.disableRotationReduction();
		p.rotationProperty.acceleration.pitch = baseAcceleration;
	}

	if(update.TurnHor!=0.0f)
	{
		::Oyster::Math::Float baseAcceleration = p.rotationProperty.acceleration.yaw;
		p.rotationProperty.acceleration.yaw *= -update.TurnHor / ::Oyster::Game::MoveAble::getDiscreteTimeSlice();
		p.yawLeft( );
		p.disableRotationReduction();
		p.rotationProperty.acceleration.yaw = baseAcceleration;
	}
	if(update.firePrim)
		p.firePrimaryWeapon();
}

