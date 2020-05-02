#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "ResourceManager.h"
#include "DirectXFramework.h"
#include <sstream>
#include "WICTextureLoader.h"
#include <locale>
#include <codecvt>

#pragma comment(lib, "Assimp/lib/assimp-vc140-mt.lib")

using namespace Assimp;

//-------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
{
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

    // Create a default texture for use where none is specified.  If white.png is not available, then
    // the default texture will be null, i.e. black.  This causes problems for materials that do not
	// provide a texture, unless we provide a totally different shader just for those cases.  That
	// might be more efficient, but is a lot of work at this stage for little gain.
	_defaultTexture = SceneGraph::Create<Texture>(L"white.png");
}

ResourceManager::~ResourceManager(void)
{
}

shared_ptr<Shader> ResourceManager::GetShader(wstring shaderName)
{
	ShaderResourceMap::iterator it = _shaderResources.find(shaderName);

	if (it != _shaderResources.end())
	{
		it->second.ReferenceCount++;
		return it->second.ShaderPointer;
	}
	else
	{
		shared_ptr<Shader> shader = make_shared<Shader>(shaderName);
		shader->CompileOnce();

		ShaderResourceStruct resourceStruct;
		resourceStruct.ReferenceCount = 1;
		resourceStruct.ShaderPointer = shader;
		_shaderResources[shaderName] = resourceStruct;

		return shader;
	}
}

shared_ptr<Mesh> ResourceManager::GetMesh(wstring modelName)
{
	// CHeck to see if the mesh has already been loaded
	MeshResourceMap::iterator it = _meshResources.find(modelName);

	if (it != _meshResources.end())
	{
		// Update reference count and return pointer to existing mesh
		it->second.ReferenceCount++;
		return it->second.MeshPointer;
	}
	else
	{
		// This is the first request for this model.  Load the mesh and
		// save a reference to it.
		shared_ptr<Mesh> mesh = LoadModelFromFile(modelName);

		if (mesh != nullptr)
		{
			MeshResourceStruct resourceStruct;
			resourceStruct.ReferenceCount = 1;
			resourceStruct.MeshPointer = mesh;

			_meshResources[modelName] = resourceStruct;
			return mesh;
		}
		else
		{
			return nullptr;
		}
	}
}

void ResourceManager::ReleaseMesh(wstring modelName)
{
	MeshResourceMap::iterator it = _meshResources.find(modelName);

	if (it != _meshResources.end())
	{
		it->second.ReferenceCount--;

		if (it->second.ReferenceCount == 0)
		{
			// If no other nodes are using this mesh, remove it frmo the map
			// (which will also release the resources).
			it->second.MeshPointer = nullptr;
			_meshResources.erase(modelName);
		}
	}
}

shared_ptr<Material> ResourceManager::GetMaterial(wstring materialName)
{
	// This works a bit different to the GetMesh method.  We can only find
	// materials we have previously created (usually when the mesh was loaded
	// from the file).
	MaterialResourceMap::iterator it = _materialResources.find(materialName);
	if (it != _materialResources.end())
	{
		it->second.ReferenceCount++;
		return it->second.MaterialPointer;
	}
	else
	{
		// Material not previously created.
		return nullptr;
	}
}

shared_ptr<Material> ResourceManager::GetDefaultMaterial()
{
	if (!_defaultMaterial)
	{
		_defaultMaterial = make_shared<Material>(GetDefaultShader());
		_defaultMaterial->SetTexture(0, GetDefaultTexture());
	}

	if (!_defaultMaterial->GetTexture(0))
	{
		_defaultMaterial->SetTexture(0, GetDefaultTexture());
	}

	return _defaultMaterial;
}

shared_ptr<Shader> ResourceManager::GetDefaultShader()
{
	if (!_defaultShader)
	{
		_defaultShader = make_shared<Shader>(L"Shaders/lambert.hlsl");
		_defaultShader->CompileOnce();
	}

	return _defaultShader;
}

