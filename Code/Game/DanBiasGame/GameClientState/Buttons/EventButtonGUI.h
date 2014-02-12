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
				Oyster::Graphics::API::DeleteTexture(texture2);
				Oyster::Graphics::API::DeleteTexture(texture3);
				texture = NULL;
				texture2 = NULL;
				texture3 = NULL;
			}

			void CreateTexture(std::wstring textureName)
			{
				std::wstring file = L".png";

				//Create texture
				texture = Oyster::Graphics::API::CreateTexture(textureName + std::wstring(L"none") + file);
				texture2 = Oyster::Graphics::API::CreateTexture(textureName + std::wstring(L"highlight") + file);
				texture3 = Oyster::Graphics::API::CreateTexture(textureName + std::wstring(L"down") + file);
			}

			virtual void Render()
			{

				if(EventButton<Owner>::Enabled())
				{
					//Render att xPos and yPos
					//With width and height

					if(EventButton<Owner>::GetState() == ButtonState_None)
					{
						Oyster::Graphics::API::RenderGuiElement(texture, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height));
					}
					else if(EventButton<Owner>::GetState() == ButtonState_Hover)
					{
						Oyster::Graphics::API::RenderGuiElement(texture2, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height));
					}
					else
					{
						Oyster::Graphics::API::RenderGuiElement(texture3, Oyster::Math::Float2(xPos, yPos), Oyster::Math::Float2(width, height));
					}

				}
			}

		protected:
			float xPos, yPos;
			float width, height;
			Oyster::Graphics::API::Texture texture;
			Oyster::Graphics::API::Texture texture2;
			Oyster::Graphics::API::Texture texture3;


		};
	}
}

#endif