#include "NoEdgeEditorWrapper.h"
#include "DllInterfaces\GFXAPI.h"
#include "GameLogic\Game.h"
#include "GamePhysics\PhysicsAPI.h"
#include "Camera_BasicV2.h"
#include "Camera.h"
#include "..\NoEdgeEditorWrapper.h"
#include "LevelLoader.h"
#include "Entity\Header\IncludeEntity.h"
#include "EditorDefault.h"
#include "Utilities.h"

#include <fstream>
#include <map>
#define NOMINMAX
#include <windowsx.h>


#define NOEDGECLASSNAME L"NoEdgeEditorRenderWin"
const wchar_t *WINDOW_CLASS_NAME = L"NOEDGERENDERWIN";

using namespace GameLogic;
using namespace Oyster;
using namespace Oyster::Graphics;
using namespace Oyster::Physics;
using namespace Oyster::Math;
using namespace std;

struct NoEdgeHistory
{

};

struct EditorData
{
	Camera*											currentCam;
	Utility::DynamicMemory::DynamicArray<Camera*>	cameras;

	Entity*											focusedEntity;
	Entity*											createdEntity;

	vector<NoEdgeEditorWrapper::NoEdgePlanet>		entityPlanet;
	vector<NoEdgeEditorWrapper::NoEdgeEntity>		entityInteractive;
	vector<NoEdgeEditorWrapper::NoEdgeEntity>		entityBuilding;
	vector<NoEdgeEditorWrapper::NoEdgeEntity>		entityProjectile;
	vector<NoEdgeEditorWrapper::NoEdgeEntity>		entityLight;
	
	HWND											renderWin;
	POINT											prevMousePos;
	bool											keyAlt;
	bool											keyShift;
	bool											keyCtrl;
	float											cameraSpeed;
	int												entityCount;

	EditorData()
		:focusedEntity(0)
	{
		this->cameraSpeed			= 1;
		this->currentCam			= 0;
		this->renderWin				= 0;
		this->keyAlt				= 0;
		this->keyShift				= 0;
		this->keyCtrl				= 0;
		this->entityCount			= 0;
		this->focusedEntity			= 0;
		memset(&this->prevMousePos, 0, sizeof(POINT));
	}
	~EditorData()
	{
		ReleaseMembers();
	}
	void ReleaseMembers()
	{
		for (unsigned int i = 0; i < entityPlanet.size(); i++)
			entityPlanet[i].Release();
		for (unsigned int i = 0; i < entityBuilding.size(); i++)
			entityBuilding[i].Release();
		for (unsigned int i = 0; i < entityInteractive.size(); i++)
			entityInteractive[i].Release();
		for (unsigned int i = 0; i < entityLight.size(); i++)
			entityLight[i].Release();
		for (unsigned int i = 0; i < entityProjectile.size(); i++)
			entityProjectile[i].Release();
		//for (unsigned int i = 0; i < cameras.Size(); i++)
		//	delete cameras[i];

		currentCam = 0;
		cameras.Clear();
		createdEntity = 0;
		entityPlanet.clear();
		entityInteractive.clear();
		entityBuilding.clear();
		entityProjectile.clear();
		entityLight.clear();
		renderWin = 0;
		keyAlt = 0;
		keyShift = 0;
		keyCtrl = 0;
		cameraSpeed = 0.0f;
		entityCount = 0;
	}

} data;

