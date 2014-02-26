#ifndef MISC_RESOURCELOADER_H
#define MISC_RESOURCELOADER_H

#include <string>
#include <map>

namespace Oyster 
{ 
	namespace Resource
	{
		struct ResourceData;

		typedef void* HRESOURCE;
		typedef char HBYTE;
		typedef HBYTE* HBYTEARRAY;

		/** Typedef on a fuction required for custom unloading */
		typedef void(*UnloadFunction)(void* loadedData);

		/** Typedef on a fuction required for custom loading */
		typedef void*(*LoadFunction)(const wchar_t filename[]);

		/** An enum class representing all avalible resources that is supported. */
		enum ResourceType
		{
			//Byte
			ResourceType_Byte_Raw,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_ANSI,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF8,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UNICODE,				/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF16LE,				/**< Handle can be interpeted as char[] or char* */

			ResourceType_CUSTOM,					/**< Handle can be interpeted as whatever */
			ResourceType_INVALID,					/**< Handle can be interpeted as whatever */
		};
		
		/** A resource handler interface to interact with when loading resources. 
		*	The resource handler uses the filename to make resources unuiqe.
		*/
		class ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			/**
			*	Load a resource given a type.
			*	@param filename The path to the resource.
			*	@param customId An optional custom ID to associate with the resource.
			*	@param type The resource type to load.
			*	@param force If set to true, the resource will be reloaded if it already exists. If it does not, nothing happens.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			HBYTEARRAY		LoadBytes(const wchar_t filename[], ResourceType type, int customId = -1, bool force = false);

			/**
			*	Load a resource with a custom loading function
			*	@param filename The path to the resource.
			*	@param loadFnc The function that will load the data.
			*	@param unloadFnc The function that will unload the loaded data.
			*	@param customId An optional custom ID to associate with the resource.
			*	@param force If set to true, the resource will be removed and loaded if exists.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			HRESOURCE		LoadResource(const wchar_t filename[], LoadFunction loadFnc = 0, UnloadFunction unloadFnc = 0, int customId = -1, bool force = false);

			/**
			*	Reload a resource
			*	@param filename The path to the resource.
			*	@return If function suceeds, the return value is the reloaded resource.
			*/
			HRESOURCE		ReloadResource(const wchar_t filename[]);

			/**
			*	Reload a resource
			*	@param filename The path to the resource.
			*	@return If function suceeds, the return value is the reloaded resource.
			*/
			HRESOURCE		ReloadResource(HRESOURCE& resource);

			/**
			*	Releases all resources loaded by the resource handler.
			*	@return Nothing
			*/
			void			Clean();

			/**
			*	Release a reference to the resource handle
			*	@param resource The handle to release.
			*	@return Nothing
			*/
			void			ReleaseResource(const HRESOURCE& resource);

			/**
			*	Release a reference to the resource handle
			*	@param resource The resource filename to release reference.
			*	@return Nothing
			*/
			void			ReleaseResource(const wchar_t filename[]);

			/**	Set a user defined ID
			*	@param resource A handle to accociate the id with.
			*	@param id A user defined identifier that the resource handler does not touch.
			*/
			void			SetResourceId(const HRESOURCE& resource, unsigned int id);

			/**	Set a user defined ID
			*	If the resource is not loaded the id will not be set.
			*	@param resource A filename to accociate the id with.
			*	@param id A user defined identifier that the resource handler does not touch.
			*/
			void			SetResourceId(const wchar_t filename[], unsigned int id);
			
			/**	Get a resource type given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the resource type of the handle
			*/
			ResourceType	GetResourceType(const HRESOURCE& resource);

			/**	Get a resource type given a filename
			*	If the resource is not loaded the id will not be set.
			*	@param resource The filename to check
			*	@return Returns the resource type of the handle
			*/
			ResourceType	GetResourceType (const wchar_t filename[]);
			
			/**	Get a resource filename given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the accociated filename
			*/
			const wchar_t*	GetResourceFilename(const HRESOURCE& resource);

			/**	Get a resource handle given a filename
			*	If the resource is not loaded function returns 0.
			*	@param resource The filename to check
			*	@return Returns the accociated handle
			*/ 
			HRESOURCE		GetResourceHandle(const wchar_t filename[]);
			
			/**	Get a user defined ID accociated with a handle
			*	@param resource The handle to check
			*	@return Returns the accociated ID
			*/
			int				GetResourceId(const HRESOURCE& resource);

			/**	Get a user defined ID accociated with a filename
			*	@param resource The filename to check
			*	@return Returns the accociated ID
			*/
			int				GetResourceId(const wchar_t filename[]);

			int				GetResourceCount(const wchar_t filename[]);

			int				GetResourceCount(const HRESOURCE& resource);

			int				GetResourceSize(const wchar_t filename[]);

			int				GetResourceSize(const HRESOURCE& resource);
		
		private:
			ResourceManager(const ResourceManager& obj);
			const ResourceManager& operator=(const ResourceManager&);
			std::map<std::wstring, ResourceData*> resources;

		};	
	}
}

#endif // !MISC_RESOURCELOADER_H
