#include "Graphics2.h"
#include "CubeNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

#define Graph SceneGraphPointer
	
	// Construct graph skeleton...
	Graph robot			= SceneGraph::Create<SceneGraph>(L"Robot");
	Graph bodyRoot		= SceneGraph::Create<SceneGraph>(L"Body");
	Graph headRoot		= SceneGraph::Create<SceneGraph>(L"Head");
	Graph leftArmRoot	= SceneGraph::Create<SceneGraph>(L"Left Arm");
	Graph rightArmRoot	= SceneGraph::Create<SceneGraph>(L"Right Arm");
	Graph leftLegRoot	= SceneGraph::Create<SceneGraph>(L"Left Leg");
	Graph rightLegRoot	= SceneGraph::Create<SceneGraph>(L"Right Leg");

	    bodyRoot->SetScale({ 5, 8, 2.5f });
	 leftLegRoot->SetScale({ 1, 7.5f, 1 });
	rightLegRoot->SetScale({ 1, 7.5f, 1 });
	 leftArmRoot->SetScale({ 1, 8.5f, 1 });
	rightArmRoot->SetScale({ 1, 8.5f, 1 });
	    headRoot->SetScale({ 3, 3, 3 });

		bodyRoot->SetPosition({ 0, 23, 0 });
	 leftLegRoot->SetPosition({ -4, 7.5f, 0 });
	rightLegRoot->SetPosition({ 4, 7.5f, 0 });
	 leftArmRoot->SetPosition({ -6, 22, 0 });
	rightArmRoot->SetPosition({ 6, 22, 0 });
		headRoot->SetPosition({ 0, 34, 0 });

		   robot->Add(bodyRoot);
		   robot->Add(leftArmRoot);
		   robot->Add(rightArmRoot);
		   robot->Add(leftLegRoot);
		   robot->Add(rightLegRoot);
		   robot->Add(headRoot);

#undef Graph
#define PMesh		shared_ptr<Mesh>
#define PMaterial	shared_ptr<Material>

	// Construct all shared resources...
	PMesh		cubeBox		= make_shared<Mesh>(CubeNode::GetVertices(), CubeNode::GetIndices());
	PMaterial	bricks		= make_shared<Material>(L"shader.hlsl"); bricks->SetTexture(L"Bricks.png");
	PMaterial	concrete	= make_shared<Material>(L"shader.hlsl"); concrete->SetTexture(L"Concrete.png");
	PMaterial	wood		= make_shared<Material>(L"shader.hlsl"); wood->SetTexture(L"Wood.png");

	cubeBox->Apply();

#undef PMesh
#undef PMaterial
#define PMeshNode	shared_ptr<MeshNode>

	// Construct all mesh nodes...
	PMeshNode body		= SceneGraph::Create<MeshNode>(L"Body Geometry");
	PMeshNode leftArm	= SceneGraph::Create<MeshNode>(L"Left Arm Geometry");
	PMeshNode rightArm	= SceneGraph::Create<MeshNode>(L"Right Arm Geometry");
	PMeshNode leftLeg	= SceneGraph::Create<MeshNode>(L"Left Leg Geometry");
	PMeshNode rightLeg	= SceneGraph::Create<MeshNode>(L"Right Leg Geometry");
	PMeshNode head		= SceneGraph::Create<MeshNode>(L"Head Geometry");

	    body->SetMaterial(wood);
	 leftArm->SetMaterial(concrete);
	rightArm->SetMaterial(concrete);
	 leftLeg->SetMaterial(concrete);
	rightLeg->SetMaterial(concrete);
	    head->SetMaterial(bricks);

		body->SetMesh(cubeBox);
	 leftArm->SetMesh(cubeBox);
	rightArm->SetMesh(cubeBox);
	 leftLeg->SetMesh(cubeBox);
	rightLeg->SetMesh(cubeBox);
	    head->SetMesh(cubeBox);

#undef PMeshNode

	// Attach all nodes...

		bodyRoot->Add(body);
	 leftArmRoot->Add(leftArm);
	rightArmRoot->Add(rightArm);
	 leftLegRoot->Add(leftLeg);
	rightLegRoot->Add(rightLeg);
		headRoot->Add(head);
	
	// Add them to the scene graph...
	  sceneGraph->Add(robot);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
