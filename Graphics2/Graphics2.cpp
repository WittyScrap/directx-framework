#include "Graphics2.h"
#include "RobotNode.h"
#include "PlaneNode.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "CameraNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 1, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	SceneGraphPointer sceneGraph = GetSceneGraph();
	shared_ptr<PlaneNode> plane = SceneGraph::Create<PlaneNode>(L"Neaoww");
	shared_ptr<CameraNode> camera = SceneGraph::Create<CameraNode>(L"Main Camera");

	camera->SetPosition({ 0, 0, -50 });
	camera->SetMain();

	sceneGraph->Add(plane);
	sceneGraph->Add(camera);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
