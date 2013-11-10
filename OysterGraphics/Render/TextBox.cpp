#include "Textbox.h"
using namespace Oyster::Render;

//int Textbox::NumTextfields=0;
Oyster::Buffer Textbox::TextBuffer;
int Textbox::NumLetters;
//std::vector<TextInstanceData> Textbox::TextInstances;
ID3D11ShaderResourceView* Textbox::Texture=NULL;
bool Textbox::Init()
{
	//NumTextfields = 0;
	//NumVertices = 0;
	//HRESULT test=HRESULT_FAI
	if(FAILED(CreateVertexBuffer()))
	{
		return false;
	}
	//update(_str, _pos);
	return true;
	//return true;
}
/// \todo proper reading of texture
bool Textbox::SetTexture(const char* _file)
{
	return false;
	/*if(FAILED(D3DX11CreateShaderResourceViewFromFileA(Oyster::Core::Device, _file, NULL, NULL, &Texture, NULL)))
	{
		return false;
	}*/
	return true;
}
bool Textbox::UpdateTextField(std::string _str)
{
	//DEPRECATED
	//Update(_str);
	return false;
	/*UINT _id=TextInstances.size();
	//TextInstances.resize(_id+1);
	Text2D newD;
	TextInstances.push_back(newD);
	if(FAILED(CreateTextfield(_id)))
	{
		return false;
	}
	Update(_id, _str, _pos);
	TextInstances[_id].Visible=true;
	TextInstances[_id].World=Float4x4::identity;
	NumTextfields++;
	return true;*/
}

HRESULT Textbox::CreateVertexBuffer()
{
	HRESULT result;
	//Create vertices
	/*
	std::vector<TextData> mVertices;
	mVertices.resize(4);
	D3DXVECTOR3 testloc=D3DXVECTOR3(0,0,0);
	mVertices[0].pos = testloc+D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mVertices[1].pos = testloc+D3DXVECTOR3(TEXT_SIZE, 0.0f, 0.0f);
	mVertices[2].pos = testloc+D3DXVECTOR3(0.0f, TEXT_SIZE, 0.0f);
	mVertices[3].pos = testloc+D3DXVECTOR3(TEXT_SIZE, TEXT_SIZE, 0.0f);
	float normaloffset=(1.0f/TEXT_NR_LETTERS);
	mVertices[0].uv=D3DXVECTOR2(normaloffset,1);
	mVertices[1].uv=D3DXVECTOR2(0,1);
	mVertices[2].uv=D3DXVECTOR2(normaloffset,0);
	mVertices[3].uv=D3DXVECTOR2(0,0);
	*/
	
	//Oyster::Buffer::BUFFER_INIT_DESC desc;
	//desc.ElementSize=sizeof(TextData);
	//desc.NumElements = mVertices.size();
	//desc.Type = Oyster::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
	//desc.Usage = Oyster::Buffer::BUFFER_USAGE::BUFFER_DEFAULT;
	//desc.InitData = &mVertices[0];
		
	//result=TextBuffer.Init(desc);
	//NumVertices=mVertices.size();
	//TextInstances[_id].NumLetters=0;
	/*Text2D tmp;
	tmp.coff=0;
	tmp.offset=0;
	tmp.Pos=Float2(0,0);*/
	Oyster::Buffer::BUFFER_INIT_DESC desc;
	desc.ElementSize=sizeof(Text2D);
	desc.NumElements = MAX_LETTER_COUNT;
	desc.Type = Oyster::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
	desc.Usage = Oyster::Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;
	desc.InitData = 0;
	result=TextBuffer.Init(desc);
	NumLetters=0;
	return result;
}
HRESULT Textbox::CreateTextfield(int _id)
{
	HRESULT result=E_FAIL;
	/*if (TextInstances.size()>_id)
	{
		TextInstances[_id].NumLetters=0;

		Oyster::Buffer::BUFFER_INIT_DESC desc;
		desc.ElementSize=sizeof(Text2D);
		desc.NumElements = MAX_LETTER_COUNT;
		desc.Type = Oyster::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
		desc.Usage = Oyster::Buffer::BUFFER_USAGE::BUFFER_CPU_WRITE_DISCARD;
		desc.InitData = 0;

		result=TextInstances[_id].InstanceBuffer.Init(desc);
	}*/
	return result;
}
void Textbox::Update(std::string _str, float _xscale)
{
	UINT instances=0;
	Text2D tmpInst;

	void* dest = TextBuffer.Map();
	Text2D* dataView = reinterpret_cast<Text2D*>(dest);
	//tmpInst.charOffset=_pos;
	for (unsigned int i=0; i<_str.length(); i++)
	{
		tmpInst.coff=(1.0f/TEXT_NR_LETTERS);
		tmpInst.offset=(_str[i]-32);
		tmpInst.Pos=i*(0.005f*_xscale);
		//float tst=getCharID(_str[i]);
		//tmpInst.offset=tst;
		//tmpInst.charOffset.x=_pos.x-i*TEXT_SIZE;
		//tmpInst.data=tst;
		dataView[instances]=tmpInst;
		instances++;
	}
	NumLetters=instances;
	//TextInstances[_id].NumLetters=instances;
	TextBuffer.Unmap();
}
float Textbox::getCharID(char _in)
{
	//int charid=_in;
	//float charid=((_in-'0')-32);
	return ((_in-32)*(1.0f/TEXT_NR_LETTERS));
	//return _in-'0';
}
void Textbox::Apply(int _id)
{
	//Check if the subset exists, so we don't try to pain something that isn't there resulting in a crash
	//if (NumTextfields>_id)
	//{
		//Oyster::Core::DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//Set the size of each vertex "jump", and the start point for the shader
		//unsigned int strides[2];
		//offsets start at 0.
		//unsigned int offsets[2]={0,0};

		//Load the strides with the size of each type
		//strides[0] = sizeof(TextData); 
		//strides[1] = sizeof(PerCharData);

		//Create an array which points to the buffers needed.
		//ID3D11Buffer* bufferPointers[2];
		//bufferPointers[0] = TextBuffer;	
		//bufferPointers[1] = TextInstances[_id].InstanceBuffer;
		//Load the vertex buffer into the shader
		//Oyster::Core::DeviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
		//Get the basic info of the technique that's loaded
	//}
}