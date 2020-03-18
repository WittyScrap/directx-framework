#include "MeshNode.h"
#include "WICTextureLoader.h"
#include "DirectXFramework.h"

/**
 * Initializes this mesh node.
 *
 */
bool MeshNode::Initialise()
{
	BuildBuffers();
	BuildShader();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildTexture();

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
	DirectXFramework* framework = DirectXFramework::GetDXFramework();

	// Calculate the world x view x projection transformation
	XMMATRIX completeTransformation = XMLoadFloat4x4(&_combinedWorldTransformation) * framework->GetViewTransformation() * framework->GetProjectionTransformation();

	// Draw the first cube
	CBUFFER cBuffer;
	cBuffer.CompleteTransformation = completeTransformation;
	cBuffer.WorldTransformation = XMLoadFloat4x4(&_combinedWorldTransformation);
	cBuffer.AmbientColour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	cBuffer.LightVector = XMVector4Normalize(XMVectorSet(0.0f, 01.0f, 1.0f, 0.0f));
	cBuffer.LightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Update the constant buffer 
	GetDeviceContext()->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	GetDeviceContext()->UpdateSubresource(_constantBuffer.Get(), 0, 0, &cBuffer, 0, 0);

	// Set the texture to be used by the pixel shader
	GetDeviceContext()->PSSetShaderResources(0, 1, _texture.GetAddressOf());

	// Now render the cube
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set up buffers and render mesh data
	GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->DrawIndexed(_indicesCount, 0, 0);
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
    _shaderName = fileName;
}

/**
 * Configures the texture to feed the shader to display in
 * this mesh.
 *
 */
void MeshNode::SetTexture(const wstring& textureName)
{
    _textureName = textureName;
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
 * Builds the shader and compiles it.
 *
 */
void MeshNode::BuildShader()
{
	DWORD shaderCompileFlags = 0;

#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile
	(
		_shaderName.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderCompileFlags, 0,
		_vertBytes.GetAddressOf(),
		compilationMessages.GetAddressOf()
	);

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}

	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(GetDevice()->CreateVertexShader(_vertBytes->GetBufferPointer(), _vertBytes->GetBufferSize(), NULL, _vert.GetAddressOf()));
	GetDeviceContext()->VSSetShader(_vert.Get(), 0, 0);

	// Compile pixel shader
	hr = D3DCompileFromFile
	(
		_shaderName.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderCompileFlags, 0,
		_fragBytes.GetAddressOf(),
		compilationMessages.GetAddressOf()
	);

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}

	ThrowIfFailed(hr);
	ThrowIfFailed(GetDevice()->CreatePixelShader(_fragBytes->GetBufferPointer(), _fragBytes->GetBufferSize(), NULL, _frag.GetAddressOf()));
	GetDeviceContext()->PSSetShader(_frag.Get(), 0, 0);
}

/**
 * Configures the vertex layout to indicate the
 * format of the vertices that will be given to the
 * shader program.
 *
 */
void MeshNode::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertBytes->GetBufferPointer(), _vertBytes->GetBufferSize(), _layout.GetAddressOf()));
	GetDeviceContext()->IASetInputLayout(_layout.Get());
}

/**
 * Builds a constant buffer to exchange data with the
 * GPU.
 *
 */
void MeshNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(GetDevice()->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

/**
 * Loads the texture from the stored file and passes it onto
 * the shader program.
 *
 */
void MeshNode::BuildTexture()
{
	// Note that in order to use CreateWICTextureFromFile, we 
	// need to ensure we make a call to CoInitializeEx in our 
	// Initialise method (and make the corresponding call to 
	// CoUninitialize in the Shutdown method).  Otherwise, 
	// the following call will throw an exception
	ThrowIfFailed(
		CreateWICTextureFromFile(
			GetDevice().Get(),
			GetDeviceContext().Get(),
			_textureName.c_str(),
			nullptr,
			_texture.GetAddressOf()
		)
	);
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
