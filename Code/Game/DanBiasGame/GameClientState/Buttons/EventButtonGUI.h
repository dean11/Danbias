//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_EVENT_BUTTON_GUI_H
#define DANBIAS_CLIENT_EVENT_BUTTON_GUI_H

#include "../Misc/EventHandler/EventButton.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class EventButtonGUI : public Oyster::Event::EventButton<Owner>
		{
		public:
			EventButtonGUI() 
				: EventButton(), xPos(0), yPos(0), halfWidth(0), halfHeight(0), texture(NULL)
			{}
			EventButtonGUI(std::wstring textureName, Owner owner, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(owner), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(func), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(func, owner), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float halfWidth, float halfHeight) 
				: EventButton(func, owner, userData), xPos(xPos), yPos(yPos), halfWidth(halfWidth), halfHeight(halfHeight)
			{
				CreateTexture(textureName);
			}
			~EventButtonGUI()
			{}

			void CreateTexture(std::wstring textureName)
			{
				//Create texture
			}

			virtual void Render()
			{
				if(EventButton<Owner>::Enabled())
				{
					//Render att xPos and yPos
					//With halfWidth and halfHeight
				}
			}

		protected:
			float xPos, yPos;
			float halfWidth, halfHeight;
			void* texture;

		};
	}
}

#endif