bool NoEdgeEditorWrapper::InitiateWindow(EditorInitDesc& desc)
{
	// Init win
	WNDCLASSEXW wc;
	wc.cbSize			= sizeof(WNDCLASSEXW);
	wc.hIconSm			= 0;
	wc.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= NoEdgeEditorWrapper::NoEdgeMsgCallback; 
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE)GetModuleHandle(0);
	wc.hIcon			= 0;
	wc.hCursor			= 0;
	wc.hbrBackground	= 0;
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= NOEDGECLASSNAME;

	if( !RegisterClassExW(&wc) ) return false;

	int width = desc.mainOptions.width + GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	int height= desc.mainOptions.height + GetSystemMetrics(SM_CYFIXEDFRAME)*2 + GetSystemMetrics(SM_CYCAPTION);
		
	RECT rectW;
	rectW.left					= 0;
	rectW.top					= 0;
	rectW.right					= width;
	rectW.bottom				= height;
	unsigned int styleW			= WS_CHILD | WS_VISIBLE; 
		
	data.renderWin = CreateWindowExW(	
										0, wc.lpszClassName, 0, styleW,
										rectW.left, rectW.top, rectW.right-rectW.left, rectW.bottom-rectW.top,
										desc.mainOptions.renderWindow, 0, wc.hInstance, 0);
	
	if(!data.renderWin)
	{
		printf("Failed to initiate window! Error code [%i]\n", GetLastError());
		return false;
	}
	
	return true;
}
bool NoEdgeEditorWrapper::Initiate3D(EditorInitDesc& desc)
{
	Oyster::Graphics::API::Option gfxOp;
	gfxOp.modelPath = Default::String::DEFAULT_MESH_PATH;
	gfxOp.texturePath = Default::String::DEFAULT_TEXTURE_PATH;
	gfxOp.resolution = Oyster::Math::Float2( (float)desc.mainOptions.width, (float)desc.mainOptions.height );
	gfxOp.ambientValue = 0.5f;
	gfxOp.fullscreen = false;
	gfxOp.globalGlowTint = Math::Float3(1.0f, 1.0f, 1.0f);
	gfxOp.globalTint = Math::Float3(1.0f, 1.0f, 1.0f);
	
	if(Oyster::Graphics::API::Init(data.renderWin, false, gfxOp) != Oyster::Graphics::API::Sucsess)
		return false;

	return true;
}
bool NoEdgeEditorWrapper::InitiatePhysic(EditorInitDesc& desc)
{
	Physics::API::Instance().Init();
	Physics::API::Instance().SetTimeStep(1.0f/120.0f);
	return true;
}
bool NoEdgeEditorWrapper::InitiateDefault(EditorInitDesc& desc)
{
	Graphics::API::BeginLoadingModels();
	{
		for (int i = 0; i < 10; i++)
		{
			NoEdgeEntity e(new EntityBuilding( NoEdgeType_Buildings_Building1, Float3(-10.0f + i*2, 4.0f, 10.0f) ));
		
			data.entityBuilding.push_back(e);
			data.entityCount++;
		}
	
		NoEdgeEntity light(new EntityLight(NoEdgeType_Light_PointLight));
		EntityLight::LightData lightData;
		lightData.pointLight.Radius = 1000.0f;
		lightData.pointLight.Color = Float3(1.0f);
		lightData.pointLight.Bright = 1000.0f;
		lightData.pointLight.Pos = Float3(1.0f, 20.0f, 10.0f);
		((EntityLight*)(light.entity))->SetLight(lightData);
		data.entityLight.push_back(light);
		data.entityCount++;
	
		Graphics::API::Option gfxOp = Graphics::API::GetOption();
		Float aspectRatio = gfxOp.resolution.x / gfxOp.resolution.y;
		data.cameras.Push(new Camera());
		data.currentCam = data.cameras[data.cameras.Size() - 1];
		data.currentCam->SetLens( Utility::Value::Radian(90.0f), aspectRatio, 0.1f, 1000.0f );
		Graphics::API::SetProjection( data.currentCam->Proj() );
		Graphics::API::SetView( data.currentCam->View() );
	}
	Graphics::API::EndLoadingModels();

	return true;
}
bool NoEdgeEditorWrapper::Initiate(EditorInitDesc& desc)
{
	memset(&data.prevMousePos, 0, sizeof(POINT));

	if(!InitiateWindow(desc))	return false;
	if(!Initiate3D(desc))		return false;
	if(!InitiatePhysic(desc))	return false;
	if(!InitiateDefault(desc))	return false;

	return true;
}


bool NoEdgeEditorWrapper::Frame()
{
	if(! data.renderWin ) return false;
	Update();
	Render();

	return true;
}

void NoEdgeEditorWrapper::Release()
{
	data.ReleaseMembers();
	Graphics::API::Clean();

	UnregisterClass( NOEDGECLASSNAME,  GetModuleHandle(0) );
}

void NoEdgeEditorWrapper::OnResize( int width, int height )
{
	Graphics::API::Option o = Graphics::API::GetOption();
	o.resolution = Float2((float)width, (float)height);
	Graphics::API::SetOptions(o);
}

