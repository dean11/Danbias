//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef MISC_IEVENT_BUTTON
#define MISC_IEVENT_BUTTON

class InputClass;

namespace Oyster
{
	namespace Event
	{
		enum ButtonState
		{
			ButtonState_None,
			ButtonState_Hover,
			ButtonState_Pressed,
			ButtonState_Down,
			ButtonState_Released,
		};
		
		//Takes normalized device coordinates
		struct MouseInput
		{
			//Normalized device coordinates
			float x, y;
			bool mouseButtonPressed;
		};

		class IEventButton
		{
		public:
			virtual ~IEventButton(){}

			virtual void RenderTexture() = 0;
			virtual void RenderText() = 0;

			virtual void Update(MouseInput& input) = 0;

			virtual void SendEvent(ButtonState state) = 0;
			
			virtual unsigned int GetID() = 0;

			virtual void SetEnabled(bool enable) = 0;
			virtual bool Enabled() = 0;
		
		};
	}
}

#endif