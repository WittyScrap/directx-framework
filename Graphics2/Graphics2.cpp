#include "Graphics2.h"
#include "RobotNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	shared_ptr<RobotNode> robot = SceneGraph::Create<RobotNode>(L"Robot");

	robot->SetPosition({ 0, 0, -30 });
	sceneGraph->Add(robot);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
