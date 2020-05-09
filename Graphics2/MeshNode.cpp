#include "MeshNode.h"
#include "DirectXFramework.h"
#include "CameraNode.h"

/**
 * Displays the mesh contained in this mesh node.
 *
 */
void MeshNode::Render()
{
    OnPreRender();

    if (_mesh)
    {
        InternalRender(_mesh, _material);

        for (size_t it = 0; it < _mesh->GetSubmeshCount(); ++it)
        {
            shared_ptr<Mesh> mesh = _mesh->GetSubmesh(it);
            shared_ptr<Material> mat = it >= _materials.size() ? nullptr : _materials[it];

            InternalRender(mesh, mat);
        }
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
        if (!material || !material->Activate()) 
        {
            material = RESOURCES->GetDefaultMaterial(); material->Activate();
        }

        if (material->CheckPass())
        {
            // Calculate the world x view x projection transformation
            XMMATRIX completeTransformation = XMLoadFloat4x4(&_combinedWorldTransformation) * MAIN_CAMERA->GetViewTransformation() * MAIN_CAMERA->GetProjectionTransformation();

            // Create mesh data object and populate with mesh object specific data
            MeshObjectData data;
            data.completeTransformation = completeTransformation;
            data.worldTransformation = XMLoadFloat4x4(&_combinedWorldTransformation);

            material->UpdateConstantBuffers(data);
            mesh->Render();
        }
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
