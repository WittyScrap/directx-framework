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
		int horizontal = GetKey('D') - GetKey('A');
		int vertical = GetKey('R') - GetKey('F');
		int forward = GetKey('W') - GetKey('S');

		AddForce(
			GetRightVector() * (_accelerationSpeed * horizontal) +
			GetUpVector() * (_accelerationSpeed * vertical) +
			GetForwardVector() * (_accelerationSpeed * forward)
		);

		float mouseX = GetMouseHorizontal() * _rotationSpeed;
		float mouseY = GetMouseVertical() * _rotationSpeed;
		int roll = GetKey('Q') - GetKey('E');

		AddSpin(Vector3::UpVector * mouseX);
		AddSpin(Vector3::RightVector * mouseY);
		AddSpin(Vector3::ForwardVector * roll * _rotationSpeed * 0.1f);

		DoCameraSway();

		if (!_mouseLocked && GetKey(VK_MBUTTON))
		{
			SetMouseLocked(true);
			SetMouseVisible(false);
		}

		if (GetAngularVelocity().SqrLength() > 0.f)
		{
			if (GetAngularVelocity().SqrLength() > 1.f)
			{
				AddSpin(GetAngularVelocity().Normalized() * -.01f);
			}
			else
			{
				AddSpin(GetAngularVelocity() * -.01f);
			}
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
