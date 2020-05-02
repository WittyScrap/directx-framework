#include "CameraNode.h"
#include "DirectXFramework.h"

CameraNode* CameraNode::_mainCamera{ nullptr };

bool CameraNode::Initialise()
{
	UpdateMatrices();

	return true;
}

void CameraNode::Update(FXMMATRIX& m)
{
	SceneGraph::Update(m);

	XMMATRIX matrix = XMLoadFloat4x4(&_combinedWorldTransformation);
	XMVECTOR determinant = XMMatrixDeterminant(matrix);
	XMMATRIX transform = XMMatrixInverse(&determinant, matrix);

	/*XMFLOAT4 up(transform._12, transform._22, transform._32, 1);
	XMFLOAT4 forward(transform._13, transform._23, transform._33, 1);
	
	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat4(&GetPosition().ToDX()),
		XMLoadFloat4(&forward),
		XMLoadFloat4(&up)
	);*/

	// XMStoreFloat4x4(&_viewTransformation, transform);
}

XMMATRIX CameraNode::GetViewTransformation() const
{
	return XMMatrixLookToLH(
		XMVectorSet(XYZ(GetPosition()), 0),
		XMVectorSet(XYZ(GetForwardVector()), 0),
		XMVectorSet(XYZ(GetUpVector()), 0)
	);
}

XMMATRIX CameraNode::GetProjectionTransformation() const
{
	return XMLoadFloat4x4(&_projectionTransformation);
}

void CameraNode::UpdateMatrices()
{
	XMStoreFloat4x4(&_projectionTransformation,
		XMMatrixPerspectiveFovLH(
			_fieldOfView * D2R,
			(float)FRAMEWORK->GetWindowWidth() / FRAMEWORK->GetWindowHeight(),
			_nearClipPlane,
			_farClipPlane
		));
}

CameraNode* const CameraNode::GetMain()
{
	return _mainCamera;
}

void CameraNode::SetMain()
{
	_mainCamera = this;
}

void CameraNode::SetNearClipPlane(FLOAT ncp)
{
	_nearClipPlane = ncp;
	UpdateMatrices();
}

void CameraNode::SetFarClipPlane(FLOAT fcp)
{
	_farClipPlane = fcp;
	UpdateMatrices();
}

void CameraNode::SetFieldOfView(FLOAT fov)
{
	_fieldOfView = fov;
	UpdateMatrices();
}
