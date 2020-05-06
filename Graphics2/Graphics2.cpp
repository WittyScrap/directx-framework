#include "Graphics2.h"
#include "RobotNode.h"
#include "PlaneNode.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PlanetNode.h"
#include "CameraNode.h"
#include "PawnNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 0, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<PlanetNode> planet = SceneGraph::Create<PlanetNode>(L"Terrain");
	shared_ptr<PawnNode> mainPawn = SceneGraph::Create<PawnNode>();

	planet->SetDrawMode(MeshMode::TriangleList);
	planet->SetMode(TerrainMode::Procedural);

	auto& noiseManager = planet->GetNoiseManager();

	auto planetNoise = noiseManager.CreateNoise<BasicNoise>();
	planetNoise->SetNoiseOctaves(1);
	planetNoise->SetNoiseScale(40.f);
	planetNoise->SetPeakHeight(25.f);
	planetNoise->RandomizeOffsets();

	auto planetDetail = noiseManager.CreateNoise<BasicNoise>();
	planetDetail->SetNoiseDirection(NoiseDirection::ND_Inwards);
	planetDetail->SetNoiseOctaves(8);
	planetDetail->SetNoiseScale(5.f);
	planetDetail->SetPeakHeight(1.f);
	planetDetail->RandomizeOffsets();

	auto planetContinents = noiseManager.CreateNoise<BasicNoise>();
	planetContinents->SetNoiseBlendMode(NoiseBlendMode::BM_Multiply);
	planetContinents->SetNoiseOctaves(4);
	planetContinents->SetNoiseScale(80.f);
	planetContinents->SetPeakHeight(1.f);
	planetDetail->RandomizeOffsets();

	noiseManager.SetMaximumHeight(10.f);

	planet->SetRadius(256.f);

	// Define LOD resolutions...
	planet->CreateLOD(4);
	planet->CreateLOD(16);
	planet->CreateLOD(64);
	planet->CreateLOD(256);

	mainPawn->SetPosition({ 0, 128, -1024 });
	mainPawn->SetMovementSpeed(2.f);
	mainPawn->SetMain();

	SCENE->Add(planet);
	SCENE->Add(mainPawn);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
