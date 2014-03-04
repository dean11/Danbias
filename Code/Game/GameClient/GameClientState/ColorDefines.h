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

		private:
			Oyster::Math::Float3	tintColor[10];
			Oyster::Math::Float3	glowTintColor[10];
			std::wstring			colorNames[10];
		};

		ColorDefines::ColorDefines()
		{
			// TODO make nice colors 
			tintColor[0] = Oyster::Math::Float3( 0.3f, 0.7f, 0.7f ); 
			tintColor[1] = Oyster::Math::Float3( 0.3f, 0.7f, 0.7f );
			tintColor[2] = Oyster::Math::Float3( 0.3f, 0.7f, 0.7f );
			tintColor[3] = Oyster::Math::Float3( 0.7f, 0.3f, 0.7f );
			tintColor[4] = Oyster::Math::Float3( 0.7f, 0.3f, 0.7f );
			tintColor[5] = Oyster::Math::Float3( 0.7f, 0.3f, 0.7f );
			tintColor[6] = Oyster::Math::Float3( 0.7f, 0.7f, 0.3f );
			tintColor[7] = Oyster::Math::Float3( 0.7f, 0.7f, 0.3f );
			tintColor[8] = Oyster::Math::Float3( 0.7f, 0.7f, 0.3f );
			tintColor[9] = Oyster::Math::Float3( 0.7f, 0.7f, 0.5f );

			glowTintColor[0] = Oyster::Math::Float3( 0.8f, 0.0f, 0.0f );
			glowTintColor[1] = Oyster::Math::Float3( 0.0f, 0.8f, 0.0f );
			glowTintColor[2] = Oyster::Math::Float3( 0.0f, 0.0f, 0.8f );
			glowTintColor[3] = Oyster::Math::Float3( 0.6f, 0.0f, 0.3f );
			glowTintColor[4] = Oyster::Math::Float3( 0.0f, 0.6f, 0.6f );
			glowTintColor[5] = Oyster::Math::Float3( 0.3f, 0.0f, 0.6f );
			glowTintColor[6] = Oyster::Math::Float3( 0.3f, 0.0f, 0.6f );
			glowTintColor[7] = Oyster::Math::Float3( 0.0f, 0.3f, 0.6f );
			glowTintColor[8] = Oyster::Math::Float3( 0.0f, 0.6f, 0.3f );
			glowTintColor[9] = Oyster::Math::Float3( 0.0f, 0.9f, 0.0f );

			colorNames[0] = L"Mr.Red";
			colorNames[1] = L"Mr.Green";
			colorNames[2] = L"Mr.Blue";
			colorNames[3] = L"Mr.Pink";
			colorNames[4] = L"Mr.Turquoise";
			colorNames[5] = L"Mr.Something";
			colorNames[6] = L"Mr.Something";
			colorNames[7] = L"Mr.Something";
			colorNames[8] = L"Mr.Something";
			colorNames[9] = L"Mr.Something";	
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
	}
}

#endif // COLORDEFINES_H
