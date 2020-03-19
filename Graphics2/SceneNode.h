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
	SceneNode(wstring name) {_name = name; XMStoreFloat4x4(&_worldTransformation, XMMatrixIdentity()); };
	~SceneNode(void) {};

	// Core methods
	virtual bool Initialise() = 0;
	virtual void Update(FXMMATRIX& currentWorldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, XMLoadFloat4x4(&_worldTransformation) * GetTRS(TRS) * currentWorldTransformation); }
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	void SetWorldTransform(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_worldTransformation, worldTransformation); }
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

	void SetPosition(const Vector3& position) { _position = position; }
	void SetRotation(const Vector3& rotation) { _rotation = rotation; }
	void SetScale(const Vector3& scale) { _scale = scale; }

protected:
	XMFLOAT4X4			_worldTransformation;
	XMFLOAT4X4			_combinedWorldTransformation;
	wstring				_name;

	Vector3				_position;
	Vector3				_rotation;
	Vector3				_scale{ 1, 1, 1 };

public:
	static XMMATRIX GetTRS(const Vector3& t, const Vector3& r, const Vector3& s) { return XMMatrixScaling(XYZ(s)) * XMMatrixRotationRollPitchYaw(RPY(r)) * XMMatrixTranslation(XYZ(t)); }
};

// Remove contextual #define macros, as they can almost certainly
// not be used as they are intended in any other situation.
#undef RPY
#undef XYZ
#undef TRS