NoEdgeEditorWrapper::NoEdgePlanet NoEdgeEditorWrapper::CreatePlanetEntity()
{
	if(! data.renderWin ) return false;
	NoEdgeEditorWrapper::NoEdgePlanet val(new EntityPlanet( data.currentCam->GetPosition() ));
	data.entityPlanet.push_back(val);
	return val;
}
NoEdgeEditorWrapper::NoEdgeEntity NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Buildings objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = new EntityBuilding( objectType, Float3(0.0f));
	
	return NoEdgeEditorWrapper::NoEdgeEntity(0);
}
NoEdgeEditorWrapper::NoEdgeEntity NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Interactive objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = (new EntityInteractive( objectType ));

	return NoEdgeEditorWrapper::NoEdgeEntity(0);
}
NoEdgeEditorWrapper::NoEdgeLight NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Light objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = ( new EntityLight(objectType));

	return NoEdgeEditorWrapper::NoEdgeLight(0);
}
NoEdgeEditorWrapper::NoEdgeEntity NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = (new EntityProjectile(objectType));
	
	return NoEdgeEditorWrapper::NoEdgeEntity(0);
}
NoEdgeEditorWrapper::NoEdgeEntity NoEdgeEditorWrapper::CreateEntity(NoEdgeType_HazardEnv objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = new EntityHazardousEnvironment(objectType, Float3(0.0f));
	
	return NoEdgeEditorWrapper::NoEdgeEntity(0);
}
NoEdgeEditorWrapper::NoEdgeEntity NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Pickup objectType)
{
	if(! data.renderWin ) return false;
	data.createdEntity = new EntityPickup(objectType);
	
	return NoEdgeEditorWrapper::NoEdgeEntity(0);
}


void NoEdgeEditorWrapper::Update()
{
	if(data.createdEntity)		data.createdEntity->Update();

	for (unsigned int i = 0; i < data.entityBuilding.size(); i++)
	{
		data.entityBuilding[i]->Update();
	}
	for (unsigned int i = 0; i < data.entityInteractive.size(); i++)
	{
		data.entityInteractive[i]->Update();
	}
	for (unsigned int i = 0; i < data.entityProjectile.size(); i++)
	{
		data.entityProjectile[i]->Update();
	}
}
bool AddToRenderList(Utility::DynamicMemory::DynamicArray<Oyster::Graphics::Model::Model>& l, Oyster::Graphics::Model::Model* m, int& tot)
{
	if(!m) return false;

	if((unsigned int)tot >= l.Size())
	{
		l.Push(*m);
		tot++;
	}
	else
	{
		l[tot++] = *m;
	}

	return true;
}
void NoEdgeEditorWrapper::Render()
{
	data.currentCam->UpdateViewMatrix();
	Oyster::Graphics::API::SetView( data.currentCam->View() );

	Graphics::API::NewFrame();
	{
		if(data.createdEntity)		data.createdEntity->Render();

		static Utility::DynamicMemory::DynamicArray<Oyster::Graphics::Model::Model> meshes;
		int tot = 0;

		for (unsigned int i = 0; i < data.entityBuilding.size(); i++)
		{
			AddToRenderList( meshes, (*data.entityBuilding[i].entity), tot );
		}
		for (unsigned int i = 0; i < data.entityInteractive.size(); i++)
		{
			AddToRenderList( meshes, (*data.entityInteractive[i].entity), tot );
		}
		for (unsigned int i = 0; i < data.entityProjectile.size(); i++)
		{
			AddToRenderList( meshes, (*data.entityProjectile[i].entity), tot );
		}
	
		if(meshes.Size() > 0)
			Graphics::API::RenderScene(&meshes[0], tot);

		//Render focused object
		if(data.focusedEntity)
		{
			Oyster::Graphics::API::StartRenderWireFrame();
				data.focusedEntity->Render();
		}
	}
	Graphics::API::EndFrame();
}

