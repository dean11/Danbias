//////////////////////////////////////
// Created by Pontus Fransson 2014  //
//////////////////////////////////////

#ifndef DANBIAS_CLIENT_EVENT_BUTTON_GUI_H
#define DANBIAS_CLIENT_EVENT_BUTTON_GUI_H

#include "../Misc/EventHandler/EventButton.h"
#include "../OysterGraphics/DllInterfaces/GFXAPI.h"

namespace DanBias
{
	namespace Client
	{
		template <typename Owner>
		class EventButtonGUI : public Oyster::Event::EventButton<Owner>
		{
		public:
			EventButtonGUI() 
				: EventButton(), xPos(0), yPos(0), width(0), height(0), texture(NULL)
			{}
			EventButtonGUI(std::wstring textureName, Owner owner, float xPos, float yPos, float width, float height) 
				: EventButton(owner), xPos(xPos), yPos(yPos), width(width), height(height), texture(NULL)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, float xPos, float yPos, float width, float height) 
				: EventButton(func), xPos(xPos), yPos(yPos), width(width), height(height), texture(NULL)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, Owner owner, float xPos, float yPos, float width, float height) 
				: EventButton(func, owner), xPos(xPos), yPos(yPos), width(width), height(height), texture(NULL)
			{
				CreateTexture(textureName);
			}
			EventButtonGUI(std::wstring textureName, EventFunc func, Owner owner, void* userData, float xPos, float yPos, float width, float height) 
				: EventButton(func, owner, userData), xPos(xPos), yPos(yPos), width(width), height(height), texture(NULL)
			{
				CreateTexture(textureName);
			}
			virtual ~EventButtonGUI()
			{
				Oyster::Graphics::API::DeleteTexture(texture);
				texture = NULL;
			}

			void CreateTexture(std::wstring textureName)
			{
				//Create texture
				texture = Oyster::Graphics::API::CreateTexture(textureName);
			}

			virtual void Render()
			{
				if(EventButton<Owner>::Enabled())
				{
					//Render att xPos and yPos
					//With width and height

					if(EventButton<Owner>::GetState() == ButtonState_None)
					{
						Oyster::Graphics::API::RenderGuiElement(texture, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height), Oyster::Math::Float3(1, 1, 1));
					}
					else if(EventButton<Owner>::GetState() == ButtonState_Hover)
					{
						Oyster::Graphics::API::RenderGuiElement(texture, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height), Oyster::Math::Float3(0, 1, 0));
					}
					else
					{
						Oyster::Graphics::API::RenderGuiElement(texture, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height), Oyster::Math::Float3(1, 0, 0));
					}

				}
			}

		protected:
			float xPos, yPos;
			float width, height;
			Oyster::Graphics::API::Texture texture;


		};
	}
}

#endif