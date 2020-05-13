#include "PlanetBuilder.h"

#define CAST(t, x) static_cast<t>(x)
#define offset (F(gridSize) / 2.f)
#define gridSize CAST(long long, _resolution)

#define F(x) CAST(float, x)


void PlanetBuilder::Generate()
{
	GenerateVertices();
	GenerateIndices();
}

void PlanetBuilder::Map(Mesh* target)
{
	target->Clear();

	for (size_t i = 0; i < _vertices.size(); ++i)
	{
		target->AddVertex(Vertex{
			_vertices[i].ToDX3(),
			{ 0, 1, 0 },
			_texCoord[i]
		});
	}

	for (size_t i = 0; i < _indices.size(); ++i)
	{
		target->AddIndex(CAST(UINT, _indices[i]));
	}
}

void PlanetBuilder::GenerateVertices()
{
	const int cornerVertices = 8;
	const int edgeVertices = (gridSize + gridSize + gridSize - 3) * 4;
	const int faceVertices = (
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1)) * 2;

	_vertices.resize(CAST(size_t, cornerVertices) + CAST(size_t, edgeVertices) + CAST(size_t, faceVertices));
	_texCoord.resize(_vertices.size());

	int v = 0;
	for (int y = 0; y <= gridSize && CanProceed(); y++)
	{
		for (int x = 0; x <= gridSize && CanProceed(); x++)
		{
			SetVertex(v, F(x) - offset, F(y) - offset, -offset);
			_texCoord[v++] = { F(x) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int z = 1; z <= gridSize && CanProceed(); z++)
		{
			SetVertex(v, F(gridSize) - offset, F(y) - offset, F(z) - offset);
			_texCoord[v++] = { F(z) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int x = gridSize - 1; x >= 0 && CanProceed(); x--)
		{
			SetVertex(v, F(x) - offset, F(y) - offset, F(gridSize) - offset);
			_texCoord[v++] = { F(x) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int z = gridSize - 1; z > 0 && CanProceed(); z--)
		{
			SetVertex(v, -offset, F(y) - offset, F(z) - offset);
			_texCoord[v++] = { F(z) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
	}

	for (int z = 1; z < gridSize && CanProceed(); z++)
	{
		for (int x = 1; x < gridSize && CanProceed(); x++)
		{
			SetVertex(v, F(x) - offset, F(gridSize) - offset, F(z) - offset);
			_texCoord[v++] = { F(x) / F(gridSize), F(z) / F(gridSize) };
		}
	}
	for (int z = 1; z < gridSize && CanProceed(); z++)
	{
		for (int x = 1; x < gridSize && CanProceed(); x++)
		{
			SetVertex(v, F(x) - offset, -offset, F(z) - offset);
			_texCoord[v++] = { F(x) / F(gridSize), F(z) / F(gridSize) };
		}
	}
}

void PlanetBuilder::GenerateIndices()
{
	const int quads = (gridSize * gridSize + gridSize * gridSize + gridSize * gridSize) * 2;
	_indices.resize(CAST(size_t, quads) * 6);

	const int ring = (gridSize + gridSize) * 2;
	int t = 0, v = 0;

	for (int y = 0; y < gridSize && CanProceed(); y++, v++)
	{
		for (int q = 0; q < ring - 1 && CanProceed(); q++, v++)
		{
			t = CreateQuad(t, v, v + 1, v + ring, v + ring + 1);
		}

		t = CreateQuad(t, v, v - ring + 1, v + ring, v + 1);
	}

	t = GenerateTopFace(t, ring);
	t = GenerateBottomFace(t, ring);
}

int PlanetBuilder::GenerateTopFace(int t, int ring)
{
	int v = ring * gridSize;

	for (int x = 0; x < gridSize - 1 && CanProceed(); x++, v++)
	{
		t = CreateQuad(t, v, v + 1, v + ring - 1, v + ring);
	}

	t = CreateQuad(t, v, v + 1, v + ring - 1, v + 2);

	int vMin = ring * (gridSize + 1) - 1;
	int vMid = vMin + 1;
	int vMax = v + 2;

	for (int z = 1; z < gridSize - 1 && CanProceed(); z++, vMin--, vMid++, vMax++)
	{
		t = CreateQuad(t, vMin, vMid, vMin - 1, vMid + gridSize - 1);
		for (int x = 1; x < gridSize - 1 && CanProceed(); x++, vMid++)
		{
			t = CreateQuad(t, vMid, vMid + 1, vMid + gridSize - 1, vMid + gridSize);
		}
		t = CreateQuad(t, vMid, vMax, vMid + gridSize - 1, vMax + 1);
	}

	int vTop = vMin - 2;
	t = CreateQuad(t, vMin, vMid, vTop + 1, vTop);
	for (int x = 1; x < gridSize - 1 && CanProceed(); x++, vTop--, vMid++)
	{
		t = CreateQuad(t, vMid, vMid + 1, vTop, vTop - 1);
	}
	t = CreateQuad(t, vMid, vTop - 2, vTop, vTop - 1);

	return t;
}

int PlanetBuilder::GenerateBottomFace(int t, int ring)
{
	int v = 1;
	int vMid = CAST(int, _vertices.size() - CAST(size_t, gridSize - 1) * CAST(size_t, gridSize - 1));

	t = CreateQuad(t, ring - 1, vMid, 0, 1);
	for (int x = 1; x < gridSize - 1 && CanProceed(); x++, v++, vMid++)
	{
		t = CreateQuad(t, vMid, vMid + 1, v, v + 1);
	}
	t = CreateQuad(t, vMid, v + 2, v, v + 1);

	int vMin = ring - 2;
	vMid -= gridSize - 2;
	int vMax = v + 2;

	for (int z = 1; z < gridSize - 1 && CanProceed(); z++, vMin--, vMid++, vMax++)
	{
		t = CreateQuad(t, vMin, vMid + gridSize - 1, vMin + 1, vMid);
		for (int x = 1; x < gridSize - 1 && CanProceed(); x++, vMid++)
		{
			t = CreateQuad(t, vMid + gridSize - 1, vMid + gridSize, vMid, vMid + 1);
		}
		t = CreateQuad(t, vMid + gridSize - 1, vMax + 1, vMid, vMax);
	}

	int vTop = vMin - 1;
	t = CreateQuad(t, vTop + 1, vTop, vTop + 2, vMid);
	for (int x = 1; x < gridSize - 1 && CanProceed(); x++, vTop--, vMid++)
	{
		t = CreateQuad(t, vTop, vTop - 1, vMid, vMid + 1);
	}
	t = CreateQuad(t, vTop, vTop - 1, vMid, vTop - 2);

	return t;
}

int PlanetBuilder::CreateQuad(int i, int v00, int v10, int v01, int v11)
{
	_indices[i] = v00;
	_indices[(size_t)i + 1] = _indices[(size_t)i + 4] = v01;
	_indices[(size_t)i + 2] = _indices[(size_t)i + 3] = v10;
	_indices[(size_t)i + 5] = v11;

	return i + 6;
}

void PlanetBuilder::SetVertex(int i, const float& x, const float& y, const float& z)
{
	Vector3 v = Vector3(x, y, z) / F(gridSize);

	float x2 = v.X * v.X;
	float y2 = v.Y * v.Y;
	float z2 = v.Z * v.Z;

	Vector3 s;
	s.SetX(v.X * sqrt(1.f - y2 / 2.f - z2 / 2.f + y2 * z2 / 3.f));
	s.SetY(v.Y * sqrt(1.f - x2 / 2.f - z2 / 2.f + x2 * z2 / 3.f));
	s.SetZ(v.Z * sqrt(1.f - x2 / 2.f - y2 / 2.f + x2 * y2 / 3.f));

	s *= F(gridSize);

	_vertices[i] = s;
}
