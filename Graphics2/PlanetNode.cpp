#include "PlanetNode.h"
#include "CameraNode.h"
#include "PlanetBuilder.h"
#include <fstream>
#include <sstream>

#undef clamp
#define clamp(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

#define SCALE_CONST 64.f
#define SCALE(x) x.X * SCALE_CONST, x.Y * SCALE_CONST, x.Z * SCALE_CONST

vector<PlanetNode*> PlanetNode::_allPlanets;

void RandomColor(float& r, float& g, float& b)
{
	r = static_cast<float>(rand()) / RAND_MAX;
	g = static_cast<float>(rand()) / RAND_MAX;
	b = static_cast<float>(rand()) / RAND_MAX;
}

CBUFFER PlanetConstantBuffer
{
	float  PlanetRadius;
	float  PlanetPeaks;
	float  PlanetOuterRadius;
	float  PlanetHasAtmosphere;
	XMFLOAT3 PlanetPosition;
	XMFLOAT4 PlanetGrassColor;
	XMFLOAT4 PlanetSandColor;
};

CBUFFER AtmosphereConstantBuffer
{
	XMFLOAT3 v3PlanetPosition;	// The position of the planet in world space
	float  fOuterRadius;		// The outer (atmosphere) radius
	float  fInnerRadius;		// The inner (planetary) radius
};

bool PlanetNode::Initialise()
{
	return Generate();
}

void PlanetNode::Update()
{
	// Since gravity is expressed as m/s/s, not multiplying the acceleration by delta time would give us an accelleration by
	// m/s/frame. Multiplying by the time delta between the previous frame and current frame will move the acceleration range
	// back to seconds.
	_linearVelocity += CalculateTotalGravity(GetWorldPosition(), GetMass(), { this }) * FRAMEWORK->GetDeltaTime();

	Vector3 position = GetPosition();
	position += _linearVelocity * FRAMEWORK->GetDeltaTime();
	SetPosition(position);

	MeshNode::Update();
}

FLOAT PlanetNode::GetMass() const
{
	return _gravity * (_radius * _radius) / G;
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

	// Start planet generation thread
	_planetBuildingThread = thread(&PlanetNode::GenerateAllLODs, this);

	return true;
}

void PlanetNode::OnPreRender()
{
	const FLOAT minimumDistance = _minimumDistance + _radius + _atmosphereThickness;	
	const FLOAT maximumDistance = _maximumDistance + _radius + _atmosphereThickness;

	const CameraNode* mainCamera = MAIN_CAMERA;
	const FLOAT cameraDistance = (mainCamera->GetWorldPosition() - GetWorldPosition()).Length() - minimumDistance + 1;
	const FLOAT cameraGradient = (cameraDistance / (maximumDistance - minimumDistance)) * _meshLODs.size();
	const size_t lodIndex = _meshLODs.size() - clamp(static_cast<size_t>(roundf(cameraGradient)), 0, _meshLODs.size());

	SetLOD(lodIndex);

	if (b_hasAtmosphere)
	{
		AtmosphereConstantBuffer* atmoBuffer = _atmosphereMaterial->GetConstantBuffer()->GetLayoutPointer<AtmosphereConstantBuffer>(1);
		atmoBuffer->v3PlanetPosition = GetWorldPosition().ToDX3();
	}

	PlanetConstantBuffer* planetBuffer = _planetMaterial->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>(1);
	planetBuffer->PlanetPosition = GetWorldPosition().ToDX3();

	MeshNode::OnPreRender(); // Does nothing, for now... ;)
							 // No like it just does nothing, don't get your hopes up.
}

void PlanetNode::OnPostRender()
{
	if (_scatterMesh)
	{
		// Place a single scatter in a predefined location since I do not have time to do more.
		Vector3 p(0, 0, 1);
		RealizeScatter(p);
	}
}

const FLOAT PlanetNode::GetHeightAtPoint(const FLOAT& radius, const Vector3& unitPos) const
{
	return _mode == TerrainMode::Procedural ? _noises.GetNoiseValue(radius, SCALE(unitPos)) : radius;
}

const FLOAT PlanetNode::GetHeightAtPoint(const Vector3& unitPos) const
{
	return GetHeightAtPoint(_radius, unitPos);
}

