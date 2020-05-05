#include "PlanetNode.h"
#include "CameraNode.h"
#include "PlanetBuilder.h"
#include <fstream>
#include <sstream>

void Output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(s2ws(szBuff).c_str());
}

struct PlanetConstantBuffer : public ConstantBuffer
{
	float  PlanetRadius;
	float  PlanetPeaks;
	float  PlanetOuterRadius;
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

void PlanetNode::CreateLOD(const UINT& resolution)
{
	_requestedLODs.push_back({ resolution, min(resolution, _atmosphereMaxResolution) });
}

bool PlanetNode::Generate()
{
	_planetMaterial = make_shared<Material>();
	_atmosphereMaterial = make_shared<Material>();

	PopulateGroundMaterial(_planetMaterial);
	PopulateAtmosphereMaterial(_atmosphereMaterial);

	SetMaterial(_planetMaterial);
	SetMaterial(0, _atmosphereMaterial);

	for (size_t lod = 0; lod < _requestedLODs.size(); ++lod)
	{
		_meshLODs.push_back(GenerateLOD(_requestedLODs[lod]));
	}

	// Set up lowest LOD
	SetLOD(0);

	return true;
}

void PlanetNode::OnPreRender()
{
	const FLOAT minimumDistance = _minimumDistance + _radius + _atmosphereThickness;
	const FLOAT maximumDistance = _maximumDistance + _radius + _atmosphereThickness;

	const CameraNode* mainCamera = MAIN_CAMERA;
	const FLOAT cameraDistance = (mainCamera->GetPosition() - GetPosition()).Length() - minimumDistance + 1;
	const FLOAT cameraGradient = (cameraDistance / (maximumDistance - minimumDistance)) * _meshLODs.size();
	const size_t lodIndex = _meshLODs.size() - static_cast<size_t>(roundf(cameraGradient));

	SetLOD(lodIndex);

	AtmosphereConstantBuffer* atmoBuffer = _atmosphereMaterial->GetConstantBuffer()->GetLayoutPointer<AtmosphereConstantBuffer>();
	atmoBuffer->v3PlanetPosition = GetPosition().ToDX3();

	PlanetConstantBuffer* planetBuffer = _planetMaterial->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>();
	planetBuffer->PlanetPosition = GetPosition().ToDX3();
}

shared_ptr<Mesh> PlanetNode::GenerateLOD(const LOD& resolution)
{
	shared_ptr<Mesh> terrain = make_shared<Mesh>();
	shared_ptr<Mesh> atmosphere = make_shared<Mesh>();
	terrain->AddSubmesh(atmosphere);

	PlanetBuilder terrainBuilder(resolution.planetLOD);
	PlanetBuilder atmosphereBuilder(resolution.atmosphereLOD);

	terrainBuilder.Generate();
	atmosphereBuilder.Generate();

	MakeSphere(terrainBuilder.GetVertices(), resolution.planetLOD, _radius, true);
	MakeSphere(atmosphereBuilder.GetVertices(), resolution.atmosphereLOD, _radius + _atmosphereThickness, false);

	terrainBuilder.Map(terrain.get());
	atmosphereBuilder.Map(atmosphere.get());

	terrain->RecalculateNormals();
	terrain->SetMode(_draw);
	atmosphere->SetMode(_draw);
	atmosphere->RecalculateNormals();
	atmosphere->Invert();

	terrain->Apply();
	atmosphere->Apply();

	return terrain;
}

void PlanetNode::MakeSphere(vector<Vector3>& vertices, const UINT& resolution, float radius, bool deform)
{
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const float height = deform && _mode == TerrainMode::Procedural ? _noises.GetNoiseValue(radius, 
																		  (vertices[i].X / resolution) * 128,
																		  (vertices[i].Y / resolution) * 128,
																		  (vertices[i].Z / resolution) * 128) : radius;

		vertices[i].Normalize();
		vertices[i] *= height;
	}
}

void PlanetNode::PopulateGroundMaterial(shared_ptr<Material>& mat)
{
	mat->SetShader(RESOURCES->GetShader(L"Shaders/planet.hlsl"));

	mat->SetTexture(0, RESOURCES->GetTexture(L"PlanetData/ground.png"));
	mat->SetTexture(1, RESOURCES->GetTexture(L"PlanetData/cliff.png"));
	mat->SetTexture(2, RESOURCES->GetTexture(L"PlanetData/sand.png"));
	mat->SetTexture(3, RESOURCES->GetTexture(L"PlanetData/snow.png"));

	// Atmosphere atlas
	mat->SetTexture(4, RESOURCES->GetTexture(L"PlanetData/atmo.png"));

	mat->GetConstantBuffer()->CreateBufferData<PlanetConstantBuffer>();

	PlanetConstantBuffer* planetBuffer = mat->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>();
	planetBuffer->PlanetRadius = max(_radius, GetNoiseManager().GetMinimumHeight());
	planetBuffer->PlanetPeaks = GetNoiseManager().GetMaximumHeight();
	planetBuffer->PlanetOuterRadius = _radius + _atmosphereThickness;
}

void PlanetNode::PopulateAtmosphereMaterial(shared_ptr<Material>& mat)
{
	const float innerRadius = _radius;
	const float outerRadius = _radius + _atmosphereThickness;

	mat->SetShader(RESOURCES->GetShader(L"Shaders/atmosphere.hlsl"));
	mat->SetTexture(0, RESOURCES->GetTexture(L"PlanetData/atmo.png"));
	mat->GetConstantBuffer()->CreateBufferData<AtmosphereConstantBuffer>();

	AtmosphereConstantBuffer* atmoBuffer = mat->GetConstantBuffer()->GetLayoutPointer<AtmosphereConstantBuffer>();
	atmoBuffer->fOuterRadius = outerRadius;
	atmoBuffer->fInnerRadius = innerRadius;
}

void PlanetNode::SetLOD(size_t lod)
{
	clamp(lod, 0, _meshLODs.size() - 1);

	if (lod != _currentLOD)
	{
		SetMesh(_meshLODs[lod]);
		_currentLOD = static_cast<int>(lod);
	}
}

#undef gridSize