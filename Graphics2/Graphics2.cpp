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

	directionalLight->SetDirection({ -1, 1, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<PlanetNode> planet = SceneGraph::Create<PlanetNode>(L"Terrain");
	shared_ptr<PawnNode> mainPawn = SceneGraph::Create<PawnNode>();

	planet->SetDrawMode(MeshMode::TriangleList);
	planet->SetMode(TerrainMode::Procedural);

	planet->SetNoiseScale(2.f);
	planet->SetPeakHeight(1.f);

	mainPawn->SetPosition({ 0, 128, -1024 });
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
