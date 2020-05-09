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
	time_t t{ 0 };
	srand((unsigned int)time(&t));

	shared_ptr<AmbientLight> ambientLight = AddLight<AmbientLight>();
	shared_ptr<DirectionalLight> directionalLight = AddLight<DirectionalLight>();

	directionalLight->SetDirection({ -1, 0, 0 });
	ambientLight->SetColor({ .25f, .25f, .25f, .25f });

	shared_ptr<BorealisNode> borealis = SceneGraph::Create<BorealisNode>();
	shared_ptr<PlanetNode> planetA = PlanetNode::GenerateRandom(2.f);
	shared_ptr<PlanetNode> planetB = PlanetNode::GenerateRandom();

	borealis->SetPosition({ 0, 200, 1024 });
	planetA->SetPosition({ -3000, 2, 2048 });
	planetB->SetPosition({ 512, 200, 1024 });

	planetA->SetHasAtmosphere(false);
	planetA->SetRadius(128.f);
	planetA->SetGrassColor(.7f, .71f, .75f);
	planetA->SetSandColor(.75f, .75f, .75f);
	planetA->Orbit(planetB.get());

	// Set borealis in orbit
	Vector3 borealisToPlanet = planetB->GetWorldPosition() - borealis->GetWorldPosition();
	Vector3 tangentVector = Vector3::Cross(planetB->GetUpVector(), borealisToPlanet).Normalized();
	borealis->SetLinearVelocity(tangentVector * planetB->GetOrbitalVelocity(borealisToPlanet.Length()));

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