Entity* NoEdgeEditorWrapper::PickObject(int x, int y)
{
	Graphics::API::Option op = Graphics::API::GetOption();
	Entity* val = 0;
	float farZ = data.currentCam->GetFarZ();
	Matrix projMat = data.currentCam->Proj();
	Oyster::Collision3D::Ray ray;
	Float3 rayOrigin, rayDirection;
	float w = op.resolution.x;
	float h = op.resolution.y;
	float vx = (+2.0f * x / w - 1.0f) / projMat.m11;
	float vy = (-2.0f * y / h + 1.0f) / projMat.m22;

	ray.direction = Float3(vx, vy, 1.0f);
	ICustomBody* temp = Physics::API::Instance().Intersect(ray.origin.xyz, ray.direction.xyz * farZ);

	if(temp)
	{
		data.focusedEntity = (Entity*)temp->GetCustomTag();
	}
	else
	{
		data.focusedEntity = 0;
	}
	return val;
}

LRESULT CALLBACK NoEdgeEditorWrapper::NoEdgeMsgCallback(HWND hwnd, UINT m, WPARAM w, LPARAM l)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (m) 
	{
		case WM_LBUTTONDOWN:
		{
			SetFocus(hwnd);
			POINT p;
			p.x = GET_X_LPARAM(l); 
			p.y = GET_Y_LPARAM(l); 
			Entity* pic = NoEdgeEditorWrapper::PickObject(p.x, p.y);
			if(pic)
			{

			}
		} return FALSE;
		case WM_MBUTTONDOWN:
			SetFocus(hwnd);
			return FALSE;
		case WM_RBUTTONDOWN:
			SetFocus(hwnd);
			if(data.createdEntity)
			{
				data.createdEntity->Release();
				data.createdEntity = 0;
			}
			
			data.focusedEntity = 0;
			
			return FALSE;
		case WM_LBUTTONDBLCLK:
			return FALSE;
		case WM_MBUTTONDBLCLK:
			return FALSE;
		case WM_RBUTTONDBLCLK:
			return FALSE;
		case WM_LBUTTONUP:
			if(data.createdEntity)
			{
					data.focusedEntity = data.createdEntity;
				switch (data.createdEntity->GetEntityType())
				{
				case Entity::EntityType_Building:
					data.entityBuilding.push_back(data.focusedEntity);
				break;
				case Entity::EntityType_Interactive:
					data.entityInteractive.push_back(data.focusedEntity);
				break;
				case Entity::EntityType_Light:
					data.entityLight.push_back(data.focusedEntity);
				break;
				case Entity::EntityType_Projectiles:
					data.entityProjectile.push_back(data.focusedEntity);
				break;
				//case Entity::EntityType_World:
				//	data.entityWorld.push_back(data.focusedEntity);
				//break;
				}
				data.createdEntity = 0;
			}
			return FALSE;
		case WM_MBUTTONUP:
			return FALSE;
		case WM_RBUTTONUP:
			return FALSE;

		case WM_MOUSEMOVE:
		{
			int k = GET_KEYSTATE_WPARAM(w);
			int delta = GET_WHEEL_DELTA_WPARAM(w);
			POINT p;
			p.x = GET_X_LPARAM(l); 
			p.y = GET_Y_LPARAM(l);

			if(data.createdEntity)
			{
				data.createdEntity->SetPosition(Float3((float)data.prevMousePos.x, (float)data.prevMousePos.y, data.createdEntity->GetPosition().z));
			}
			if(data.keyAlt)
			{
				if(k == VK_LBUTTON)
				{
					data.currentCam->Yaw	((float) (p.x - data.prevMousePos.x) );
					data.currentCam->Pitch	((float) -(p.y - data.prevMousePos.y) );
				}
				else if(k == MK_MBUTTON)
				{
					float dx = (float) (p.x - data.prevMousePos.x);
					float dy = (float)(p.y - data.prevMousePos.y);
					Float3 r = data.currentCam->GetRight()  * dx * data.cameraSpeed;
					Float3 up = data.currentCam->GetUp() * dy * data.cameraSpeed;
					Float3 dir = (r + up);
					data.currentCam->SetPosition(data.currentCam->GetPosition() + (dir) );
				}
			}

			data.prevMousePos = p;

			return FALSE; 
		}

		case WM_MOUSEWHEEL:// WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
			{
				int k = GET_KEYSTATE_WPARAM(w);
				int delta = GET_WHEEL_DELTA_WPARAM(w);
				POINT p;
				p.x = GET_X_LPARAM(l); 
				p.y = GET_Y_LPARAM(l); 
				ScreenToClient(hwnd, &p);

				if(delta > 0) //Towards
				{
					data.currentCam->Walk(data.cameraSpeed);
				}
				else			//Away
				{
					data.currentCam->Walk(-data.cameraSpeed);
				}
			}
			return FALSE;

		case WM_CREATE: 
			return FALSE;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_IME_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			switch (w)
			{
			case VK_MENU:
				data.keyAlt = true;
				break;
			case VK_CONTROL:
				data.keyCtrl = true;
				break;
			case VK_SHIFT:
				data.keyShift = true;
				break;
			}
		}
		break;
		case WM_IME_KEYUP:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			switch (w)
			{
			case VK_MENU:
				data.keyAlt = false;
				break;
			case VK_CONTROL:
				data.keyCtrl = false;
				break;
			case VK_SHIFT:
				data.keyShift = false;
				break;
			}
		}
		break;
	}

	return DefWindowProc(hwnd, m, w, l);
}

