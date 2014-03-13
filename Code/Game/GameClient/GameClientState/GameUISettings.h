#ifndef GAME_UI_SETTINGS
#define GAME_UI_SETTINGS

#include <fstream>

namespace Settings
{
	class StatsUISettings
	{
	public: 
		struct Settings
		{
			float fontSize;
			float lineSpacing;
			float textOffsetY;
			float globalZ;

			float nameMarigin;
			float killsMargin;
			float deathMargin;

			float nameTextSize[2];
			float killTextSize[2];
			float deathTextSize[2];

			float nameMarginTextSize[2];
			float killMarginTextSize[2];
			float deathMarginTextSize[2];

			float killMarginColor[4];
			float deathMarginColor[4];

			Settings()
			{
				fontSize = 0.0f;
				lineSpacing = 0.0f;
				textOffsetY = 0.0f;
				globalZ = 0.0f;
				nameMarigin = 0.0f;
				killsMargin = 0.0f;
				deathMargin = 0.0f;
				memset(&nameTextSize, 0, sizeof(float) * 2);
				memset(&killTextSize, 0, sizeof(float) * 2);
				memset(&deathTextSize, 0, sizeof(float) * 2);
				memset(&nameMarginTextSize, 0, sizeof(float) * 2);
				memset(&killMarginTextSize, 0, sizeof(float) * 2);
				memset(&deathMarginTextSize, 0, sizeof(float) * 2);
			}
		};
		static void ParseStatsSettings(Settings& data)
		{
			std::ifstream in("..\\Settings\\statsLayout.settings");
			if(in.is_open())
			{
				while (!in.eof())
				{
					std::string buff;
					in >> buff;

					if( buff == "fontSize")
					{
						in >> data.fontSize;
					}
					else if( buff == "lineSpacing")
					{
						in >> data.lineSpacing;
					}
					else if( buff == "textOffsetY")
					{
						in >> data.textOffsetY;
					}
					else if( buff == "globalZ")
					{
						in >> data.globalZ;
					}
					else if( buff == "nameMarigin")
					{
						in >> data.nameMarigin;
					}
					else if( buff == "killsMargin")
					{
						in >> data.killsMargin;
					}
					else if( buff == "deathMargin")
					{
						in >> data.deathMargin;
					}
					else if( buff == "nameTextSize")
					{
						in >> data.nameTextSize[0]; in >> data.nameTextSize[1];
					}
					else if( buff == "killTextSize")
					{
						in >> data.killTextSize[0]; in >> data.killTextSize[1];
					}
					else if( buff == "deathTextSize")
					{
						in >> data.deathTextSize[0]; in >> data.deathTextSize[1];
					}
					else if( buff == "nameMarginTextSize")
					{
						in >> data.nameMarginTextSize[0]; in >> data.nameMarginTextSize[1];
					}
					else if( buff == "killMarginTextSize")
					{
						in >> data.killMarginTextSize[0]; in >> data.killMarginTextSize[1];
					}
					else if( buff == "deathMarginTextSize")
					{
						in >> data.deathMarginTextSize[0]; in >> data.deathMarginTextSize[1];
					}
					else if( buff == "killMarginColor")
					{
						in >> data.killMarginColor[0];
						in >> data.killMarginColor[1];
						in >> data.killMarginColor[2];
						in >> data.killMarginColor[3];
					}
					else if( buff == "deathMarginColor")
					{
						in >> data.deathMarginColor[0];
						in >> data.deathMarginColor[1];
						in >> data.deathMarginColor[2];
						in >> data.deathMarginColor[3];
					}
				}
				in.close();
			}
		}
	};

	class GameUISettings
	{
	public: 
		struct Settings
		{
			float timersize;
			float timerpos[3];
			float timercolor[4];
			float timerboxsize[2];

			float healthsize;
			float healthpos[3];
			float healthcolor[4];
			float healthboxsize[2];

			float energysize;
			float energypos[3];
			float energycolor[4];
			float energyboxsize[2];

			Settings()
			{
				this->timersize = 0.0f;
				memset(&this->timerpos[0], 0, sizeof(float) * 3);
				memset(&this->timercolor[0], 0, sizeof(float) * 4);
				memset(&this->timerboxsize[0], 0, sizeof(float) * 2);
				this->healthsize = 0.0f;
				memset(&this->healthpos[0], 0, sizeof(float) * 3);
				memset(&this->healthcolor[0], 0, sizeof(float) * 4);
				memset(&this->healthboxsize[0], 0, sizeof(float) * 2);
				this->energysize = 0.0f;
				memset(&this->energypos[0], 0, sizeof(float) * 3);
				memset(&this->energycolor[0], 0, sizeof(float) * 4);
				memset(&this->energyboxsize[0], 0, sizeof(float) * 2);
			}
		};
		static void ParseSettings(Settings& data)
		{
			std::ifstream in("..\\Settings\\gameUI.settings");
			if(in.is_open())
			{
				while (!in.eof())
				{
					std::string buff;
					in >> buff;

					if(buff == "timersize")
					{
						in >> data.timersize;
					}
					else if(buff == "timerpos")
					{
						in >> data.timerpos[0];
						in >> data.timerpos[1];
						in >> data.timerpos[2];
					}
					else if(buff == "timercolor")
					{
						in >> data.timercolor[0];
						in >> data.timercolor[1];
						in >> data.timercolor[2];
						in >> data.timercolor[3];
					}
					else if(buff == "timerboxsize")
					{
						in >> data.timerboxsize[0];
						in >> data.timerboxsize[1];
					}
					else if(buff == "healthsize")
					{
						in >> data.healthsize;
					}
					else if(buff == "healthpos")
					{
						in >> data.healthpos[0];
						in >> data.healthpos[1];
						in >> data.healthpos[2];
					}
					else if(buff == "healthcolor")
					{
						in >> data.healthcolor[0];
						in >> data.healthcolor[1];
						in >> data.healthcolor[2];
						in >> data.healthcolor[3];
					}
					else if(buff == "healthboxsize")
					{
						in >> data.healthboxsize[0];
						in >> data.healthboxsize[1];
					}
					else if(buff == "energysize")
					{
						in >> data.energysize;
					}
					else if(buff == "energypos")
					{
						in >> data.energypos[0];
						in >> data.energypos[1];
						in >> data.energypos[2];
					}
					else if(buff == "energycolor")
					{
						in >> data.energycolor[0];
						in >> data.energycolor[1];
						in >> data.energycolor[2];
						in >> data.energycolor[3];
					}
					else if(buff == "energyboxsize")
					{
						in >> data.energyboxsize[0];
						in >> data.energyboxsize[1];
					}

				}
				in.close();
			}
		}
	};
}


#endif // !GAME_UI_SETTINGS

