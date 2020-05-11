#include "BorealisNode.h"
#include "SkyboxNode.h"
#define lerp(a, b, t) ((a) + (t) * ((b) - (a)))

using namespace std;

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

		auto skybox = SceneGraph::Create<SkyboxNode>(L"Skybox");
		skybox->Link(_camera);
		SCENE->Add(skybox);
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

		RegisterThrust(horizontal, vertical, forward);

		float mouseX = GetMouseHorizontal();
		float mouseY = GetMouseVertical();
		int roll = GetKey('Q') - GetKey('E');

		if (GetKeyDown(VK_MBUTTON))
		{
			b_headlookActive = !b_headlookActive;

			if (!b_headlookActive)
			{
				_camera->SetRotation(XMQuaternionIdentity());
			}
		}

		if (!b_headlookActive)
		{
			AddSpin(Vector3::UpVector * mouseX * _rotationSpeed);
			AddSpin(Vector3::RightVector * mouseY * _rotationSpeed);
			AddSpin(Vector3::ForwardVector * static_cast<float>(roll)* _rotationSpeed * 0.1f);
		}
		else
		{
			_camera->RotateAround(Vector3::UpVector, mouseX * _cameraHeadlookSensitivity * FRAMEWORK->GetDeltaTime());
		}

		DoCameraSway();

		if (!_mouseLocked && GetKey(VK_LBUTTON))
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
	Vector3 velocity = _lastThrust;
	float length = velocity.Length();

	velocity *= -_cameraSwayStrength;
	_time += _cameraBobSpeed;
	_time = fmod(_time, 2.f * PI);
	float bobValue = static_cast<float>(sin(_time)) * _cameraBobStrength;

	velocity.SetY(velocity.Y + bobValue);

	_camera->SetPosition(_pilotPosition + velocity);
}

void BorealisNode::RegisterThrust(const int& x, const int& y, const int& z)
{
	const float& deltaTime = FRAMEWORK->GetDeltaTime();

	_lastThrust.SetX(lerp(_lastThrust.X, x, _cameraSwaySmoothness));
	_lastThrust.SetY(lerp(_lastThrust.Y, y, _cameraSwaySmoothness));
	_lastThrust.SetZ(lerp(_lastThrust.Z, z, _cameraSwaySmoothness));
}
