#include "EditorCLIWrapper.h"
#include <string>
#include < stdio.h >
#include < stdlib.h >
#include < vcclr.h >


namespace System { namespace Windows { namespace Interop
{
	EditorCLIWrapper::EditorCLIWrapper()
	{
	}

	EditorCLIWrapper::~EditorCLIWrapper()
	{
	}

	EditorReturnCode EditorCLIWrapper::InitiateCore(InitDesc desc)
	{
		NoEdgeEditorWrapper::EditorInitDesc d;
		d.mainOptions.height = desc.mainOptions.height;
		d.mainOptions.width = desc.mainOptions.width;
		HWND win = (HWND)desc.mainOptions.renderWindow.ToPointer();
		d.mainOptions.renderWindow = win;

		if(!NoEdgeEditorWrapper::Initiate(d))	return EditorReturnCode::EditorReturnCode_Error;

		return EditorReturnCode::EditorReturnCode_Sucess;
	}

	bool EditorCLIWrapper::LoadMap(String^ mapPath)
	{
		pin_ptr<const wchar_t> wch = PtrToStringChars(mapPath);
		std::wstring m = wch;
		return NoEdgeEditorWrapper::ImportMap(m.c_str());
	}

	void EditorCLIWrapper::Frame()
	{
		NoEdgeEditorWrapper::Frame();
	}
	void EditorCLIWrapper::Release()
	{
		NoEdgeEditorWrapper::Release();
	}

	void EditorCLIWrapper::OnResize( int width, int height )
	{
		NoEdgeEditorWrapper::OnResize(width, height);
	}

	GamePlanet^ EditorCLIWrapper::CreatePlanet()
	{
		GamePlanet ^e = gcnew GamePlanet( new NoEdgeEditorWrapper::NoEdgePlanet( NoEdgeEditorWrapper::CreatePlanetEntity() ) );

		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_Buildings object)
	{
		GameEntity ^e = gcnew GameEntity( new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_Buildings)object) ) );
		
		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_Interactive object)
	{
		GameEntity ^e = gcnew GameEntity( new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_Interactive)object) ) );

		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_Light object)
	{
		GameEntity ^e = gcnew GameEntity(new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_Light)object) ));
		
		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_Projectiles object)
	{
		GameEntity ^e = gcnew GameEntity(new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_Projectiles)object) ));
		
		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_Pickup object)
	{
		GameEntity ^e = gcnew GameEntity(new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_Pickup)object) ));
		
		return e;
	}
	GameEntity^ EditorCLIWrapper::CreateEntity(EnumBridge::NoEdgeType_HazardEnv object)
	{
		GameEntity ^e = gcnew GameEntity(new NoEdgeEditorWrapper::NoEdgeEntity( NoEdgeEditorWrapper::CreateEntity((NoEdgeType_HazardEnv)object) ));
		
		return e;
	}


#pragma region GameWorld

	GamePlanet::GamePlanet(NoEdgeEditorWrapper::NoEdgePlanet* w)
	{
		this->planet = w;
	}
	GamePlanet::~GamePlanet()
	{
		this->!GamePlanet();
	}
	GamePlanet::!GamePlanet()
	{
		this->planet->Release();
	}
	void GamePlanet::SetPosition(float v[3])
	{
		this->planet->SetPosition(v);
	}
	void GamePlanet::SetRotation(float v[3])
	{
		this->planet->SetRotation(v);
	}
	void GamePlanet::SetScale(float v[3])
	{
		this->planet->SetScale(v);
	}
	void GamePlanet::GetPosition(float v[3])
	{
		this->planet->GetPosition(v);
	}
	void GamePlanet::GetRotation(float v[3])
	{
		this->planet->GetRotation(v);
	}
	void GamePlanet::GetScale(float v[3])
	{
		this->planet->GetScale(v);
	}
	void GamePlanet::Release()
	{
		this->planet->Release();
	}

#pragma endregion

#pragma region GameEntity

	GameEntity::GameEntity(NoEdgeEditorWrapper::NoEdgeEntity* e)
	{
		this->entity = e;
	}
	GameEntity::~GameEntity()
	{
		this->!GameEntity();
	}
	GameEntity::!GameEntity()
	{
		this->entity->Release();
	}
	void GameEntity::SetPosition(float v[3])
	{
		this->entity->SetPosition(v);
	}
	void GameEntity::SetRotation(float q[4])
	{
		this->entity->SetRotation(q);
	}
	void GameEntity::SetScale(float v[3])
	{
		this->entity->SetScale(v);
	}
	void GameEntity::RelativeMove(float v[3])
	{
		this->entity->RelativeMove(v);
	}
	void GameEntity::RelativeRotate(float v[4])
	{
		this->entity->RelativeRotate(v);
	}
	void GameEntity::RelativeScale(float v[3])
	{
		this->entity->RelativeScale(v);
	}
	void GameEntity::Lock()
	{
		this->entity->Lock();
	}
	void GameEntity::Unlock()
	{
		this->entity->Unlock();
	}
	void GameEntity::Hide()
	{
		this->entity->Hide();
	}
	void GameEntity::Show()
	{
		this->entity->Show();
	}
	void GameEntity::GetPosition(float v[3])
	{
		this->entity->GetPosition(v);
	}
	void GameEntity::GetRotation(float v[4])
	{
		this->entity->GetRotation(v);
	}
	void GameEntity::GetScale(float v[3])
	{
		this->entity->GetScale(v);
	}
	bool GameEntity::IsLocked()
	{
		return this->entity->IsLocked();
	}
	bool GameEntity::IsVisible()
	{
		return this->entity->IsVisible();
	}
	void GameEntity::Release()
	{
		this->entity->Release();
	}

#pragma endregion
}}}







