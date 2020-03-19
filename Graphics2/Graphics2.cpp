#include "Graphics2.h"
#include "CubeNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	
	// Construct all meshes...
	shared_ptr<CubeNode> cubeTest = SceneGraph::Create<CubeNode>(L"Testing Cube");
	shared_ptr<Material> cubeMat = make_shared<Material>(L"shader.hlsl");
	cubeMat->SetTexture(L"Concrete.png");
	cubeTest->SetMaterial(cubeMat);

	
	// Add them to the scene graph...
	sceneGraph->Add(cubeTest);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
