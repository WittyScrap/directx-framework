#pragma once
#include "core.h"
#include "DirectXCore.h"
#include "Vector3.h"

#define PI		3.14159265359f
#define D2R		(PI / 180.f)
#define RPY(v)	v.X * D2R, v.Y * D2R, v.Z * D2R
#define XYZ(v)	v.X, v.Y, v.Z
#define TRS		_position, _rotation, _scale


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
	virtual bool Initialise() = 0;
	virtual void Update(FXMMATRIX& currentWorldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, GetTRS(TRS) * currentWorldTransformation); ResetMouse(); }
	virtual void Render() = 0;
	virtual void Shutdown() = 0;
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

	virtual void SetPosition(const Vector3& position) { _position = position; }
	virtual void SetRotation(const Vector3& rotation) { _rotation = rotation; }
	virtual void SetScale(const Vector3& scale) { _scale = scale; }

	const Vector3& GetPosition() const { return _position; }
	const Vector3& GetRotation() const { return _rotation; }
	const Vector3& GetScale()	 const { return _scale; }

#define TRANSFORM _combinedWorldTransformation

	const Vector3 GetForwardVector()		const { return Vector3(-TRANSFORM._13, -TRANSFORM._23, TRANSFORM._33).Normalized(); }
	const Vector3 GetUpVector()				const { return Vector3(-TRANSFORM._12, -TRANSFORM._22, TRANSFORM._32).Normalized(); }
	const Vector3 GetRightVector()			const { return Vector3::Cross(GetForwardVector(), GetUpVector()); }

#undef TRANSFORM

	const XMMATRIX GetTranslationMatrix() { return XMMatrixTranslation(XYZ(_position)); }
	const XMMATRIX GetRotationMatrix()	  { return XMMatrixRotationRollPitchYaw(RPY(_rotation)); }
	const XMMATRIX GetScaleMatrix()		  { return XMMatrixScaling(XYZ(_scale)); }

	const int GetKey(const int& keyCode);
	const int GetKeyDown(const int& keyCode);

	const float GetMouseHorizontal();
	const float GetMouseVertical();

	void SetMouseLocked(const BOOL& value)		{ _mouseLocked = value; }

private:
	void ResetMouse();

protected:
	XMFLOAT4X4			_combinedWorldTransformation;
	wstring				_name;

	Vector3				_position{ 0, 0, 0 };
	Vector3				_rotation{ 0, 0, 0 };
	Vector3				_scale{ 1, 1, 1 };

	BOOL				_mouseLocked{ false };
	FLOAT				_mouseHorizontalAxis{ 0 };
	FLOAT				_mouseVerticalAxis{ 0 };

public:
	static XMMATRIX GetTRS(const Vector3& t, const Vector3& r, const Vector3& s) { return XMMatrixScaling(XYZ(s)) * XMMatrixRotationRollPitchYaw(RPY(r)) * XMMatrixTranslation(XYZ(t)); }
};