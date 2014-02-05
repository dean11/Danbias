/////////////////////////////////////////////////////////////////////
// Created by [Erik Persson] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef GAME_API_H
#define GAME_API_H

//Includes windows so we need to undef minmax
#define NOMINMAX
#include <vld.h>

#include "GameLogicDef.h"
#include "GameLogicStates.h"
#include <OysterMath.h>


namespace GameLogic
{
	class IObjectData;
	
	/**
	*	Collection of various event structures that may be used to get events grom the game.
	*/
	namespace GameEvent
	{
		/**
		*	The type of event to listen on.
		*/
		enum ObjectEventFunctionType
		{
			ObjectEventFunctionType_OnMove,
			ObjectEventFunctionType_OnDead,
		};

		typedef void(*ObjectEventFunction)(IObjectData* object);	// Callback method that recieves and object
		//etc...
	};

		/**
		*	An object that collects the standard usage functions
		*/
		class IObjectData
		{
		public:
			/********************************************************
			* Gets players position
			* @param playerID: ID of the player whos position you want
			********************************************************/
			virtual Oyster::Math::Float3 GetPosition() = 0;

			/********************************************************
			* Gets players current orientation
			* @param playerID: ID of the player whos position you want
			********************************************************/
			virtual Oyster::Math::Float4x4 GetOrientation() = 0;

			/** Get the uniuqe id for the object
			*	@return The id for the object.
			*/
			virtual int GetID() const = 0;

			/** Get the type of the object
			*	@return The OBJECT_TYPE of the object is returned
			*/
			virtual OBJECT_TYPE GetObjectType() const = 0;
		};

		class IPlayerData :public IObjectData
		{
		public:
			/********************************************************
			* Moves the chosen player based on input
			* @param playerID: ID of the player you want to recieve the message
			* @param movement: enum value on what kind of action is to be taken
			********************************************************/
			virtual void Move(const PLAYER_MOVEMENT &movement) = 0;

			/** Relative rotation around given axis
			*	@param x: The relative x axis	
			*	@param y: The relative y axis	
			**/
			virtual void Rotate(const Oyster::Math3D::Float4 lookDir) = 0;

			/********************************************************
			* Uses the chosen players weapon based on input
			* @param Usage: enum value on what kind of action is to be taken
			********************************************************/
			virtual void UseWeapon(const WEAPON_FIRE &usage) = 0;

			/***/
			virtual int GetTeamID() const = 0;

			/********************************************************
			* Check player state
			* @return The current player state
			********************************************************/
			virtual PLAYER_STATE GetState() const = 0;
		};

		class ILevelData :public IObjectData
		{
		public:
			virtual int getNrOfDynamicObj()const						= 0;
			virtual IObjectData* GetObjectAt(int ID) const				= 0;
		};

	class DANBIAS_GAMELOGIC_DLL GameAPI
	{
	public:
		/**	A global function to get an instance of the API
		*	@return Returns the GameAPI instance
		*/
		static GameAPI& Instance( void );

	public:
		/**	Initiates the Game instance.
		*	- This method must be called before the API can be used.
		*	@return	If the return value is false, something went wrong.
		*/
		virtual bool Initiate( void ) = 0;

		/********************************************************
		* Gets the position of all players currently in the game
		********************************************************/
		virtual void GetAllPlayerPositions( void ) const = 0;

		/**	Creates a player
		*	@return Returns a IPlayerData container to use for player manipulation.
		*/
		virtual IPlayerData* CreatePlayer( void ) = 0;

		/**	Creates a level 
		*	@return Returns a ILevelData container to use for level manipulation
		*/
		virtual ILevelData* CreateLevel( void ) = 0;

		/** Creates a team
		*	@return ?
		*/
		virtual void CreateTeam( void ) = 0;
		
		/**	Runs a update of the gamelogic and physics
		*	@return Returns true if a frame was proccessed
		*/
		virtual bool NewFrame( void ) = 0;

		/**	Set the frame time in fps
		*	@param FPS The fps to set
		*/
		virtual void SetFPS( int FPS = 60 ) = 0;

		/**	Set the frames time in seconds
		*	@param seconds The frame length
		*/
		virtual void SetFrameTimeLength( float seconds ) = 0;

		/**	Set a specific object event subscription callback
		*	@param 
		*/
		virtual void SetSubscription(GameEvent::ObjectEventFunctionType type, GameEvent::ObjectEventFunction functionPointer) = 0;

	};	
}


#endif
