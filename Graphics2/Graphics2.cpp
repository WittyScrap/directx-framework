#include "Graphics2.h"
#include "RobotNode.h"
#include "PlaneNode.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PlanetNode.h"
#include "CameraNode.h"
#include "PawnNode.h"
#include "BorealisNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 0, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<BorealisNode> borealis = SceneGraph::Create<BorealisNode>();
	shared_ptr<PlanetNode> planetA = PlanetNode::GenerateRandom();
	shared_ptr<PlanetNode> planetB = PlanetNode::GenerateRandom();

	borealis->SetPosition({ 0, 128, -1024 });
	planetA->SetPosition({ -5, 2, 2048 });
	planetB->SetPosition({ 512, 200, 1024 });
	planetB->SetRadius(512.f);

	SCENE->Add(planetA);
	SCENE->Add(planetB);
	SCENE->Add(borealis);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
