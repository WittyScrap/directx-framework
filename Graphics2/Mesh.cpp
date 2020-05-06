#include "Mesh.h"
#include "Shader.h"

struct NormalCalculator
{
	Vector3 normal;
	int contributions;
};

void Mesh::AddVertex(Vertex v)
{
	_vertices.push_back(v);
}

void Mesh::AddVertices(const vector<Vertex>& vertices)
{
	_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
}

void Mesh::AddIndex(UINT i)
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
	if (_vertices.size() && _indices.size())
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

	for (shared_ptr<Mesh>& subMesh : _subMeshes)
	{
		subMesh->Apply();
	}
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
		GetDeviceContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)_mode);
		GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
	}

	/*for (const shared_ptr<Mesh>& subMesh : _subMeshes)
	{
		subMesh->Render();
	}*/
}

void Mesh::AddSubmesh(shared_ptr<Mesh>& subMesh)
{
	_subMeshes.push_back(subMesh);
}

void Mesh::RemoveSubmesh(shared_ptr<Mesh>& subMesh)
{
	vector<shared_ptr<Mesh>>::iterator it = find(_subMeshes.begin(), _subMeshes.end(), subMesh);

	if (it != _subMeshes.end())
	{
		_subMeshes.erase(it);
	}
}

void Mesh::RemoveSubmesh(const size_t& index)
{
	if (index >= 0 && index < _subMeshes.size())
	{
		_subMeshes.erase(_subMeshes.begin() + index);
	}
}

shared_ptr<Mesh> Mesh::GetSubmesh(const size_t& index) const
{
	if (index >= 0 && index < _subMeshes.size())
	{
		return _subMeshes[index];
	}

	return nullptr;
}

size_t Mesh::GetSubmeshCount() const
{
	return _subMeshes.size();
}

void Mesh::SetReferenceMaterial(const shared_ptr<Material>& material)
{
	_referencedMaterial = material;
}

const shared_ptr<Material>& Mesh::GetReferenceMaterial() const
{
	return _referencedMaterial;
}

void Mesh::RecalculateNormals()
{
	vector<NormalCalculator> normals;

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		normals.push_back({ {}, 0 });
	}

	for (int i0 = 0, i1 = 1, i2 = 2; i2 < _indices.size(); i0 += 3, i1 += 3, i2 += 3)
	{
		int index0 = _indices[i0];
		int index1 = _indices[i1];
		int index2 = _indices[i2];

		if (index0 >= _vertices.size() ||
			index1 >= _vertices.size() ||
			index2 >= _vertices.size())
		{
			ErrorDialog(L"Invalid Mesh", L"Invalid Mesh Data: A mesh index has exceeded the boundaries of the vertex array.", std::out_of_range("Invalid mesh data: index out of range"));
		}

		Vertex& a = _vertices[index0];
		Vertex& b = _vertices[index1];
		Vertex& c = _vertices[index2];

		Vector3 aTob(Vector3(b.Position) - a.Position);
		Vector3 aToc(Vector3(c.Position) - a.Position);

		aTob.Normalize();
		aToc.Normalize();

		Vector3 normal = Vector3::Cross(aTob, aToc).Normalized();

		NormalCalculator& n0 = normals[index0];
		NormalCalculator& n1 = normals[index1];
		NormalCalculator& n2 = normals[index2];

		n0.normal += normal;
		n0.contributions++;

		n1.normal += normal;
		n1.contributions++;

		n2.normal += normal;
		n2.contributions++;
	}

	for (size_t i = 0; i < normals.size(); ++i)
	{
		normals[i].normal /= static_cast<float>(normals[i].contributions);
	}

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		_vertices[i].Normal = normals[i].normal.ToDX3();
	}

	b_isApplied = false;
}

void Mesh::Invert()
{
	for (size_t it0 = 0, it1 = 2; it1 < _indices.size(); it0 = it1 + 1, it1 = it0 + 2)
	{
		UINT i0 = _indices[it0];
		_indices[it0] = _indices[it1];
		_indices[it1] = i0;
	}
	
	b_isApplied = false;
}

ComPtr<ID3D11Device> Mesh::GetDevice()
{
	return DirectXFramework::GetDXFramework()->GetDevice();
}

ComPtr<ID3D11DeviceContext> Mesh::GetDeviceContext()
{
	return DirectXFramework::GetDXFramework()->GetDeviceContext();
}