void ResourceManager::ReleaseMaterial(wstring materialName)
{
	MaterialResourceMap::iterator it = _materialResources.find(materialName);

	if (it != _materialResources.end())
	{
		it->second.ReferenceCount--;

		if (it->second.ReferenceCount == 0)
		{
			it->second.MaterialPointer = nullptr;
			_meshResources.erase(materialName);
		}
	}
}

void ResourceManager::ReleaseShader(wstring shaderName)
{
	ShaderResourceMap::iterator it = _shaderResources.find(shaderName);

	if (it != _shaderResources.end())
	{
		it->second.ReferenceCount--;

		if (it->second.ReferenceCount == 0)
		{
			it->second.ShaderPointer = nullptr;
			_shaderResources.erase(shaderName);
		}
	}
}

shared_ptr<Mesh> ResourceManager::LoadModelFromFile(wstring modelName)
{
    wstring* materials = nullptr;
	Importer importer;

	unsigned int postProcessSteps = aiProcess_Triangulate |
		                            aiProcess_ConvertToLeftHanded;

	string modelNameUTF8 = ws2s(modelName);
	const aiScene* scene = importer.ReadFile(modelNameUTF8.c_str(), postProcessSteps);

	if (!scene)
	{
        // If failed to load, there is nothing to do
		return nullptr;
	}

    if (!scene->HasMeshes())
    {
        //If there are no meshes, then there is nothing to do.
		return nullptr;
    }

    if (scene->HasMaterials())
    {
        // We need to find the directory part of the model name since we will need to add it to any texture names. 
        // There is definately a more elegant and accurate way to do this using Windows API calls, but this is a quick
        // and dirty approach
        string::size_type slashIndex = modelNameUTF8.find_last_of("/");
        string directory;

        if (slashIndex == string::npos) 
        {
            directory = ".";
        }
        else if (slashIndex == 0) 
        {
            directory = "/";
        }
        else 
        {
            directory = modelNameUTF8.substr(0, slashIndex);
        }

        // Let's deal with the materials/textures first
        materials = new wstring[scene->mNumMaterials];

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			// Get the core material properties.  Ideally, we would be looking for more information
			// e.g. emissive colour, etc.  This is a task for later.
			aiMaterial* material = scene->mMaterials[i];

			aiColor3D defaultColour(0.0f, 0.0f, 0.0f);
			aiColor3D diffuseColour(0.0f, 0.0f, 0.0f);
			aiColor3D specularColour(0.0f, 0.0f, 0.0f);

			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColour) != aiReturn_SUCCESS)
			{
				diffuseColour = defaultColour;
			}

			if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColour) != aiReturn_SUCCESS)
			{
				specularColour = defaultColour;
			}

			float defaultShininess = 0.0f;
			float& shininess = defaultShininess;
			material->Get(AI_MATKEY_SHININESS, shininess);

			float defaultOpacity = 1.0f;
			float& opacity = defaultOpacity;
			material->Get(AI_MATKEY_OPACITY, opacity);

			bool defaultTwoSided = false;
			bool& twoSided = defaultTwoSided;
			material->Get(AI_MATKEY_TWOSIDED, twoSided);

			string fullTextureNamePath = "";

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString textureName;
				aiTextureOp blendOp;
				float blendFactor;

				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, &blendFactor, &blendOp, NULL) == AI_SUCCESS)
				{
					// Get full path to texture by prepending the same folder as included in the model name. This
					// does assume that textures are in the same folder as the model files
					fullTextureNamePath = directory + "/" + textureName.data;
				}
			}

			// Now create a unique name for the material based on the model name and loop count
			stringstream materialNameStream;
			materialNameStream << modelNameUTF8 << i;

			string materialName = materialNameStream.str();
			wstring materialNameWS = s2ws(materialName);

			shared_ptr<Material> materialObject = make_shared<Material>(materialNameWS, GetDefaultShader());
			materialObject->SetAlbedo({ diffuseColour.r, diffuseColour.g, diffuseColour.b, 1.0f });
			materialObject->SetSpecularColor({ specularColour.r, specularColour.g, specularColour.b, 1.0f });
			materialObject->SetShininess(shininess);
			materialObject->SetOpacity(opacity);

			if (!fullTextureNamePath.empty())
			{
				materialObject->SetTexture(0, make_shared<Texture>(s2ws(fullTextureNamePath)));
			}

			// Save new material
			_materialResources[materialNameWS] = { 1, materialObject };

            materials[i] = materialNameWS;
        }
    }

    // Now we have created all of the materials, build up the mesh

	shared_ptr<Mesh> resourceMesh = make_shared<Mesh>();

    for (unsigned int sm = 0; sm < scene->mNumMeshes; sm++)
    {
	    aiMesh * subMesh = scene->mMeshes[sm];
		shared_ptr<Mesh> resourceSubmesh = make_shared<Mesh>();

	    unsigned int numVertices = subMesh->mNumVertices;
	    bool hasNormals = subMesh->HasNormals();
	    bool hasTexCoords = subMesh->HasTextureCoords(0);

	    if (numVertices == 0 || !hasNormals)
	    {
		    return nullptr;
	    }

	    // Build up our vertex structure
	    aiVector3D * subMeshVertices = subMesh->mVertices;
	    aiVector3D * subMeshNormals = subMesh->mNormals;

        // We only handle one set of UV coordinates at the moment.  Again, handling multiple sets of UV
        // coordinates is a future enhancement.
	    aiVector3D * subMeshTexCoords = subMesh->mTextureCoords[0];

	    for (unsigned int i = 0; i < numVertices; i++)
	    {
			Vertex currentVertex;

			currentVertex.Position = XMFLOAT3(subMeshVertices->x, subMeshVertices->y, subMeshVertices->z);
			currentVertex.Normal = XMFLOAT3(subMeshNormals->x, subMeshNormals->y, subMeshNormals->z);

		    subMeshVertices++;
		    subMeshNormals++;

            if (!hasTexCoords)
            {
                // If the model does not have texture coordinates, set them to 0
				currentVertex.TextureCoordinate = XMFLOAT2(0.0f, 0.0f);
            }
            else
            {
                // Handle negative texture coordinates by wrapping them to positive.  This should
                // ideally be handled in the shader.  Note we are assuming that negative coordinates
                // here are no smaller than -1.0 - this may not be a valid assumption.
		        if (subMeshTexCoords->x < 0)
		        {
			        currentVertex.TextureCoordinate.x = subMeshTexCoords->x + 1.0f;
		        }
		        else
		        {
			        currentVertex.TextureCoordinate.x = subMeshTexCoords->x;
		        }

		        if (subMeshTexCoords->y < 0)
		        {
			        currentVertex.TextureCoordinate.y = subMeshTexCoords->y + 1.0f;
		        }
		        else
		        {
			        currentVertex.TextureCoordinate.y = subMeshTexCoords->y;
		        }

		        subMeshTexCoords++;
            }

			resourceSubmesh->AddVertex(currentVertex);
	    }

	    // Now extract the indices from the file
	    unsigned int numberOfFaces = subMesh->mNumFaces;
	    unsigned int numberOfIndices = numberOfFaces * 3;
	    aiFace * subMeshFaces = subMesh->mFaces;

	    if (subMeshFaces->mNumIndices != 3)
	    {
		    // We are not dealing with triangles, so we cannot handle it
			return nullptr;
	    }

	    for (unsigned int i = 0; i < numberOfFaces; i++)
	    {
			resourceSubmesh->AddIndex(subMeshFaces->mIndices[0]);
			resourceSubmesh->AddIndex(subMeshFaces->mIndices[1]);
			resourceSubmesh->AddIndex(subMeshFaces->mIndices[2]);

		    subMeshFaces++;
	    }

		// Do we have a material associated with this mesh?
		shared_ptr<Material> material = nullptr;

        if (scene->HasMaterials())
        {
            material = GetMaterial(materials[subMesh->mMaterialIndex]);
        }

		resourceSubmesh->SetReferenceMaterial(material);
		resourceMesh->AddSubmesh(resourceSubmesh);
    }

	resourceMesh->Apply();

	delete[] materials;
	return resourceMesh;
}
