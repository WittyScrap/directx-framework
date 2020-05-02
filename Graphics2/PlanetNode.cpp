#include "PlanetNode.h"
#include <fstream>

#define clamp(a, x, y) { a = ((a) > (x) ? ((a) < (y) ? (a) : (y)) : (x)); }
#define CAST(t, x) static_cast<t>(x)
#define gridSize CAST(long long, _resolution)
#define offset (F(_resolution) / 2.f)

#define F(x) CAST(float, x)

bool PlanetNode::Initialise()
{
	return Generate();
}

bool PlanetNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();

	GenerateVertices(terrainData.get());
	GenerateIndices(terrainData.get(), terrainData->GetVertices().size());

	terrainData->SetMode(_draw);
	terrainData->RecalculateNormals();
	terrainData->Apply();

	SetMesh(terrainData);

	return true;
}

void PlanetNode::GenerateVertices(Mesh* target)
{
	const int cornerVertices = 8;
	const int edgeVertices = (gridSize + gridSize + gridSize - 3) * 4;
	const int faceVertices = (
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1)) * 2;

	vector<Vector3> vertices(CAST(size_t, cornerVertices) + CAST(size_t, edgeVertices) + CAST(size_t, faceVertices));

	int v = 0;
	for (int y = 0; y <= gridSize; y++)
	{
		for (int x = 0; x <= gridSize; x++)
		{
			vertices[v++] = Vector3(F(x) - offset, F(y) - offset, -offset);
		}
		for (int z = 1; z <= gridSize; z++)
		{
			vertices[v++] = Vector3(F(gridSize) - offset, F(y) - offset, F(z) - offset);
		}
		for (int x = gridSize - 1; x >= 0; x--)
		{
			vertices[v++] = Vector3(F(x) - offset, F(y) - offset, F(gridSize) - offset);
		}
		for (int z = gridSize - 1; z > 0; z--)
		{
			vertices[v++] = Vector3(-offset, F(y) - offset, F(z) - offset);
		}
	}

	for (int z = 1; z < gridSize; z++)
	{
		for (int x = 1; x < gridSize; x++)
		{
			vertices[v++] = Vector3(F(x) - offset, F(gridSize) - offset, F(z) - offset);
		}
	}
	for (int z = 1; z < gridSize; z++)
	{
		for (int x = 1; x < gridSize; x++)
		{
			vertices[v++] = Vector3(F(x) - offset, -offset, F(z) - offset);
		}
	}

	MakeSphere(vertices);
	
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		target->AddVertex(Vertex{
			vertices[i].ToDX3(),
			{ 0, 1, 0 },
			{ 0, 0 }
		});
	}
}

void PlanetNode::GenerateIndices(Mesh* target, size_t verticesLength)
{
	const int quads = (gridSize * gridSize + gridSize * gridSize + gridSize * gridSize) * 2;
	vector<int> triangles(CAST(size_t, quads) * 6);
	const int ring = (gridSize + gridSize) * 2;
	int t = 0, v = 0;

	for (int y = 0; y < gridSize; y++, v++)
	{
		for (int q = 0; q < ring - 1; q++, v++)
		{
			t = CreateQuad(triangles, t, v, v + 1, v + ring, v + ring + 1);
		}

		t = CreateQuad(triangles, t, v, v - ring + 1, v + ring, v + 1);
	}

	t = GenerateTopFace(triangles, t, ring);
	t = GenerateBottomFace(triangles, t, ring, verticesLength);

	for (size_t i = 0; i < triangles.size(); ++i)
	{
		target->AddIndex(CAST(UINT, triangles[i]));
	}
}

int PlanetNode::GenerateTopFace(vector<int>& triangles, int t, int ring)
{
	int v = ring * gridSize;

	for (int x = 0; x < gridSize - 1; x++, v++) 
	{
		t = CreateQuad(triangles, t, v, v + 1, v + ring - 1, v + ring);
	}

	t = CreateQuad(triangles, t, v, v + 1, v + ring - 1, v + 2);

	int vMin = ring * (gridSize + 1) - 1;
	int vMid = vMin + 1;
	int vMax = v + 2;

	for (int z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) 
	{
		t = CreateQuad(triangles, t, vMin, vMid, vMin - 1, vMid + gridSize - 1);
		for (int x = 1; x < gridSize - 1; x++, vMid++)
		{
			t = CreateQuad(triangles, t, vMid, vMid + 1, vMid + gridSize - 1, vMid + gridSize);
		}
		t = CreateQuad(triangles, t, vMid, vMax, vMid + gridSize - 1, vMax + 1);
	}

	int vTop = vMin - 2;
	t = CreateQuad(triangles, t, vMin, vMid, vTop + 1, vTop);
	for (int x = 1; x < gridSize - 1; x++, vTop--, vMid++)
	{
		t = CreateQuad(triangles, t, vMid, vMid + 1, vTop, vTop - 1);
	}
	t = CreateQuad(triangles, t, vMid, vTop - 2, vTop, vTop - 1);

	return t;
}

int PlanetNode::GenerateBottomFace(vector<int>& triangles, int t, int ring, size_t verticesLength)
{
	int v = 1;
	int vMid = CAST(int, verticesLength - CAST(size_t, gridSize - 1) * CAST(size_t, gridSize - 1));

	t = CreateQuad(triangles, t, ring - 1, vMid, 0, 1);
	for (int x = 1; x < gridSize - 1; x++, v++, vMid++) 
	{
		t = CreateQuad(triangles, t, vMid, vMid + 1, v, v + 1);
	}
	t = CreateQuad(triangles, t, vMid, v + 2, v, v + 1);

	int vMin = ring - 2;
	vMid -= gridSize - 2;
	int vMax = v + 2;

	for (int z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) 
	{
		t = CreateQuad(triangles, t, vMin, vMid + gridSize - 1, vMin + 1, vMid);
		for (int x = 1; x < gridSize - 1; x++, vMid++) 
		{
			t = CreateQuad(triangles, t, vMid + gridSize - 1, vMid + gridSize, vMid, vMid + 1);
		}
		t = CreateQuad(triangles, t, vMid + gridSize - 1, vMax + 1, vMid, vMax);
	}

	int vTop = vMin - 1;
	t = CreateQuad(triangles, t, vTop + 1, vTop, vTop + 2, vMid);
	for (int x = 1; x < gridSize - 1; x++, vTop--, vMid++) 
	{
		t = CreateQuad(triangles, t, vTop, vTop - 1, vMid, vMid + 1);
	}
	t = CreateQuad(triangles, t, vTop, vTop - 1, vMid, vTop - 2);

	return t;
}

void PlanetNode::MakeSphere(vector<Vector3>& vertices)
{
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normalize();
		vertices[i] *= _radius;
	}
}

int PlanetNode::CreateQuad(vector<int>& indices, int i, int v00, int v10, int v01, int v11)
{
	indices[i] = v00;
	indices[(size_t)i + 1] = indices[(size_t)i + 4] = v01;
	indices[(size_t)i + 2] = indices[(size_t)i + 3] = v10;
	indices[(size_t)i + 5] = v11;

	return i + 6;
}

constexpr FLOAT PlanetNode::GetNormalizedValue(const UINT& value, const UINT& range) const
{
	if (value == 0)
	{
		return -1.f;
	}

	if (value == range)
	{
		return 1.f;
	}

	return static_cast<float>(value) / static_cast<float>(range) * 2 - 1;
}

FLOAT PlanetNode::GetNoiseValue(FLOAT x, FLOAT y, FLOAT z) const
{
	return _constantValue;
}

#undef gridSize