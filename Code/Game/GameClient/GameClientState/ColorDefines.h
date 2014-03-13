#ifndef COLORDEFINES_H
#define COLORDEFINES_H
namespace DanBias
{
	namespace Client
	{
		class ColorDefines
		{
		public:
			ColorDefines();
			~ColorDefines();
			Oyster::Math::Float3 getGlowColor(int index);
			Oyster::Math::Float3 getTintColor(int index);
			std::wstring getColorName(int index);
			std::wstring getPlayerName(int index);

		private:
			Oyster::Math::Float3	tintColor[10];
			Oyster::Math::Float3	glowTintColor[10];
			std::wstring			colorNames[10];
			std::wstring			playerNames[10];
		};

		ColorDefines::ColorDefines()
		{
			// TODO make nice colors 
			tintColor[0] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[1] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[2] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[3] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[4] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[5] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[6] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[7] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[8] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 
			tintColor[9] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); 

			glowTintColor[0] = Oyster::Math::Float3( 0.78f, 0.0f, 1.0f ); // purple
			glowTintColor[1] = Oyster::Math::Float3( 0.0f, 0.9f, 0.0f ); // green
			glowTintColor[2] = Oyster::Math::Float3( 0.0f, 0.0f, 0.9f ); // blue 
			glowTintColor[3] = Oyster::Math::Float3( 1.0f, 0.0f, 0.5f ); // pink
			glowTintColor[4] = Oyster::Math::Float3( 1.0f, 1.0f, 0.0f ); // yellow
			glowTintColor[5] = Oyster::Math::Float3( 1.0f, 0.4f, 0.0f ); // orange
			glowTintColor[6] = Oyster::Math::Float3( 0.0f, 1.0f, 1.0f ); // cyan
			glowTintColor[7] = Oyster::Math::Float3( 0.9f, 0.0f, 0.0f ); // red
			glowTintColor[8] = Oyster::Math::Float3( 1.0f, 1.0f, 1.0f ); // white
			glowTintColor[9] = Oyster::Math::Float3( 0.5f, 1.0f, 0.0f ); // lime

			colorNames[0] = L"Mr.Purple";
			colorNames[1] = L"Mr.Green";
			colorNames[2] = L"Mr.Blue";
			colorNames[3] = L"Mr.Pink";
			colorNames[4] = L"Mr.Yellow";
			colorNames[5] = L"Mr.Orange";
			colorNames[6] = L"Mr.Cyan";
			colorNames[7] = L"Mr.Red";
			colorNames[8] = L"Mr.White";
			colorNames[9] = L"Mr.Lime";	
		}

		ColorDefines::~ColorDefines()
		{
		}
		Oyster::Math::Float3 ColorDefines::getGlowColor(int index)
		{
			return glowTintColor[index];
		}
		Oyster::Math::Float3 ColorDefines::getTintColor(int index)
		{
			return tintColor[index];
		}
		std::wstring ColorDefines::getColorName(int index)
		{
			return colorNames[index];
		}
		std::wstring ColorDefines::getPlayerName(int index)
		{
			return colorNames[index];
		}
	}
}

#endif // COLORDEFINES_H
