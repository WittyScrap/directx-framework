#include "MeshNode.h"
#include "DirectXFramework.h"


/**
 * Carries the transformation matrix through.
 *
 */
void MeshNode::Update(FXMMATRIX& currentWorldTransformation)
{
	SceneNode::Update(currentWorldTransformation);
}

/**
 * Displays the mesh contained in this mesh node.
 *
 */
void MeshNode::Render()
{
    InternalRender(_mesh, _material);

    for (size_t it = 0; it < _mesh->GetSubmeshCount(); ++it)
    {
        shared_ptr<Mesh> mesh = _mesh->GetSubmesh(it);
        shared_ptr<Material> mat = it >= _materials.size() ? nullptr : _materials[it];

        InternalRender(mesh, mat);
    }
}

/**
 * Destroys this mesh node.
 *
 */
void MeshNode::Shutdown()
{
}

/**
 * Sets the material associated with this mesh node.
 *
 */
void MeshNode::SetMaterial(const shared_ptr<Material>& material)
{
    _material = material;
}

/**
 * Sets the mesh to be rendered by this node.
 *
 */
void MeshNode::SetMesh(const shared_ptr<Mesh>& mesh)
{
    _mesh = mesh;
}

/**
 * Returns the mesh associated with this node.
 *
 */
const shared_ptr<Mesh> MeshNode::GetMesh() const
{
    return _mesh;
}

/**
 * Returns the material associated with this node.
 *
 */
const shared_ptr<Material> MeshNode::GetMaterial() const
{
    return _material;
}

void MeshNode::SetMaterial(const size_t location, const shared_ptr<Material>& material)
{
    if (location < 0)
    {
        return;
    }

    size_t diff = location - (_materials.size() - 1);

    while (diff--)
    {
        _materials.push_back(nullptr);
    }

    _materials[location] = material;
}

const shared_ptr<Material> MeshNode::GetMaterial(const size_t location) const
{
    if (location >= 0 && location < _materials.size())
    {
        return _materials[location];
    }

    return nullptr;
}

/**
 * Returns the Device object currently in use by the framework.
 *
 */
ComPtr<ID3D11Device> MeshNode::GetDevice()
{
	return DirectXFramework::GetDXFramework()->GetDevice();
}

/**
 * Returns the Device Context currently in use by the framework.
 *
 */
ComPtr<ID3D11DeviceContext> MeshNode::GetDeviceContext()
{
	return DirectXFramework::GetDXFramework()->GetDeviceContext();
}

/**
 * Renders a specific mesh using a specific material.
 *
 */
void MeshNode::InternalRender(shared_ptr<Mesh> mesh, shared_ptr<Material> material)
{
    if (mesh)
    {
        if (material && material->Activate()) 
        {
            ; // NOP
        }
        else
        {
            material = RESOURCES->GetDefaultMaterial(); material->Activate();
        }

        DirectXFramework* framework = DirectXFramework::GetDXFramework();

        // Calculate the world x view x projection transformation
        XMMATRIX completeTransformation = XMLoadFloat4x4(&_combinedWorldTransformation) * framework->GetViewTransformation() * framework->GetProjectionTransformation();

        // Draw the first cube
        CBUFFER cBuffer;
        cBuffer.CompleteTransformation = completeTransformation;
        cBuffer.WorldTransformation = XMLoadFloat4x4(&_combinedWorldTransformation);
        cBuffer.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
        cBuffer.LightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f));
        cBuffer.LightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        material->Update(&cBuffer);
        mesh->Render();
    }
}

/**
 * Maps the mesh and all of its submeshes, along with their suggested materials,
 * to this mesh node.
 *
 */
void MeshNode::Build(shared_ptr<Mesh> mesh)
{
    _mesh = mesh;
    SetMaterial(mesh->GetReferenceMaterial());

    for (size_t it = 0; it < mesh->GetSubmeshCount(); ++it)
    {
        SetMaterial(it, mesh->GetSubmesh(it)->GetReferenceMaterial());
    }
}
