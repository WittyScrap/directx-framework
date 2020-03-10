#include "Mesh.h"
#include "DirectXFramework.h"

Mesh::Mesh()
{ }

Mesh::Mesh(const Mesh& copy)
{
	_vertices = copy._vertices;
	_normals = copy._normals;
	_triangles = copy._triangles;
}

Mesh::Mesh(const vector<Vector3> vertices, const vector<Vector3> normals, const vector<int> triangles)
{
	_vertices = vertices;
	_normals = normals;
	_triangles = triangles;
}

Mesh::~Mesh()
{
	_vertices.clear();
	_normals.clear();
	_triangles.clear();
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
	int lastIndex = _vertices.size();
	
	_vertices.push_back(a);
	_vertices.push_back(b);
	_vertices.push_back(c);

	_triangles.push_back(++lastIndex);
	_triangles.push_back(++lastIndex);
	_triangles.push_back(++lastIndex);
}

void Mesh::AddVertex(const Vector3& vertex)
{
	_vertices.push_back(vertex);
}

void Mesh::AddNormal(const Vector3& normal)
{
	_normals.push_back(normal);
}

void Mesh::AddIndex(const int& index)
{
	_triangles.push_back(index);
}

void Mesh::SetVertices(const vector<Vector3>& vertices)
{
	_vertices = vertices;
}

void Mesh::SetNormals(const vector<Vector3>& normals)
{
	_normals = normals;
}

void Mesh::SetIndices(const vector<int>& indices)
{
	_triangles = indices;
}

void Mesh::ClearVertices()
{
	_vertices.clear();
}

void Mesh::ClearNormals()
{
	_normals.clear();
}

void Mesh::ClearIndices()
{
	_triangles.clear();
}

void Mesh::Clear()
{
	ClearVertices();
	ClearNormals();
	ClearIndices();
}

void Mesh::Apply()
{
	/**            Build VERTEX buffer            **/

	Vertex* vertexBuffer = new Vertex[_vertices.size() + 1]; // VS code analysis gets anxiety
															 // when buffers are the same length

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		vertexBuffer[i] =
		{
			_vertices[i].ToDX(), XMFLOAT4(Colors::White)
		};
	}

	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * _vertices.size();
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = vertexBuffer;

	const auto& device = GetFramework()->GetDevice();

	ThrowIfFailed(
		device->CreateBuffer(
			&vertexBufferDescriptor,
			&vertexInitialisationData,
			_vertexBuffer.GetAddressOf()
		)
	);

	delete[] vertexBuffer;

	/**          Build TRIANGLE buffer           **/

	UINT* indicesBuffer = new UINT[_triangles.size() + 1];

	for (size_t i = 0; i < _triangles.size(); ++i)
	{
		indicesBuffer[i] = _triangles[i];
	}

	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * _triangles.size();
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &indicesBuffer;

	ThrowIfFailed(
		device->CreateBuffer(
			&indexBufferDescriptor,
			&indexInitialisationData,
			_indexBuffer.GetAddressOf()
		)
	);

	delete[] indicesBuffer;
}

DirectXFramework* Mesh::GetFramework()
{
	return DirectXFramework::GetDXFramework();
}