#pragma region NoEdgeEntity

	void NoEdgeEditorWrapper::NoEdgeEntity::SetPosition(float v[3])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::SetRotation(float v[4])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::SetScale(float v[3])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::RelativeMove(float v[3])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::RelativeRotate(float v[3])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::RelativeScale(float v[3])
	{

	}
	void NoEdgeEditorWrapper::NoEdgeEntity::Lock()
	{
		this->entity->Lock();
	}
	void NoEdgeEditorWrapper::NoEdgeEntity::Unlock()
	{
		this->entity->Unlock();
	}
	void NoEdgeEditorWrapper::NoEdgeEntity::Hide()
	{
		this->entity->Hide();
	}
	void NoEdgeEditorWrapper::NoEdgeEntity::Show()
	{
		this->entity->Show();
	}

	void NoEdgeEditorWrapper::NoEdgeEntity::GetPosition(float v[3])
	{
		v = this->entity->GetPosition();
	}
	void NoEdgeEditorWrapper::NoEdgeEntity::GetRotation(float v[4])
	{
		v = this->entity->GetRotation();
	}
	void NoEdgeEditorWrapper::NoEdgeEntity::GetScale(float v[3])
	{
		v = this->entity->GetScale();
	}
	bool NoEdgeEditorWrapper::NoEdgeEntity::IsLocked()
	{
		return this->entity->IsLocked();
	}
	bool NoEdgeEditorWrapper::NoEdgeEntity::IsVisible()
	{
		return this->entity->IsVisible();
	}

	void NoEdgeEditorWrapper::NoEdgeEntity::Release()
	{
		if(this->reference && this->entity)
		{
			if(--(*this->reference) == 0)
			{
				this->entity->Release();
				delete this->entity;
				delete this->reference;
				this->reference = 0;
			}
			this->entity = 0;
		}
	}

	NoEdgeEditorWrapper::NoEdgeEntity::NoEdgeEntity(const NoEdgeEntity& obj)
	{
		this->reference = obj.reference;
		this->entity = obj.entity;

		if(this->reference)
		{
			(*(this->reference))++;
		}
	}
	const NoEdgeEditorWrapper::NoEdgeEntity& NoEdgeEditorWrapper::NoEdgeEntity::operator=(const NoEdgeEntity& obj)
	{
		this->Release();

		this->reference = obj.reference;
		this->entity = obj.entity;

		if(this->reference)
		{
			(*(this->reference))++;
		}
		return *this;
	}
	NoEdgeEditorWrapper::NoEdgeEntity::~NoEdgeEntity()
	{
		Release();
	}
	NoEdgeEditorWrapper::NoEdgeEntity::NoEdgeEntity(Entity* ent)
	{  
	
		this->entity = ent;

		if(! ent ) return;

		this->reference = new int(1);
	}
	

#pragma endregion

#pragma region NoEdgeWorld

	NoEdgeEditorWrapper::NoEdgePlanet::NoEdgePlanet(const NoEdgePlanet& obj)
		:NoEdgeEditorWrapper::NoEdgeEntity(obj)
	{
		this->reference = obj.reference;
		this->entity = obj.entity;

		if(this->reference)
		{
			(*(this->reference))++;
		}
	}
	const NoEdgeEditorWrapper::NoEdgePlanet& NoEdgeEditorWrapper::NoEdgePlanet::operator=(const NoEdgeEditorWrapper::NoEdgePlanet& obj)
	{ return *this; }
	NoEdgeEditorWrapper::NoEdgePlanet::~NoEdgePlanet()
	{ }
	NoEdgeEditorWrapper::NoEdgePlanet::NoEdgePlanet(Entity* ent)
		:NoEdgeEditorWrapper::NoEdgeEntity(ent)
	{ }

