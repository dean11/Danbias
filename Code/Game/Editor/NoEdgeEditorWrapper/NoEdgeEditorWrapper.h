#ifndef EDITOR_EDITORWRAPPER_H
#define EDITOR_EDITORWRAPPER_H

#ifdef NOEDGE_EDITOR_DLL_EXPORT
	#define NOEDGE_EDITOR_DLL __declspec(dllexport)
#else
	#define NOEDGE_EDITOR_DLL __declspec(dllimport)
#endif

#define NO_EDGE_EXPORTER_MAJOR_VERSION  4
#define NO_EDGE_EXPORTER_MINOR_VERSION  0

#define NO_EDGE_IMPORTER_MAJOR_VERSION  4
#define NO_EDGE_IMPORTER_MINOR_VERSION  0

#include "Entity\NoEdgeTypes.h"
#define NOMINMAX
#include <Windows.h>

class Entity;
struct NamespaceForward;

class NOEDGE_EDITOR_DLL NoEdgeEditorWrapper
{
public:
	class NOEDGE_EDITOR_DLL NoEdgeEntity
	{
	public: // Set methods
		void SetPosition(float v[3]);
		void SetRotation(float v[3]);
		void SetScale(float v[3]);
		void RelativeMove(float v[3]);
		void RelativeRotate(float v[3]);
		void RelativeScale(float v[3]);
		void Lock();
		void Unlock();
		void Hide();
		void Show();

	public: // Get methods
		void GetPosition(float v[3]);
		void GetRotation(float v[3]);
		void GetScale(float v[3]);
		bool IsLocked();
		bool IsVisible();

	public:
		void Release();

	public:
		NoEdgeEntity(const NoEdgeEntity& obj);
		const NoEdgeEntity& operator=(const NoEdgeEntity& obj);
		~NoEdgeEntity();
		
	protected:
		
		NoEdgeEntity();
		NoEdgeEntity(Entity* ent);
		Entity* operator->() { return entity; }
		operator bool() { return this->reference != 0; }
		friend struct EditorData;
		friend class NoEdgeEditorWrapper;
		Entity* entity;
		int* reference;
	};

	class NOEDGE_EDITOR_DLL NoEdgeWorld :public NoEdgeEntity
	{
	public:
		NoEdgeWorld(const NoEdgeWorld& obj);
		const NoEdgeWorld& operator=(const NoEdgeWorld& obj);
		~NoEdgeWorld();
		
	private:
		NoEdgeWorld(Entity* ent);
		friend class NoEdgeEditorWrapper;
	};

	class NOEDGE_EDITOR_DLL NoEdgeLight :public NoEdgeEntity
	{
	public:
		NoEdgeLight(const NoEdgeLight& obj);
		const NoEdgeLight& operator=(const NoEdgeLight& obj);
		~NoEdgeLight();
		
	private:
		NoEdgeLight(Entity* ent);
		friend class NoEdgeEditorWrapper;
	};

	struct EditorInitDesc
	{
		struct MainOptions
		{
			HWND renderWindow;
			int width;
			int height;
		} mainOptions;
	};

	struct EditorSaveOptions
	{
		const wchar_t* mapPath;
		const wchar_t* worldName;
		const wchar_t* author;
		const wchar_t* previewThumb;
		const wchar_t* description;
		const wchar_t* gameModes;
		unsigned worldSize;
		unsigned version;
		unsigned playerLimit;
	};
	
public:
	static bool Initiate(EditorInitDesc& desc);
	static bool Frame();

	static void Release();

	static bool ImportMap(const wchar_t mapPath[]);
	static bool ExportMap(const EditorSaveOptions& op);

	static void OnResize( int width, int height );
	//static void OnEntityCreate(

public: /* Entity */
	static NoEdgeWorld		CreateWorldEntity();
	static NoEdgeLight		CreateEntity(NoEdgeType_Light objectType);
	static NoEdgeEntity		CreateEntity(NoEdgeType_Buildings objectType);
	static NoEdgeEntity		CreateEntity(NoEdgeType_Interactive objectType);
	static NoEdgeEntity		CreateEntity(NoEdgeType_Projectiles objectType);

private: /* Map import */
	static void ImportGameObject( NamespaceForward* oth, int ID);
	static void ImportLight( NamespaceForward* oth, int ID);

private: /* Map export */
	static void ExportLevelHeader(const EditorSaveOptions& op);
	static void ExportGameObject();
	static void ExportLight();

private:
	static void Update();
	static void Render();
	static bool InitiateDefault(EditorInitDesc& desc);
	static bool InitiateWindow(EditorInitDesc& desc);
	static bool Initiate3D(EditorInitDesc& desc);
	static bool InitiatePhysic(EditorInitDesc& desc);
	

	static Entity* PickObject(int x, int y);

	static LRESULT CALLBACK NoEdgeMsgCallback(HWND h, UINT m, WPARAM w, LPARAM l);

};

#endif // !EDITOR_EDITORWRAPPER_H
