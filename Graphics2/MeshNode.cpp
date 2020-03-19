#include "MeshNode.h"
#include "DirectXFramework.h"

/**
 * Initializes this mesh node.
 *
 */
bool MeshNode::Initialise()
{
	this->BuildBuffers();

	return true;
}

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
	if (_material && _material->Activate())
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

		// Now render the mesh
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// Set up buffers and render mesh data
		GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetDeviceContext()->DrawIndexed(_indicesCount, 0, 0);
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
 * Configures the shader from which to take the source code
 * for this mesh.
 *
 */
void MeshNode::SetShader(const wstring& fileName)
{
	if (!this->_material)
	{
		this->_material = make_shared<Material>(fileName);
	}
	else
	{
		this->_material->SetShader(fileName);
	}
}

/**
 * Configures the texture to feed the shader to display in
 * this mesh.
 *
 */
void MeshNode::SetTexture(const wstring& textureName)
{
    this->_material->SetTexture(textureName);
}

/**
 * Builds the mesh buffers.
 *
 */
void MeshNode::BuildBuffers()
{
    vector<Vertex> verticesVector = MeshVertices();
    vector<UINT> indicesVector = MeshIndices();

    Vertex* vertices = verticesVector.data();
    UINT* indices = indicesVector.data();

	_indicesCount = (UINT)indicesVector.size();

    // Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * (UINT)verticesVector.size();
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = vertices;

	// and create the vertex buffer
	ThrowIfFailed(GetDevice()->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	// Setup the structure that specifies how big the index 
	// buffer should be
	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * (UINT)indicesVector.size();
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the indices from
	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = indices;

	// and create the index buffer
	ThrowIfFailed(GetDevice()->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
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
