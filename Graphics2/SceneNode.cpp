#include "SceneNode.h"
#include <WinUser.h>
#include "DirectXFramework.h"

void SceneNode::SetRotation(const Vector3& forward, const Vector3& up)
{
	XMVECTOR zeroPosition = XMVectorSet(0, 0, 0, 0);

	XMVECTOR fwdDirection = XMVectorSet(XYZ(forward), 0);
	XMVECTOR  upDirection = XMVectorSet(XYZ(up), 0);

	XMMATRIX lookAtMatrix = XMMatrixLookToLH(zeroPosition, fwdDirection, upDirection);
	XMVECTOR quaternion = XMQuaternionRotationMatrix(lookAtMatrix);

	_rotation = quaternion;
}

void SceneNode::RotateAround(const Vector3& axis, const float& angle)
{
	XMVECTOR axisVecSD = XMVectorSet(XYZ(axis), 0);
	XMVECTOR transform = XMQuaternionRotationAxis(axisVecSD, angle * D2R);

	_rotation = XMQuaternionMultiply(_rotation, transform);
}

const XMMATRIX SceneNode::GetWorldMatrix() const
{
	XMMATRIX transform = _scale * XMMatrixRotationQuaternion(_rotation) * _position;

	if (_parent)
	{
		transform *= _parent->GetWorldMatrix();
	}

	return transform;
}

const Vector3 SceneNode::GetWorldPosition() const
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, GetWorldMatrix());

	return { POS(matrix) };
}

const XMVECTOR SceneNode::GetWorldRotation() const
{
	XMVECTOR rotation = GetRotation();

	if (_parent)
	{
		rotation = XMQuaternionMultiply(rotation, _parent->GetWorldRotation());
	}

	return rotation;
}

const Vector3 SceneNode::GetWorldScale() const
{
	Vector3 scale = GetScale();

	if (_parent)
	{
		scale *= _parent->GetWorldScale();
	}

	return scale;
}

const Vector3 SceneNode::GetForwardVector() const
{
	XMVECTOR v = XMVectorSet(XYZ(Vector3::ForwardVector), 0);
	XMFLOAT3 out;

	XMVECTOR o = XMVector3TransformNormal(v, XMMatrixRotationQuaternion(GetWorldRotation()));

	XMStoreFloat3(&out, o);
	return Vector3(out.x, out.y, out.z);
}

const Vector3 SceneNode::GetUpVector() const
{
	XMVECTOR v = XMVectorSet(XYZ(Vector3::UpVector), 0);
	XMFLOAT3 out;

	XMVECTOR o = XMVector3TransformNormal(v, XMMatrixRotationQuaternion(GetWorldRotation()));

	XMStoreFloat3(&out, o);
	return Vector3(out.x, out.y, out.z);
}

const int SceneNode::GetKey(const int& keyCode)
{
	return GetAsyncKeyState(keyCode) < 0;
}

const int SceneNode::GetKeyDown(const int& keyCode)
{
	SHORT key = GetAsyncKeyState(keyCode);
	return (key & 1) && key < 0;
}

const float SceneNode::GetMouseHorizontal()
{
	return _mouseHorizontalAxis;
}

const float SceneNode::GetMouseVertical()
{
	return _mouseVerticalAxis;
}

void SceneNode::ResetMouse()
{
	if (_mouseLocked)
	{
		if (GetKey(VK_ESCAPE))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}

		POINT mouseLocation;
		RECT windowRect;

		if (!GetCursorPos(&mouseLocation) || !GetClientRect(FRAMEWORK->GetHWnd(), &windowRect))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}

		// Convert mouse location from screen space
		// to window space.
		mouseLocation.x -= windowRect.left;
		mouseLocation.y -= windowRect.top;

		// Normalize mouse location.
		float windowWidth = (float)windowRect.right - (float)windowRect.left;
		float windowHeight = (float)windowRect.bottom - (float)windowRect.top;

		float normalMouseX = (float)mouseLocation.x / windowWidth;
		float normalMouseY = (float)mouseLocation.y / windowHeight;

		// Convert mouse location from 0 - 1 range to -1 - +1 range
		float clipMouseX = normalMouseX * 2 - 1; // These values now represent the
		float clipMouseY = normalMouseY * 2 - 1; // horizontal and vertical mouse axis.

		_mouseHorizontalAxis = clipMouseX;
		_mouseVerticalAxis = clipMouseY;

		if (!SetCursorPos((windowRect.right + windowRect.left) / 2, (windowRect.top + windowRect.bottom) / 2))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}
	}
	else
	{
		_mouseHorizontalAxis = 0;
		_mouseVerticalAxis = 0;
	}
}
