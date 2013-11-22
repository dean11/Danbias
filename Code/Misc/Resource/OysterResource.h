/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef MISC_OYSTER_RESOURCE_H
#define MISC_OYSTER_RESOURCE_H


namespace Oyster 
{ 
	namespace Resource
	{
		/** A Resource handle representing various resources */
		typedef unsigned long OHRESOURCE;

		/** An enum class representing all avalible resources that is supported. */
		enum ResourceType
		{
			//Texture
			ResourceType_Texture_PNG,				/**< Handle can be interpeted as ID3D11ShaderResourceView */
			ResourceType_Texture_DDS,				/**< Handle can be interpeted as ID3D11ShaderResourceView */
			ResourceType_Texture_JPG,				/**< Handle can be interpeted as ID3D11ShaderResourceView */
		
			//Mesh
			ResourceType_Mesh_AllInOne,				/**< Handle can be interpeted as ? */
			ResourceType_Mesh_VertexData,			/**< Handle can be interpeted as ? */
			ResourceType_Mesh_AnimationData,		/**< Handle can be interpeted as ? */
		
			//Audio
			ResourceType_Audio_mp3,					/**< Handle can be interpeted as ? */
		
			//Shaders
			ResourceType_Shader_Vertex,				/**< Handle can be interpeted as ? */
			ResourceType_Shader_VertexBLOB,			/**< Handle can be interpeted as ID3Blob */
			ResourceType_Shader_Hull,				/**< Handle can be interpeted as ? */
			ResourceType_Shader_Domain,				/**< Handle can be interpeted as ? */
			ResourceType_Shader_Geometry,			/**< Handle can be interpeted as ? */
			ResourceType_Shader_Pixel,				/**< Handle can be interpeted as ? */
			ResourceType_Shader_Compute,			/**< Handle can be interpeted as ? */
		
			//Byte
			ResourceType_Byte_Raw,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_ANSI,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF8,					/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UNICODE,				/**< Handle can be interpeted as char[] or char* */
			ResourceType_Byte_UTF16LE,				/**< Handle can be interpeted as char[] or char* */

			ResourceType_COUNT,						/**< Handle can be interpeted as ? */
			ResourceType_UNKNOWN = -1				/**< Handle can be interpeted as char[] or char* */
		};
		
		/** A resource handler interface to interact with when loading resources. 
		*	The resource handler uses the filename to make resources unuiqe.
		*/
		class IResourceHandler
		{
		public:

			/**
			*	Release the resource handle, wich in turn releases every resource loaded with this instance.
			*	@return Nothing.
			*/
			static void						Release();

			/**
			*	Load a resource given a type.
			*	@param filename The path to the resource.
			*	@param type The resource type to load.
			*	@param force If set to true, the resource will be reloaded if it already exists. If it does not, nothing happens.
			*	@return If function suceeds, a handle to the resource will be returned. If failed 0 is returned.
			*/
			static OHRESOURCE				LoadResource(const wchar_t filename[], ResourceType type, bool force = false);

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
