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
	if (GetForegroundWindow() == FRAMEWORK->GetHWnd())
	{
		AddForce(
			GetRightVector() * (_accelerationSpeed * (GetKey('D') - GetKey('A'))) +
			GetUpVector() * (_accelerationSpeed * (GetKey('R') - GetKey('F'))) +
			GetForwardVector() * (_accelerationSpeed * (GetKey('W') - GetKey('S')))
		);

		RotateAround(GetUpVector(), GetMouseHorizontal() * _rotationSpeed);
		RotateAround(GetRightVector(), GetMouseVertical() * _rotationSpeed);
		RotateAround(GetForwardVector(), (GetKey('Q') - GetKey('E')) * _rotationSpeed * 0.1f);

		DoCameraSway();

		if (!_mouseLocked && GetKey(VK_MBUTTON))
		{
			SetMouseLocked(true);
			SetMouseVisible(false);
		}
	}

	PhysicsNode::Update(m);
}

void BorealisNode::DoCameraSway()
{
	Vector3 velocity = GetLinearVelocity();
	float length = velocity.Length();

	if (length > _cameraSwayStrength)
	{
		velocity /= length;
		velocity *= _cameraSwayStrength;
	}

	velocity *= -1;
	_time += _cameraBobSpeed;
	float bobValue = static_cast<float>(sin(_time)) * _cameraBobStrength;

	velocity.SetY(velocity.Y + bobValue);

	_camera->SetPosition(_pilotPosition + velocity);
}
