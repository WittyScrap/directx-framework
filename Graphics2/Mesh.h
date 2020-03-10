#pragma once
#include "Core.h"
#include <vector>

// DX Framework forward declaration
class DirectXFramework;

/**
 * Represents a simple vertex.
 *
 */
struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

/**
 * Represents a simple container for vertices,
 * triangles, normals, and other information related
 * to a mesh.
 */
class Mesh
{
public:
    /* Constructors */
    Mesh();
    Mesh(const Mesh& copy);
    Mesh(const vector<Vector3> vertices, const vector<Vector3> normals, const vector<int> triangles);

    virtual ~Mesh();

    const vector<Vector3>& GetVertices() const;
    const vector<Vector3>& GetNormals() const;
    const vector<int>& GetTriangles() const;

    void AddTriangle(const Vector3& a, const Vector3& b, const Vector3& c);

	void AddVertex(const Vector3& vertex);
	void AddNormal(const Vector3& normal);
    void AddIndex(const int& index);

    void SetVertices(const vector<Vector3>& vertices);
    void SetNormals(const vector<Vector3>& normals);
    void SetIndices(const vector<int>& indices);

    void ClearVertices();
    void ClearNormals();
    void ClearIndices();

    void Clear();
    void Apply();

private:
    static DirectXFramework* GetFramework();

private:
    vector<Vector3> _vertices;
    vector<Vector3> _normals;
    vector<int> _triangles;

    ComPtr<ID3D11Buffer> _vertexBuffer;
    ComPtr<ID3D11Buffer> _indexBuffer;
};

typedef shared_ptr<Mesh> MeshPtr;

