#pragma once
#include <map>
#include <assimp\importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "DirectXCore.h"
#include "HelperFunctions.h"

class Mesh;
class Material;
class Shader;
class Texture;

/**
 * Mesh resource tracker structure.
 *
 */
struct MeshResourceStruct
{
	unsigned int			ReferenceCount;
	shared_ptr<Mesh>		MeshPointer;
};

/**
 * Material resource tracker structure.
 *
 */
struct MaterialResourceStruct
{
	unsigned int			ReferenceCount;
	shared_ptr<Material>	MaterialPointer;
};

/**
 * Shader resource tracker structure.
 *
 */
struct ShaderResourceStruct
{
	unsigned int			ReferenceCount;
	shared_ptr<Shader>		ShaderPointer;
};

/**
 * Texture resource tracker structure.
 *
 */
struct TextureResourceStruct
{
	unsigned int			ReferenceCount;
	shared_ptr<Texture>		TexturePointer;
};

/** Mesh resource mapper */
typedef map<wstring, MeshResourceStruct>		MeshResourceMap;

/** Material resource mapper  */
typedef map<wstring, MaterialResourceStruct>	MaterialResourceMap;

/** Shader resource mapper */
typedef map<wstring, ShaderResourceStruct>		ShaderResourceMap;

/** Texture resource mapper */
typedef map<wstring, TextureResourceStruct>		TextureResourceMap;

/**
 * Manages system resources for meshes, materials, and shaders.
 *
 */
class ResourceManager
{
public:
			ResourceManager();
		   ~ResourceManager();
	
			shared_ptr<Shader>							GetShader(wstring shaderName);
			shared_ptr<Mesh>							GetMesh(wstring modelName);
			shared_ptr<Material>						GetMaterial(wstring materialName);
			shared_ptr<Texture>							GetTexture(wstring textureName);

			void										ReleaseMesh(wstring modelName);
			void										ReleaseMaterial(wstring materialName);
			void										ReleaseShader(wstring shaderName);

			shared_ptr<Mesh>							LoadModelFromFile(wstring modelName);

	inline  shared_ptr<Texture>							GetDefaultTexture() const { return _defaultTexture; }
		    shared_ptr<Material>						GetDefaultMaterial();
			shared_ptr<Shader>							GetDefaultShader();

private:
	MeshResourceMap										_meshResources;
	MaterialResourceMap									_materialResources;
	ShaderResourceMap									_shaderResources;
	TextureResourceMap									_textureResources;

	ComPtr<ID3D11Device>								_device;
	ComPtr<ID3D11DeviceContext>							_deviceContext;

	shared_ptr<Texture>									_defaultTexture;
	shared_ptr<Material>								_defaultMaterial;
	shared_ptr<Shader>									_defaultShader;
};

