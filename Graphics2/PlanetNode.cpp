#include "PlanetNode.h"
#include <fstream>

#define clamp(a, x, y) { a = ((a) > (x) ? ((a) < (y) ? (a) : (y)) : (x)); }

bool PlanetNode::Initialise()
{
	return Generate();
}

bool PlanetNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();

	GenerateVertices(terrainData.get());
	GenerateIndices(terrainData.get());

	terrainData->SetMode(_draw);
	terrainData->RecalculateNormals();
	terrainData->Apply();

	SetMesh(terrainData);

	return true;
}

void PlanetNode::GenerateVertices(Mesh* target)
{
	const int cornerVertices = 8;
	const int edgeVertices = (_resolution * 3 - 3) * 4;
	const int faceVertices = (
		(_resolution - 1) * (_resolution - 1) +
		(_resolution - 1) * (_resolution - 1) +
		(_resolution - 1) * (_resolution - 1)) * 2;

	vector<Vector3> vertices((size_t)cornerVertices + (size_t)edgeVertices + (size_t)faceVertices);

	size_t v = 0;

	for (int y = 0; y <= (int)_resolution; ++y)
	{
		for (int x = 0; x <= (int)_resolution; ++x)
		{
			vertices[v++] = Vector3((float)x, 0, 0);
		}

		for (int z = 1; z <= (int)_resolution; ++z)
		{
			vertices[v++] = Vector3((float)_resolution, 0, (float)z);
		}

		for (int x = (int)_resolution - 1; x >= 0; --x)
		{
			vertices[v++] = Vector3((float)x, 0, (float)_resolution);
		}

		for (int z = (int)_resolution - 1; z > 0; --z)
		{
			vertices[v++] = Vector3(0, 0, (float)z);
		}
	}

	for (int z = 1; z < (int)_resolution; ++z)
	{
		for (int x = 1; x < (int)_resolution; ++x)
		{
			vertices[v++] = Vector3((float)x, (float)_resolution, (float)z);
		}
	}

	for (int z = 1; z < (int)_resolution; ++z)
	{
		for (int x = 1; x < (int)_resolution; ++x)
		{
			vertices[v++] = Vector3((float)x, 0, (float)z);
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

void PlanetNode::GenerateIndices(Mesh* target)
{
	int quads = ((_resolution * _resolution) * 3) * 2;

	vector<int> indices((size_t)quads * 6);

	int ring = _resolution * 4;
	int t = 0, v = 0;

	for (int y = 0; y < (int)_resolution; ++y, ++v)
	{
		for (int q = 0; q < ring - 1; q++, v++)
		{
			t = CreateQuad(indices, t, v, v + 1, v + ring, v + ring + 1);
		}

		t = CreateQuad(indices, t, v, v - ring + 1, v + ring, v + 1);
	}

	t = GenerateTopFace(indices, t, ring);
	t = GenerateBottomFace(indices, t, ring);

	for (size_t i = 0; i < indices.size(); ++i)
	{
		target->AddIndex(indices[i]);
	}
}

int PlanetNode::GenerateTopFace(vector<int>& indices, int t, int ring)
{
	int v = ring * _resolution;

	for (int x = 0; x < (int)_resolution - 1; ++x, ++v)
	{
		t = CreateQuad(indices, t, v, v + 1, v + ring - 1, v + ring);
	}

	t = CreateQuad(indices, t, v, v + 1, v + ring - 1, v + 2);

	int vMin = ring * (_resolution + 1) - 1;
	int vMid = vMin + 1;
	int vMax = v + 2;

	for (int z = 1; z < (int)_resolution - 1; ++z, --vMin, ++vMid, ++vMax)
	{
		t = CreateQuad(indices, t, vMin, vMid, vMin - 1, vMid + _resolution - 1);

		for (int x = 1; x < (int)_resolution - 1; ++x, ++vMid)
		{
			t = CreateQuad(indices, t, vMid, vMid + 1, vMid + _resolution - 1, vMid + _resolution);
		}

		t = CreateQuad(indices, t, vMid, vMax, vMid + _resolution - 1, vMax + 1);
	}

	int vTop = vMin - 2;
	t = CreateQuad(indices, t, vMin, vMid, vTop + 1, vTop);

	for (int x = 1; x < (int)_resolution - 1; ++x, --vTop, ++vMid)
	{
		t = CreateQuad(indices, t, vMid, vMid + 1, vTop, vTop - 1);
	}

	t = CreateQuad(indices, t, vMid, vTop - 2, vTop, vTop - 1);

	return t;
}

int PlanetNode::GenerateBottomFace(vector<int>& indices, int t, int ring)
{
	int v = 1;

	int vMid = (int)(indices.size() - (size_t)(_resolution - 1) * (size_t)(_resolution - 1));

	t = CreateQuad(indices, t, ring - 1, vMid, 0, 1);

	for (int x = 0; x < (int)_resolution - 1; ++x, ++v, vMid)
	{
		t = CreateQuad(indices, t, vMid, vMid + 1, v, v + 1);
	}

	t = CreateQuad(indices, t, vMid, v + 2, v, v + 1);

	int vMin = ring - 2;
	vMid -= _resolution - 2;
	int vMax = v + 2;

	for (int z = 1; z < (int)_resolution - 1; ++z, --vMin, ++vMid, ++vMax)
	{
		t = CreateQuad(indices, t, vMin, vMid + _resolution - 1, vMin + 1, vMid);

		for (int x = 1; x < (int)_resolution - 1; ++x, ++vMid)
		{
			t = CreateQuad(indices, t, vMid + _resolution - 1, vMid + _resolution, vMid, vMid + 1);
		}

		t = CreateQuad(indices, t, vMid + _resolution - 1, vMax + 1, vMid, vMax);
	}

	int vTop = vMin - 1;

	t = CreateQuad(indices, t, vTop + 1, vTop, vTop + 2, vMid);

	for (int x = 1; x < (int)_resolution - 1; ++x, --vTop, ++vMid)
	{
		t = CreateQuad(indices, t, vTop, vTop - 1, vMid, vMid + 1);
	}

	t = CreateQuad(indices, t, vTop, vTop - 1, vMid, vTop - 2);

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
