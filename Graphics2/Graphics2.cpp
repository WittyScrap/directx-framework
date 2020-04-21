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

	terrain->SetDrawMode(MeshMode::LineList);
	terrain->LoadHeightMap(L"Example_HeightMap.raw");
	terrain->SetMode(TerrainMode::TextureSample);

	mainCam->SetPosition({ 0, 512, 0 });
	mainCam->SetMain();

	SCENE->Add(terrain);
	SCENE->Add(mainCam);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
