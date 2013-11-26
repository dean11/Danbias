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
		typedef unsigned long OHRESOURCE;
		typedef void(*CustomUnloadFunction)(void* loadedData);
		typedef const CustomData&(*CustomLoadFunction)(const wchar_t filename[]);

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
		};

		/** A struct to return when doing a custom resource Load 
		*	By loading this way you are handing over the ownership to the resource loaded. 
		*/
		struct CustomData
		{
			void* loadedData;							///<! The loaded resource interpeted as a void*.
			CustomUnloadFunction resourceUnloadFnc;		///<! The function that will be used to free the resource when needed.
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
			static OHRESOURCE				LoadResource(const wchar_t filename[], ResourceType type);

			/**
			*	Load a resource with a custom loading function
			*	@param filename The path to the resource.
			*	@param force If set to true, the resource will be reloaded even if exists.
			*	@param loadFnc If set, this gives you the right to do custom resource loading if your recource type is not supported.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				LoadResource(const wchar_t filename[], CustomLoadFunction loadFnc = 0, unsigned int CustomId = 0);

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

			/**	Set a user defined ID
			*	@param resource A handle to accociate the id with.
			*	@param id A user defined identifier that the resource handler does not touch.
			*/
			static void						SetResourceId(const OHRESOURCE& resource, unsigned int id);
			
			/**	Get a resource type given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the resource type of the handle
			*/
			static ResourceType				GetResourceType(const OHRESOURCE& resource);
			
			/**	Get a resource filename given a OHRESOURCE handle
			*	@param resource The handle to check
			*	@return Returns the accociated filename
			*/
			static const wchar_t*			GetResourceFilename(const OHRESOURCE& resource);
			
			/**	Get a user defined ID accociated with a handle
			*	@param resource The handle to check
			*	@return Returns the accociated ID
			*/
			static unsigned int				GetResourceId(const OHRESOURCE& resource);
		
		};	
	}
}

#endif
