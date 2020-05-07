#include "BorealisNode.h"

bool BorealisNode::Initialise()
{
	_borealisModel = RESOURCES->LoadModelFromFile(L"Models/Borealis/borealis.fbx");

	if (_borealisModel)
	{
		_borealisMeshNode = SceneGraph::Create<MeshNode>(L"Borealis");
		_borealisMeshNode->Build(_borealisModel);
		_borealisMeshNode->SetPosition({ 0, 5.f, 0 });

		Add(_borealisMeshNode);

		_camera = SceneGraph::Create<CameraNode>(L"Main Camera");
		_camera->SetMain();
		_camera->SetPosition(_pilotPosition + _borealisMeshNode->GetPosition());

		Add(_camera);
	}

	SetMouseLocked(true);
	SetMouseVisible(false);

	return _borealisModel != nullptr && PhysicsNode::Initialise();
}

void BorealisNode::Update(FXMMATRIX& m)
{
	AddForce(
		GetRightVector() * (_accelerationSpeed * (GetKey('D') - GetKey('A'))) +
		GetUpVector() * (_accelerationSpeed * (GetKey('R') - GetKey('F'))) +
		GetForwardVector() * (_accelerationSpeed * (GetKey('W') - GetKey('S')))
	);

	RotateAround(GetUpVector(), GetMouseHorizontal() * _rotationSpeed);
	RotateAround(GetRightVector(), GetMouseVertical() * _rotationSpeed);
	RotateAround(GetForwardVector(), (GetKey('Q') - GetKey('E')) * _rotationSpeed * 0.1f);

	PhysicsNode::Update(m);
}
