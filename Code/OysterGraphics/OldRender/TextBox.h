#pragma once

#include "..\Engine.h"
const int MAX_LETTER_COUNT=60;
const int TEXT_NR_LETTERS=95;
const float TEXT_SIZE=2.5;
struct Text2D
{
	Oyster::Math::Float Pos;
	int offset;
	float coff;
};

namespace Oyster
{
	namespace Graphics
	{
		namespace Render
		{
			class Textbox
			{
			private:
				static float getCharID(char _in);
				static HRESULT CreateVertexBuffer();
				static HRESULT CreateTextfield(int _id);
			public:
				static Buffer TextBuffer;
				static int NumLetters;
				static ID3D11ShaderResourceView* Texture;

				static bool Init();
				static bool UpdateTextField(std::string _str);
				static bool SetTexture(const char* _file);

				//Updates a textbox with the certain id
				static void Update(std::string _str, float _scale);

				//Removes all old instances and recreates it with the input data
				static HRESULT Reset(int _count, std::string* _str, Float3* _pos);
				static void Apply(int _id);
			};
		}
	}
}