#pragma endregion

#pragma region NoEdgeLight

	NoEdgeEditorWrapper::NoEdgeLight::NoEdgeLight(const NoEdgeLight& obj)
		:NoEdgeEditorWrapper::NoEdgeEntity(obj)
	{ }
	const NoEdgeEditorWrapper::NoEdgeLight& NoEdgeEditorWrapper::NoEdgeLight::operator=(const NoEdgeEditorWrapper::NoEdgeLight& obj)
	{ return *this; }
	NoEdgeEditorWrapper::NoEdgeLight::~NoEdgeLight()
	{ }
	NoEdgeEditorWrapper::NoEdgeLight::NoEdgeLight(Entity* ent)
		:NoEdgeEditorWrapper::NoEdgeEntity(ent)
	{ }

#pragma endregion

#pragma region IO

std::wofstream outStream;
FILE* fout;

struct NamespaceForward
{
	ObjectTypeHeader* value;
};
inline Quaternion ArrayToQuaternion( const float source[4] )
{
	return Quaternion( Float3(source[0], source[1], source[2]), source[3] );
}

#pragma region LOADING
	bool NoEdgeEditorWrapper::ImportMap( const wchar_t mapPath[] )
	{
		bool val = true;
		//mapPath
		LevelLoader loader;
		auto objects = loader.LoadLevel( Utility::String::WStringToString( mapPath , std::string() ) );
		auto object = objects.begin();
		ObjectTypeHeader *oth;

		int objectID = 100; // first 100 is reserved for players. This is how the server does it.

		for( ; object != objects.end(); ++object )
		{
			++objectID;
			oth = (ObjectTypeHeader*)(*object._Ptr);
			switch( oth->typeID )
			{
			case ObjectType::ObjectType_Static:
			case ObjectType::ObjectType_Dynamic:
				{
					NamespaceForward temp;
					temp.value = oth;
					ImportGameObject(&temp, objectID);
				}
				break;
			case ObjectType::ObjectType_Light:
				{
					BasicLight *light = (BasicLight*)oth;

					EntityLight::LightData l;
					l.pointLight.Color	= light->color;
					l.pointLight.Pos	= light->position;
					l.pointLight.Bright	= light->intensity;
					l.pointLight.Radius	= light->radius; 

					EntityLight *newLight = new EntityLight( NoEdgeType_Light_PointLight );
					newLight->SetLight(l);
					data.entityLight.push_back(newLight);
				}
				break;
			default: break;
			}
		}

		Graphics::API::EndLoadingModels();

		return val;
	}
	void NoEdgeEditorWrapper::ImportGameObject( NamespaceForward* oth, int ID)
	{
		ObjectHeader *oh = (ObjectHeader*)oth;

		//ModelInitData desc;
		//desc.id			= ID;
		//Utility::String::StringToWstring( oh->ModelFile, desc.modelPath );
		//desc.position	= oh->position; 
		//desc.rotation	= ArrayToQuaternion( oh->rotation );
		//desc.scale		= oh->scale;
		//desc.visible	= true; 

		Entity* ent = 0;

		switch (oh->specialTypeID)
		{
			case ObjectSpecialType_Sky:
			{
				//NoEdgeEditorWrapper::CreateAtmosphereEntity();
			} break;
			case ObjectSpecialType_World:		//Always the main celestial body
			{
				NoEdgeEditorWrapper::CreatePlanetEntity();
			} break;
			case ObjectSpecialType_Building:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Buildings_Building1);
			} break;
			case ObjectSpecialType_Stone:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles_Stone);
			} break;
			case ObjectSpecialType_StandardBox:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles_Stone);
			} break;
			case ObjectSpecialType_RedExplosiveBox:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Interactive_ExplosiveBox);
			}break;
			case ObjectSpecialType_SpikeBox:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles_SpikeBox);
			} break;
			case ObjectSpecialType_Spike:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles_Spike);
			} break;
			case ObjectSpecialType_CrystalFormation:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_HazardEnv_CrystalFormation1);
			} break;
			case ObjectSpecialType_CrystalShard:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Projectiles_CrystalShard);
			} break;
			case ObjectSpecialType_JumpPad:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Interactive_JumpPad);
			} break;
			case ObjectSpecialType_Portal:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Interactive_Portal);
			}
			case ObjectSpecialType_Generic:
			{
				//NoEdgeEditorWrapper::CreateEntity(NoEdgeType_);
			} break;
			case ObjectSpecialType_PickupHealth:
			{
				NoEdgeEditorWrapper::CreateEntity(NoEdgeType_Pickup_HealthPackMedium);
			} break;
	
		}

	
		//if(object)
		//{
		//	if(object->Init(desc))
		//	{
		//		// RB DEBUG
		//		RBInitData RBData;
		//		if(oh->boundingVolume.geoType == CollisionGeometryType_Box)
		//		{
		//			RBData.position = ((Float3)oh->position + (Float3)oh->boundingVolume.box.position) * (Float3)oh->scale;
		//			RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
		//			RBData.scale =  (Float3)oh->scale * (Float3)oh->boundingVolume.box.size * 2;
		//			RBData.type = RB_Type_Cube;
		//			object->InitRB( RBData );
		//		}
		//
		//		if(oh->boundingVolume.geoType == CollisionGeometryType_Sphere)
		//		{
		//			RBData.position = ((Float3)oh->position + (Float3)oh->boundingVolume.box.position) * (Float3)oh->scale;
		//			RBData.rotation = ArrayToQuaternion( oh->rotation ); // Only model rotation 
		//			RBData.scale =  (Float3)oh->scale * oh->boundingVolume.sphere.radius * 2;
		//			RBData.type = RB_Type_Sphere;
		//			object->InitRB( RBData );
		//		}
		//		// !RB DEBUG 
		//
		//		if(oth->typeID == ObjectType::ObjectType_Static)
		//		{
		//			(*this->privData->staticObjects)[ID] = (C_StaticObj*)object;
		//		}
		//		else
		//		{
		//			if(oth->typeID == ObjectType::ObjectType_Dynamic)
		//			{
		//				(*this->privData->dynamicObjects)[ID] = (C_DynamicObj*)object;
		//			}
		//		}
		//			
		//	}
		//	else
		//	{
		//		delete object;
		//	}
		//}
	}
	void NoEdgeEditorWrapper::ImportLight( NamespaceForward* oth, int ID)
	{

	}
