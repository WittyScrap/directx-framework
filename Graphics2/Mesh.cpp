#include "Mesh.h"
#include "Shader.h"

void Mesh::AddVertex(const Vertex& v)
{
	_vertices.push_back(v);
}

void Mesh::AddVertices(const vector<Vertex>& vertices)
{
	_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
}

void Mesh::AddIndex(const UINT& i)
{
	_indices.push_back(i);
}

void Mesh::AddIndices(const vector<UINT>& indices)
{
	_indices.insert(_indices.end(), indices.begin(), indices.end());
}

void Mesh::SetVertices(const vector<Vertex>& vertices)
{
	_vertices = vertices;
}

void Mesh::SetIndices(const vector<UINT>& indices)
{
	_indices = indices;
}

const vector<Vertex>& Mesh::GetVertices() const
{
	return _vertices;
}

const vector<UINT>& Mesh::GetIndices() const
{
	return _indices;
}

void Mesh::Apply()
{
	Vertex* vertices = _vertices.data();
	UINT* indices = _indices.data();

	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * (UINT)_vertices.size();
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
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * (UINT)_indices.size();
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

	b_isApplied = true;
}

void Mesh::Render() const
{
	if (b_isApplied)
	{
		// Now render the mesh
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// Set up buffers and render mesh data
		GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
	}
}

ComPtr<ID3D11Device> Mesh::GetDevice()
{
	return DirectXFramework::GetDXFramework()->GetDevice();
}

ComPtr<ID3D11DeviceContext> Mesh::GetDeviceContext()
{
	return DirectXFramework::GetDXFramework()->GetDeviceContext();
}
