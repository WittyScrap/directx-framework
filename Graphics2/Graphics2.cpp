#include "Graphics2.h"
#include "RobotNode.h"
#include "PlaneNode.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "TerrainNode.h"
#include "CameraNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 1, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<TerrainNode> terrain = SceneGraph::Create<TerrainNode>(L"Terrain");
	shared_ptr<CameraNode> mainCam = SceneGraph::Create<CameraNode>(L"Main Camera");
	shared_ptr<PlaneNode> plane = SceneGraph::Create<PlaneNode>(L"Neaowww");

	//	terrain->LoadHeightMap(L"Example_HeightMap.raw");

	terrain->SetDrawMode(MeshMode::TriangleList);
	terrain->SetMode(TerrainMode::Procedural);

	FastNoise terrainNoise;

	terrainNoise.SetNoiseType(FastNoise::PerlinFractal);
	terrainNoise.SetFractalGain((FN_DECIMAL)0.45);
	terrainNoise.SetFractalOctaves(10);

	terrain->SetNoise(terrainNoise);
	terrain->SetNoiseScale(0.25f);
	terrain->SetPeakHeight(300.f);

	plane->SetPosition({ 0, 128, -1024 });
	plane->Add(mainCam);

	mainCam->SetPosition({ 0, 20.f, -50.f });
	mainCam->SetMain();

	SCENE->Add(terrain);
	SCENE->Add(plane);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
