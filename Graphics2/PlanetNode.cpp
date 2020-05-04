#include "PlanetNode.h"
#include "CameraNode.h"
#include <fstream>

#define CAST(t, x) static_cast<t>(x)
#define gridSize CAST(long long, _resolution)
#define offset (F(_resolution) / 2.f)

#define F(x) CAST(float, x)

struct PlanetConstantBuffer : public ConstantBuffer
{
	float  PlanetRadius;
	float  PlanetPeaks;
	float  PlanetOuterRadius;
	float  PlanetResolution;
	XMFLOAT3 PlanetPosition;
};

struct AtmosphereConstantBuffer : public ConstantBuffer
{
	XMFLOAT3 v3PlanetPosition;	// The position of the planet in world space
	float  fOuterRadius;		// The outer (atmosphere) radius
	float  fInnerRadius;		// The inner (planetary) radius
};

bool PlanetNode::Initialise()
{
	return Generate();
}

bool PlanetNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();
	_planetMaterial = make_shared<Material>();

	shared_ptr<Mesh> atmosphereData = terrainData->AddSubmesh();
	_atmosphereMaterial = make_shared<Material>();

	InternalGenerateSpheroid(terrainData.get(), _radius, true);
	InternalGenerateSpheroid(atmosphereData.get(), _radius + _atmosphereThickness, false);

	atmosphereData->Invert();
	atmosphereData->Apply();

	PopulateGroundMaterial(_planetMaterial);
	PopulateAtmosphereMaterial(_atmosphereMaterial);

	SetMaterial(_planetMaterial);
	SetMaterial(0, _atmosphereMaterial);

	SetMesh(terrainData);

	return true;
}

void PlanetNode::OnPreRender()
{
	const float innerRadius = _radius;
	const float outerRadius = _radius + _atmosphereThickness;

	AtmosphereConstantBuffer* atmoBuffer = _atmosphereMaterial->GetConstantBuffer()->GetLayoutPointer<AtmosphereConstantBuffer>();
	atmoBuffer->v3PlanetPosition = GetPosition().ToDX3();
	atmoBuffer->fOuterRadius = outerRadius;
	atmoBuffer->fInnerRadius = innerRadius;

	PlanetConstantBuffer* planetBuffer = _planetMaterial->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>();
	planetBuffer->PlanetPosition = GetPosition().ToDX3();
}

bool PlanetNode::InternalGenerateSpheroid(Mesh* target, float radius, bool deform)
{
	GenerateVertices(target, radius, deform);
	GenerateIndices(target, target->GetVertices().size());

	target->SetMode(_draw);
	target->RecalculateNormals();
	target->Apply();

	return true;
}

void PlanetNode::GenerateVertices(Mesh* target, float radius, bool deform)
{
	const int cornerVertices = 8;
	const int edgeVertices = (gridSize + gridSize + gridSize - 3) * 4;
	const int faceVertices = (
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1) +
		(gridSize - 1) * (gridSize - 1)) * 2;

	vector<Vector3> vertices(CAST(size_t, cornerVertices) + CAST(size_t, edgeVertices) + CAST(size_t, faceVertices));
	vector<XMFLOAT2> uv(vertices.size());

	int v = 0;
	for (int y = 0; y <= gridSize; y++)
	{
		for (int x = 0; x <= gridSize; x++)
		{
			SetVertex(vertices, v, F(x) - offset, F(y) - offset, -offset);
			uv[v++] = { F(x) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int z = 1; z <= gridSize; z++)
		{
			SetVertex(vertices, v, F(gridSize) - offset, F(y) - offset, F(z) - offset);
			uv[v++] = { F(z) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int x = gridSize - 1; x >= 0; x--)
		{
			SetVertex(vertices, v, F(x) - offset, F(y) - offset, F(gridSize) - offset);
			uv[v++] = { F(x) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
		for (int z = gridSize - 1; z > 0; z--)
		{
			SetVertex(vertices, v, -offset, F(y) - offset, F(z) - offset);
			uv[v++] = { F(z) / F(gridSize * 4) * 4, F(y) / F(gridSize) };
		}
	}

	for (int z = 1; z < gridSize; z++)
	{
		for (int x = 1; x < gridSize; x++)
		{
			SetVertex(vertices, v, F(x) - offset, F(gridSize) - offset, F(z) - offset);
			uv[v++] = { F(x) / F(gridSize), F(z) / F(gridSize) };
		}
	}
	for (int z = 1; z < gridSize; z++)
	{
		for (int x = 1; x < gridSize; x++)
		{
			SetVertex(vertices, v, F(x) - offset, -offset, F(z) - offset);
			uv[v++] = { F(x) / F(gridSize), F(z) / F(gridSize) };
		}
	}

	MakeSphere(vertices, radius, deform);
	
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		target->AddVertex(Vertex{
			vertices[i].ToDX3(),
			{ 0, 1, 0 },
			uv[i]
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

void PlanetNode::MakeSphere(vector<Vector3>& vertices, float radius, bool deform)
{
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const float height = deform ? _noises.GetNoiseValue(radius, XYZ(vertices[i])) : radius;

		vertices[i].Normalize();
		vertices[i] *= height;
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

void PlanetNode::SetVertex(vector<Vector3>& vertices, int i, const float& x, const float& y, const float& z)
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

	vertices[i] = s;
}

void PlanetNode::PopulateGroundMaterial(shared_ptr<Material>& mat)
{
	mat->SetShader(RESOURCES->GetShader(L"Shaders/planet.hlsl"));

	mat->SetTexture(0, RESOURCES->GetTexture(L"PlanetData/ground.png"));
	mat->SetTexture(1, RESOURCES->GetTexture(L"PlanetData/cliff.png"));
	mat->SetTexture(2, RESOURCES->GetTexture(L"PlanetData/sand.png"));
	mat->SetTexture(3, RESOURCES->GetTexture(L"PlanetData/snow.png"));
	mat->SetTexture(4, RESOURCES->GetTexture(L"PlanetData/atmo.png"));

	mat->GetConstantBuffer()->CreateBufferData<PlanetConstantBuffer>();

	PlanetConstantBuffer* planetBuffer = mat->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>();
	planetBuffer->PlanetRadius = max(_radius, GetNoiseManager().GetMinimumHeight());
	planetBuffer->PlanetPeaks = GetNoiseManager().GetMaximumHeight();
	planetBuffer->PlanetResolution = (FLOAT)_resolution / 4.f;
	planetBuffer->PlanetOuterRadius = _radius + _atmosphereThickness;
}

void PlanetNode::PopulateAtmosphereMaterial(shared_ptr<Material>& mat)
{
	mat->SetShader(RESOURCES->GetShader(L"Shaders/atmosphere.hlsl"));
	mat->GetConstantBuffer()->CreateBufferData<AtmosphereConstantBuffer>();

	mat->SetTexture(0, RESOURCES->GetTexture(L"PlanetData/atmo.png"));
}

#undef gridSize