#include "Graphics2.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PlanetNode.h"
#include "BorealisNode.h"
#include "SatelliteNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	time_t t{ 0 };
	srand((unsigned int)time(&t));

	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 0, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<BorealisNode> borealis = SceneGraph::Create<BorealisNode>();
	shared_ptr<PlanetNode> planetA = PlanetNode::GenerateRandom();
	shared_ptr<PlanetNode> planetB = PlanetNode::GenerateRandom(.5f);
	shared_ptr<SatelliteNode> satellite = SceneGraph::Create<SatelliteNode>(L"Satellite");

	borealis->SetPosition({ 0, 200, 512 });
	planetA->SetPosition({ -3000, 2, 2048 });
	planetB->SetPosition({ 512, 200, 1024 });
	satellite->SetPosition({ -3500, 2, 2048 });

	planetB->SetRadius(512.f);
	planetB->SetScatterMesh(RESOURCES->GetMesh(L"Models/Scatters/Tree/tree.fbx"));
	planetA->SetHasAtmosphere(false);
	planetA->SetGrassColor(.7f, .71f, .75f);
	planetA->SetSandColor(.75f, .75f, .75f);

	planetA->Orbit(planetB.get());
	borealis->Orbit(planetB.get());
	satellite->Orbit(planetA.get());

	SCENE->Add(planetA);
	SCENE->Add(planetB);
	SCENE->Add(borealis);
	SCENE->Add(satellite);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph.
}
