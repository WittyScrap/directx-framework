#pragma once
#include "core.h"
#include "DirectXCore.h"
#include "Vector3.h"

#define PI		3.14159265359f
#define D2R		(PI / 180.f)
#define RPY(v)	v.X * D2R, v.Y * D2R, v.Z * D2R
#define XYZ(v)	v.X, v.Y, v.Z
#define TRS		(_scale * XMMatrixRotationQuaternion(_rotation) * _position)
#define POS(m)	m._41, m._42, m._43


using namespace std;

// Abstract base class for all nodes of the scene graph.  
// This scene graph implements the Composite Design Pattern

class SceneNode;

typedef shared_ptr<SceneNode>	SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) { _name = name; };
	~SceneNode(void) {};

	// Core methods
	virtual bool			Initialise()															= 0;
	virtual void			Update(FXMMATRIX& currentWorldTransformation)							{ XMStoreFloat4x4(&_combinedWorldTransformation, TRS * currentWorldTransformation); ResetMouse(); }
	virtual void			Render()																= 0;
	virtual void			Shutdown()																= 0;
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void			Add(SceneNodePointer node)												{}
	virtual void			Remove(SceneNodePointer node)											{};
	virtual	SceneNodePointer Find(wstring name)														{ return (_name == name) ? shared_from_this() : nullptr; }

	virtual void			SetPosition(const Vector3& position)									{ _position = XMMatrixTranslation(XYZ(position)); }
	virtual void			SetRotation(const Vector3& rotation)									{ _rotation = XMQuaternionRotationRollPitchYaw(RPY(rotation)); }
	virtual void			SetRotation(const XMVECTOR& quat)										{ _rotation = quat; }
	virtual void			SetRotation(const XMMATRIX& rot)										{ _rotation = XMQuaternionRotationMatrix(rot); }
	virtual void			SetRotation(const Vector3& forward, const Vector3& up)					;
	virtual void			SetScale(const Vector3& scale)											{ _scale = XMMatrixScaling(XYZ(scale)); }

	virtual void			RotateAround(const Vector3& axis, const float& angle);

			const Vector3	GetPosition() const														{ XMFLOAT4X4 m; XMStoreFloat4x4(&m, _position); return { m._41, m._42, m._43 }; }
			const XMVECTOR	GetRotation() const														{ return _rotation; }
			const Vector3	GetScale() const														{ XMFLOAT4X4 m; XMStoreFloat4x4(&m, _position); return { m._11, m._22, m._33 }; }

			const XMMATRIX	GetWorldMatrix() const;

			const Vector3	GetWorldPosition() const;
			const XMVECTOR	GetWorldRotation() const;
			const Vector3	GetWorldScale() const;

			const Vector3	GetForwardVector() const;
			const Vector3	GetUpVector() const;
			const Vector3	GetRightVector() const													{ return Vector3::Cross(GetUpVector(), GetForwardVector()); }

			const int		GetKey(const int& keyCode);
			const int		GetKeyDown(const int& keyCode);

			const float		GetMouseHorizontal();
			const float		GetMouseVertical();

			void			SetMouseLocked(const BOOL& value)										{ _mouseLocked = value; }
			void			SetMouseVisible(const BOOL& value)										{ if (value != _mouseVisible) { ShowCursor(value); _mouseVisible = value; } }

			SceneNode*		GetParent() const														{ return _parent; }
			void			SetParent(SceneNode* parent)											{ _parent = parent; }

private:
			void			ResetMouse();

protected:
	XMFLOAT4X4			_combinedWorldTransformation;
	wstring				_name;

	XMMATRIX			_position = XMMatrixIdentity();
	XMVECTOR			_rotation = XMQuaternionIdentity();
	XMMATRIX			_scale = XMMatrixIdentity();

	BOOL				_mouseLocked{ false };
	BOOL				_mouseVisible{ true };

	FLOAT				_mouseHorizontalAxis{ 0 };
	FLOAT				_mouseVerticalAxis{ 0 };

	SceneNode*			_parent{ nullptr };

public:
	static XMMATRIX GetTRS(const Vector3& t, const Vector3& r, const Vector3& s)					{ return XMMatrixScaling(XYZ(s)) * XMMatrixRotationRollPitchYaw(RPY(r)) * XMMatrixTranslation(XYZ(t)); }
};