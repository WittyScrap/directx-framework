#include "Graphics2.h"
#include "RobotNode.h"
#include "PlaneNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	shared_ptr<PlaneNode> plane = SceneGraph::Create<PlaneNode>(L"Neaoww");

	sceneGraph->Add(plane);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