void PlanetNode::Orbit(const PlanetNode* const planet)
{
	Vector3 directionToPlanet = planet->GetWorldPosition() - GetWorldPosition();
	Vector3 tangentVector = Vector3::Cross(planet->GetUpVector(), directionToPlanet).Normalized();
	
	_linearVelocity = tangentVector * planet->GetOrbitalVelocity(directionToPlanet.Length()) + planet->_linearVelocity;
}

shared_ptr<PlanetNode> PlanetNode::GenerateRandom(FLOAT noiseScale)
{
	shared_ptr<PlanetNode> planet = SceneGraph::Create<PlanetNode>(L"Terrain");

	planet->SetDrawMode(MeshMode::TriangleList);
	planet->SetMode(TerrainMode::Procedural);

	auto& noiseManager = planet->GetNoiseManager();

	auto planetNoise = noiseManager.CreateNoise<BasicNoise>();
	planetNoise->SetNoiseOctaves(1);
	planetNoise->SetNoiseScale(40.f * noiseScale);
	planetNoise->SetPeakHeight(25.f);
	planetNoise->RandomizeOffsets();

	auto planetDetail = noiseManager.CreateNoise<BasicNoise>();
	planetDetail->SetNoiseDirection(NoiseDirection::ND_Inwards);
	planetDetail->SetNoiseOctaves(8);
	planetDetail->SetNoiseScale(5.f * noiseScale);
	planetDetail->SetPeakHeight(1.f);
	planetDetail->RandomizeOffsets();

	auto planetContinents = noiseManager.CreateNoise<BasicNoise>();
	planetContinents->SetNoiseBlendMode(NoiseBlendMode::BM_Multiply);
	planetContinents->SetNoiseOctaves(4);
	planetContinents->SetNoiseScale(80.f * noiseScale);
	planetContinents->SetPeakHeight(1.f);
	planetContinents->RandomizeOffsets();

	noiseManager.SetMaximumHeight(10.f);
	planet->SetRadius(256.f);

	// Define LOD resolutions...

	planet->CreateLOD(4);
	planet->CreateLOD(16);
	planet->CreateLOD(64);
	planet->CreateLOD(256);
	
	// Set colors

	float r, g, b;
	RandomColor(r, g, b);
	planet->SetGrassColor(r, g, b);

	RandomColor(r, g, b);
	planet->SetSandColor(r, g, b);

	return planet;
}

Vector3 PlanetNode::CalculateTotalGravity(Vector3 sourcePoint, FLOAT sourceMass, initializer_list<PlanetNode*> exclude)
{
	Vector3 overallAcceleration;

	for (PlanetNode* planet : _allPlanets)
	{
		if (find(exclude.begin(), exclude.end(), planet) == exclude.end())
		{
			Vector3 forceDir = planet->GetWorldPosition() - sourcePoint;
			FLOAT distanceSqr = forceDir.SqrLength();
			forceDir.Normalize();

			Vector3 force = forceDir * G * sourceMass * planet->GetMass() / distanceSqr;
			Vector3 acceleration = force / sourceMass;

			overallAcceleration += acceleration;
		}
	}

	return overallAcceleration;
}

void PlanetNode::GenerateAllLODs()
{
	for (size_t lod = 0; lod < _requestedLODs.size() && !b_abortBuild; ++lod)
	{
		_meshLODs.push_back(GenerateLOD(_requestedLODs[lod]));
	}
}

shared_ptr<Mesh> PlanetNode::GenerateLOD(const LOD& resolution)
{
	/**  Terrain generation */
	shared_ptr<Mesh> terrain = make_shared<Mesh>();
	PlanetBuilder terrainBuilder(resolution.planetLOD);

	terrainBuilder.SetStopFlag(&b_abortBuild);
	terrainBuilder.Generate();
	MakeSphere(terrainBuilder.GetVertices(), resolution.planetLOD, _radius, true);
	terrainBuilder.Map(terrain.get());

	terrain->RecalculateNormals();
	terrain->SetMode(_draw);

	terrain->Apply();

	/**  Atmosphere generation */

	if (b_hasAtmosphere)
	{
		shared_ptr<Mesh> atmosphere = make_shared<Mesh>();
		terrain->AddSubmesh(atmosphere);

		PlanetBuilder atmosphereBuilder(resolution.atmosphereLOD);

		atmosphereBuilder.SetStopFlag(&b_abortBuild);
		atmosphereBuilder.Generate();
		MakeSphere(atmosphereBuilder.GetVertices(), resolution.atmosphereLOD, _radius + _atmosphereThickness, false);
		atmosphereBuilder.Map(atmosphere.get());

		atmosphere->SetMode(_draw);
		atmosphere->RecalculateNormals();
		atmosphere->Invert();

		atmosphere->Apply();
	}

	return terrain;
}

