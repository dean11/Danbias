#include "ShaderEffects.h"

namespace Oyster
{
	namespace Resources
	{
		Core::ShaderManager::ShaderEffect ShaderEffects::BasicSprite = Core::ShaderManager::ShaderEffect();
		Core::ShaderManager::ShaderEffect ShaderEffects::Text2DEffect = Core::ShaderManager::ShaderEffect();
		Core::ShaderManager::ShaderEffect ShaderEffects::ModelEffect = Core::ShaderManager::ShaderEffect();

		void ShaderEffects::Init()
		{
			BasicSprite.IAStage.Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			BasicSprite.Shaders.Vertex = Oyster::Core::ShaderManager::Get::Vertex(L"2D");
			BasicSprite.Shaders.Geometry = Oyster::Core::ShaderManager::Get::Geometry(L"2D");
			BasicSprite.Shaders.Pixel = Oyster::Core::ShaderManager::Get::Pixel(L"Texture0");

			D3D11_BLEND_DESC blendDesc;
			blendDesc.AlphaToCoverageEnable=false;
			blendDesc.IndependentBlendEnable=false;
			blendDesc.RenderTarget[0].BlendEnable=true;

			blendDesc.RenderTarget[0].SrcBlend =  D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;

			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			
			ID3D11BlendState* blender;

			Oyster::Core::Device->CreateBlendState(&blendDesc,&blender);

			BasicSprite.RenderStates.BlendState = blender;

			ID3D11InputLayout* layout;

			Oyster::Core::ShaderManager::CreateInputLayout(SpriteVertexDesc,1,Oyster::Core::ShaderManager::Get::Vertex(L"2D"),layout);

			BasicSprite.IAStage.Layout = layout;

			Text2DEffect.IAStage.Topology=D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			Text2DEffect.Shaders.Vertex = Oyster::Core::ShaderManager::Get::Vertex(L"Text");
			Text2DEffect.Shaders.Geometry = Oyster::Core::ShaderManager::Get::Geometry(L"Text");
			Text2DEffect.Shaders.Pixel = Oyster::Core::ShaderManager::Get::Pixel(L"Texture0");

			Oyster::Core::ShaderManager::CreateInputLayout(Text2DDesc,3,Oyster::Core::ShaderManager::Get::Vertex(L"Text"),layout);
			
			Text2DEffect.IAStage.Layout = layout;

			blendDesc.AlphaToCoverageEnable = true;
			Oyster::Core::Device->CreateBlendState(&blendDesc,&blender);
			Text2DEffect.RenderStates.BlendState = blender;

			ModelEffect.IAStage.Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			Oyster::Core::ShaderManager::CreateInputLayout(ModelDesc,3,Oyster::Core::ShaderManager::Get::Vertex(L"OBJ"),layout);
			ModelEffect.IAStage.Layout = layout;

			ModelEffect.Shaders.Vertex = Oyster::Core::ShaderManager::Get::Vertex(L"OBJ");
			ModelEffect.Shaders.Pixel = Oyster::Core::ShaderManager::Get::Pixel(L"OBJDEF");

			Oyster::Buffer::BUFFER_INIT_DESC desc;
	
			desc.ElementSize=sizeof(Oyster::Math::Float4x4);
			desc.NumElements = 1;
			desc.Usage = Oyster::Buffer::BUFFER_CPU_WRITE_DISCARD;
			desc.Type = Oyster::Buffer::CONSTANT_BUFFER_VS;
			desc.InitData = NULL;

			ModelEffect.CBuffers.Vertex.push_back(Oyster::Engine::Init::Buffers::CreateBuffer(desc));
			ModelEffect.CBuffers.Vertex.push_back(Oyster::Engine::Init::Buffers::CreateBuffer(desc));

			//use Oyster::Resources::Buffers::CbufferVS for per object data
			//perObject = Oyster::Engine::Init::Buffers::CreateBuffer(desc);
			
		}

		D3D11_INPUT_ELEMENT_DESC ShaderEffects::SpriteVertexDesc[1] =	
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11_INPUT_ELEMENT_DESC ShaderEffects::Text2DDesc[3] =
		{
			{"Position",0,		DXGI_FORMAT_R32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"Offset",0,		DXGI_FORMAT_R32_SINT,	0,	4,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"CharOffset",0,	DXGI_FORMAT_R32_FLOAT,	0,	8,	D3D11_INPUT_PER_VERTEX_DATA,	0},
		};

		D3D11_INPUT_ELEMENT_DESC ShaderEffects::ModelDesc[3] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}
}