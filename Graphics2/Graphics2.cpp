#include "Graphics2.h"
#include "CubeNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	
	// This is where you add nodes to the scene graph
	sceneGraph->Add(SceneNode::Create<CubeNode>());
	shared_ptr<CubeNode> cube1 = make_shared<CubeNode>();
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