void PlanetNode::MakeSphere(vector<Vector3>& vertices, const UINT& resolution, float radius, bool deform)
{
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normalize();
		vertices[i] *= deform ? GetHeightAtPoint(radius, vertices[i]) : radius;
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

	mat->GetConstantBuffer()->CreateBuffer<PlanetConstantBuffer>();

	PlanetConstantBuffer* planetBuffer = mat->GetConstantBuffer()->GetLayoutPointer<PlanetConstantBuffer>(1);
	planetBuffer->PlanetRadius = max(_radius, GetNoiseManager().GetMinimumHeight());
	planetBuffer->PlanetPeaks = GetNoiseManager().GetMaximumHeight();
	planetBuffer->PlanetOuterRadius = planetBuffer->PlanetRadius + _atmosphereThickness;
	planetBuffer->PlanetHasAtmosphere = static_cast<float>(b_hasAtmosphere);
	planetBuffer->PlanetGrassColor = _grassColor;
	planetBuffer->PlanetSandColor = _sandColor;
}

void PlanetNode::PopulateAtmosphereMaterial(shared_ptr<Material>& mat)
{
	const float innerRadius = max(_radius, GetNoiseManager().GetMinimumHeight());
	const float outerRadius = innerRadius + _atmosphereThickness;

	mat->SetShader(RESOURCES->GetShader(L"Shaders/atmosphere.hlsl"));
	mat->SetTexture(0, RESOURCES->GetTexture(L"PlanetData/atmo.png"));

	mat->GetConstantBuffer()->CreateBuffer<AtmosphereConstantBuffer>();

	AtmosphereConstantBuffer* atmoBuffer = mat->GetConstantBuffer()->GetLayoutPointer<AtmosphereConstantBuffer>(1);
	atmoBuffer->fOuterRadius = outerRadius;
	atmoBuffer->fInnerRadius = innerRadius;

	mat->SetTransparencyEnabled(true);
	mat->SetTransparencyModes(Blend::One, Blend::One, Operation::Add);
}

void PlanetNode::RealizeScatter(Vector3& cubePosition) const
{
	FLOAT height = GetHeightAtPoint(cubePosition);

	Vector3 c = MAIN_CAMERA->GetWorldPosition();
	Vector3 p = cubePosition  * height + GetWorldPosition();

	FLOAT dist = (c - p).Length();

	if (dist < _scatterMinimumDistance)
	{
		Vector3 u = GetUpVector();
		Vector3 r = Vector3::Cross(u, cubePosition);

		r.Normalize();

		XMMATRIX T = XMMatrixTranslation(XYZ(p));
		XMMATRIX R = XMMatrixIdentity(); // Cannot figure out rotation from axis...
		XMMATRIX S = XMMatrixScaling(10, 10, 10);

		XMMATRIX M = S * R * T;
		RenderScatter(M);
	}
}

void PlanetNode::RenderScatter(XMMATRIX& location) const
{
	RenderTransformed(_scatterMesh, _scatterMesh->GetReferenceMaterial(), location);

	for (size_t submesh = 0; submesh < _scatterMesh->GetSubmeshCount(); ++submesh)
	{
		RenderTransformed(_scatterMesh->GetSubmesh(submesh), _scatterMesh->GetSubmesh(submesh)->GetReferenceMaterial(), location);
	}
}

void PlanetNode::SetLOD(size_t lod)
{
	lod = clamp(lod, 0, _meshLODs.size() - 1);

	if (lod != _currentLOD)
	{
		SetMesh(_meshLODs[lod]);
		_currentLOD = static_cast<int>(lod);
	}
}

void PlanetNode::RemovePlanet(PlanetNode* target)
{
	vector<PlanetNode*>::iterator it = find(_allPlanets.begin(), _allPlanets.end(), target);

	if (it != _allPlanets.end())
	{
		_allPlanets.erase(it);
	}
}
