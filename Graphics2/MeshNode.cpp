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
	if (_mesh && _material && _material->Activate())
	{
		DirectXFramework* framework = DirectXFramework::GetDXFramework();

		// Calculate the world x view x projection transformation
		XMMATRIX completeTransformation = XMLoadFloat4x4(&_combinedWorldTransformation) * framework->GetViewTransformation() * framework->GetProjectionTransformation();

		// Draw the first cube
		CBUFFER cBuffer;
		cBuffer.CompleteTransformation = completeTransformation;
		cBuffer.WorldTransformation = XMLoadFloat4x4(&_combinedWorldTransformation);
		cBuffer.AmbientColour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		cBuffer.LightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f));
		cBuffer.LightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		_material->Update(&cBuffer);
        _mesh->Render();
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
