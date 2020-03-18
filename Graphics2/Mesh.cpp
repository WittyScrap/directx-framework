#include "Mesh.h"
#include "DirectXFramework.h"

#define DELETE_BUF(buff) { if (buff) { delete buff; } buff = nullptr; }
#define DELETE_ARR(buff) { if (buff) { delete[] buff; } buff = nullptr; }

Mesh::Mesh() : _vertices(), _normals(), _triangles()
{ }

Mesh::Mesh(const Mesh& copy) : Mesh()
{
	_vertices = copy._vertices;
	_normals = copy._normals;
	_triangles = copy._triangles;
}

Mesh::Mesh(const vector<Vector3> vertices, const vector<Vector3> normals, const vector<int> triangles) : Mesh()
{
	_vertices = vertices;
	_normals = normals;
	_triangles = triangles;
}

Mesh::~Mesh()
{
	Clear();
}

const vector<Vector3>& Mesh::GetVertices() const
{
	return _vertices;
}

const vector<Vector3>& Mesh::GetNormals() const
{
	return _normals;
}

const vector<int>& Mesh::GetTriangles() const
{
	return _triangles;
}

void Mesh::AddTriangle(const Vector3& a, const Vector3& b, const Vector3& c)
{
	size_t lastIndex = _vertices.size();
	
	_vertices.push_back(a);
	_vertices.push_back(b);
	_vertices.push_back(c);

	_triangles.push_back((int)(++lastIndex));
	_triangles.push_back((int)(++lastIndex));
	_triangles.push_back((int)(++lastIndex));

	_ready = false;
}

void Mesh::AddVertex(const Vector3& vertex)
{
	_vertices.push_back(vertex);
	_ready = false;
}

void Mesh::AddNormal(const Vector3& normal)
{
	_normals.push_back(normal);
	_ready = false;
}

void Mesh::AddIndex(const int& index)
{
	_triangles.push_back(index);
	_ready = false;
}

void Mesh::SetVertices(const vector<Vector3>& vertices)
{
	_vertices = vertices;
	_ready = false;
}

void Mesh::SetNormals(const vector<Vector3>& normals)
{
	_normals = normals;
	_ready = false;
}

void Mesh::SetIndices(const vector<int>& indices)
{
	_triangles = indices;
	_ready = false;
}

void Mesh::ClearVertices()
{
	_vertices.clear();
	_ready = false;
}

void Mesh::ClearNormals()
{
	_normals.clear();
	_ready = false;
}

void Mesh::ClearIndices()
{
	_triangles.clear();
	_ready = false;
}

void Mesh::ClearBuffers()
{
	_vertexBuffer.Reset();
	_indexBuffer.Reset();
	_constantBuffer.Reset();
	
	DELETE_ARR(_rawVertices);
	DELETE_ARR(_rawIndices);

	_ready = false;
}

void Mesh::Clear()
{
	ClearVertices();
	ClearNormals();
	ClearIndices();
	ClearBuffers();
}

void Mesh::Apply()
{
	/**            Build VERTEX buffer            **/

	DELETE_ARR(_rawVertices);

	_rawVertices = new Vertex[_vertices.size() + 1]; // VS code analysis gets anxiety
															 // when buffers are the same length

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		_rawVertices[i] =
		{
			_vertices[i].ToDX(), XMFLOAT4(Colors::White)
		};
	}

	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = (UINT)(sizeof(Vertex) * _vertices.size());
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = _rawVertices;

	const auto& device = GetFramework()->GetDevice();

	ThrowIfFailed(
		device->CreateBuffer(
			&vertexBufferDescriptor,
			&vertexInitialisationData,
			_vertexBuffer.GetAddressOf()
		)
	);

	/**          Build TRIANGLE buffer           **/

	DELETE_ARR(_rawIndices);

	_rawIndices = new UINT[_triangles.size() + 1];

	for (size_t i = 0; i < _triangles.size(); ++i)
	{
		_rawIndices[i] = _triangles[i];
	}

	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = (UINT)(sizeof(UINT) * _triangles.size());
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &_rawIndices;

	ThrowIfFailed(
		device->CreateBuffer(
			&indexBufferDescriptor,
			&indexInitialisationData,
			_indexBuffer.GetAddressOf()
		)
	);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(
		device->CreateBuffer(
			&bufferDesc,
			NULL,
			_constantBuffer.GetAddressOf()
		)
	);

	_ready = true;
}

ID3D11Buffer* const * Mesh::GetVertexBufferPointer() const
{
	return _vertexBuffer.GetAddressOf();
}

ID3D11Buffer* const * Mesh::GetIndexBufferPointer() const
{
	return _indexBuffer.GetAddressOf();
}

ID3D11Buffer* const * Mesh::GetConstantBufferPointer() const
{
	return _constantBuffer.GetAddressOf();
}

ID3D11Buffer* const Mesh::GetVertexBuffer() const
{
	return _vertexBuffer.Get();
}

ID3D11Buffer* const Mesh::GetIndexBuffer() const
{
	return _indexBuffer.Get();
}

ID3D11Buffer* const Mesh::GetConstantBuffer() const
{
	return _constantBuffer.Get();
}

const bool& Mesh::IsReady() const
{
	return _ready;
}

DirectXFramework* Mesh::GetFramework()
{
	return DirectXFramework::GetDXFramework();
}