#pragma endregion LOADING END

#pragma region SAVING
	bool NoEdgeEditorWrapper::ExportMap(const EditorSaveOptions& op)
	{
		outStream.open(op.mapPath, std::ios::binary);
		_wfopen_s(&fout, op.mapPath, L"wb");
		fclose(fout);
		if(outStream.is_open())
		{
			outStream << NO_EDGE_EXPORTER_MAJOR_VERSION << NO_EDGE_EXPORTER_MINOR_VERSION;

			ExportLevelHeader(op);

			outStream.close();
			return true;
		}
		return false;
	}
	void NoEdgeEditorWrapper::ExportLevelHeader(const EditorSaveOptions& op)
	{
		//fwprintf(
		/* Header ID for LevelMetaData */	outStream << 0;

		/* string size of levelName */		outStream << lstrlenW( op.worldName );
		/* level name as string */			outStream << op.worldName;						//FIX: Can't just print a pointer..
		/* Map version as uint*/			outStream << op.version;
		/* string size of description */	outStream << lstrlenW(op.description);
		/* description as string */			outStream << op.description;					//FIX: Can't just print a pointer..
		/* string size author */			outStream << lstrlenW(op.author);
		/* author as string */				outStream << op.author;							//FIX: Can't just print a pointer..
		/* max players */					outStream << op.playerLimit;
		/* worldSize */						outStream << op.worldSize;
		/* string size of previewFile */	outStream << lstrlenW(op.previewThumb);
		/* previewName */					outStream << op.previewThumb;					//FIX: Can't just print a pointer..
		/* number of game modes */			outStream << lstrlenW(op.gameModes);
		/* gamemode collection */			outStream << op.gameModes;						//FIX: Can't just print a pointer..
	}
	void NoEdgeEditorWrapper::ExportGameObject()
	{

	}
	void NoEdgeEditorWrapper::ExportLight()
	{

	}
#pragma endregion SAVING END

#pragma endregion