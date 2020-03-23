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
	shared_ptr<RobotNode> robot = SceneGraph::Create<RobotNode>(L"Beep boop");
	shared_ptr<CameraNode> camera = SceneGraph::Create<CameraNode>(L"Main Camera");

	plane->Add(camera);
	camera->SetPosition({ 0, 0, -50 });
	camera->SetMain();
	robot->SetPosition({ 0, -25, 75 });

	sceneGraph->Add(plane);
	sceneGraph->Add(robot);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
