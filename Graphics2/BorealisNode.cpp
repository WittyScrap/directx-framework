#include "BorealisNode.h"

bool BorealisNode::Initialise()
{
	_borealisModel = RESOURCES->LoadModelFromFile(L"Models/Borealis/borealis.fbx");

	if (_borealisModel)
	{
		_borealisMeshNode = SceneGraph::Create<MeshNode>(L"Borealis");
		_borealisMeshNode->Build(_borealisModel);

		Add(_borealisMeshNode);

		_camera = SceneGraph::Create<CameraNode>(L"Main Camera");
		_camera->SetMain();
		_camera->SetPosition(_pilotPosition);

		Add(_camera);
	}

	SetMouseLocked(true);
	SetMouseVisible(false);

	return _borealisModel != nullptr && PhysicsNode::Initialise();
}

void BorealisNode::Update(FXMMATRIX& m)
{
	AddForce({
		_accelerationSpeed * (GetKey('D') - GetKey('A')),
		_accelerationSpeed * (GetKey('R') - GetKey('F')),
		_accelerationSpeed * (GetKey('W') - GetKey('S'))
	});

	RotateAround(GetUpVector(), GetMouseHorizontal() * _rotationSpeed);
	RotateAround(GetRightVector(), GetMouseVertical() * _rotationSpeed);
	RotateAround(GetForwardVector(), (GetKey('E') - GetKey('Q')) * _rotationSpeed);

	PhysicsNode::Update(m);
}
