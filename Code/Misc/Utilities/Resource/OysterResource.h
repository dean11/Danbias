/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_OYSTER_RESOURCE_H
#define MISC_OYSTER_RESOURCE_H


namespace Oyster 
{ 
	namespace Resource
	{
		struct CustomData;
		/** A Resource handle representing various resources */
		typedef void* OHRESOURCE;
		/** Typedef on a fuction required for custom unloading */
		typedef void(*CustomUnloadFunction)(void* loadedData);
		/** Typedef on a fuction required for custom loading */
		typedef void(*CustomLoadFunction)(const wchar_t filename[], CustomData& outData);

		/** An enum class representing all avalible resources that is supported. */
		enum ResourceType
		{
			//Byte
			ResourceType_Byte_Raw,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_ANSI,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF8,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UNICODE,				/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF16LE,				/**< Handle can be interpeted as char[] or char* */

			ResourceType_COUNT,						/**< Handle can be interpeted as ? */

			ResourceType_UNKNOWN = -1,				/**< Handle can be interpeted as void* */
			ResourceType_INVALID = -2,				/**< Invalid or non existing resource */
		};

		/** A struct to fill when doing a custom resource Load. */
		struct CustomData
		{
			void* loadedData;							//<! The loaded resource interpeted as a void*.
			CustomUnloadFunction resourceUnloadFnc;		//<! The function that will be used to free the resource when needed.
		};
		
		/** A resource handler interface to interact with when loading resources. 
		*	The resource handler uses the filename to make resources unuiqe.
		*/
		class OysterResource
		{
		public:
			/**
			*	Load a resource given a type.
			*	@param filename The path to the resource.
			*	@param type The resource type to load.
			*	@param force If set to true, the resource will be reloaded if it already exists. If it does not, nothing happens.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				LoadResource(const wchar_t filename[], ResourceType type, int customId = -1, bool force = false);

			/**
			*	Load a resource with a custom loading function
			*	@param filename The path to the resource.
			*	@param loadFnc If set, this gives you the right to do custom resource loading if your recource type is not supported.
			*	@param customId A custom ID that can be used.
			*	@param force If set to true, the resource will be reloaded even if exists.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				LoadResource(const wchar_t filename[], CustomLoadFunction loadFnc = 0, int customId = -1, bool force = false);

			/**
			*	Reload a resource
			*	@param filename The path to the resource.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				ReloadResource(const wchar_t filename[]);

			/**
			*	Reload a resource
			*	@param filename The path to the resource.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				ReloadResource(OHRESOURCE resource);

			/**
			*	Releases all resources loaded by the resource handler.
			*	@return Nothing
			*/
			static void						Clean();

			/**
			*	Release a reference to the resource handle
			*	@param resource The handle to release.
			*	@return Nothing
			*/
			static void						ReleaseResource(const OHRESOURCE& resource);

			/**
			*	Release a reference to the resource handle
			*	@param resource The resource filename to release reference.
			*	@return Nothing
			*/
			static void						ReleaseResource(const wchar_t filename[]);

			/**	Set a user defined ID
			*	@param resource A handle to accociate the id with.
			*	@param id A user defined identifier that the resource handler does not touch.
			*/
			static void						SetResourceId(const OHRESOURCE& resource, unsigned int id);

			/**	Set a user defined ID
			*	If the resource is not loaded the id will not be set.
			*	@param resource A filename to accociate the id with.
			*	@param id A user defined identifier that the resource handler does not touch.
			*/
			static void						SetResourceId(const wchar_t filename[], unsigned int id);
			
			/**	Get a resource type given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the resource type of the handle
			*/
			static ResourceType				GetResourceType(const OHRESOURCE& resource);

			/**	Get a resource type given a filename
			*	If the resource is not loaded the id will not be set.
			*	@param resource The filename to check
			*	@return Returns the resource type of the handle
			*/
			static ResourceType				GetResourceType (const wchar_t filename[]);
			
			/**	Get a resource filename given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the accociated filename
			*/
			static const wchar_t*			GetResourceFilename(const OHRESOURCE& resource);

			/**	Get a resource handle given a filename
			*	If the resource is not loaded function returns 0.
			*	@param resource The filename to check
			*	@return Returns the accociated handle
			*/
			static OHRESOURCE				GetResourceHandle(const wchar_t filename[]);
			
			/**	Get a user defined ID accociated with a handle
			*	@param resource The handle to check
			*	@return Returns the accociated ID
			*/
			static int						GetResourceId(const OHRESOURCE& resource);

			/**	Get a user defined ID accociated with a filename
			*	@param resource The filename to check
			*	@return Returns the accociated ID
			*/
			static int						GetResourceId(const wchar_t filename[]);


			static int						GetResourceSize(const OHRESOURCE& resource);


		
		};	
	}
}

#endif
