#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include <map>
#include <assimp\importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "DirectXCore.h"

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

/** Mesh resource mapper */
typedef map<wstring, MeshResourceStruct>		MeshResourceMap;

/** Material resource mapper  */
typedef map<wstring, MaterialResourceStruct>	MaterialResourceMap;

/** Shader resource mapper */
typedef map<wstring, ShaderResourceStruct>		ShaderResourceMap;

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

	void										ReleaseMesh(wstring modelName);
	void										ReleaseMaterial(wstring materialName);
	void										ReleaseShader(wstring shaderName);

	shared_ptr<Mesh>							LoadModelFromFile(wstring modelName);

private:
	MeshResourceMap								_meshResources;
	MaterialResourceMap							_materialResources;
	ShaderResourceMap							_shaderResources;

	ComPtr<ID3D11Device>						_device;
	ComPtr<ID3D11DeviceContext>					_deviceContext;

	ComPtr<ID3D11ShaderResourceView>			_defaultTexture;
};

