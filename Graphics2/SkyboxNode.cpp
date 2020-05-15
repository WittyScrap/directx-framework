#include "SkyboxNode.h"

bool SkyboxNode::Initialise()
{
	shared_ptr<Mesh> invertedCube = RESOURCES->GetMesh(L"Models/Miscellaneous/skybox.fbx");
	_material = SceneGraph::Create<Material>(L"SkyboxMat");
	_material->SetShader(L"Shaders/skybox.hlsl");
	_material->SetTexture(0, RESOURCES->GetTexture(L"Models/Miscellaneous/skybox.png"));
	_material->SetIsBackground(true);

	Build(invertedCube);
	SetMaterial(0, _material);

	return MeshNode::Initialise();
}

void SkyboxNode::Update()
{
	SetPosition(_linkedCamera->GetWorldPosition());
	MeshNode::Update();
}
