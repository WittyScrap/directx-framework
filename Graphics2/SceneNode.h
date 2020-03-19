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
	virtual void Update(FXMMATRIX& currentWorldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, GetTRS(TRS) * currentWorldTransformation); }
	virtual void Render() = 0;
	virtual void Shutdown() = 0;
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

	void SetPosition(const Vector3& position) { _position = position; }
	void SetRotation(const Vector3& rotation) { _rotation = rotation; }
	void SetScale(const Vector3& scale) { _scale = scale; }

	const Vector3& GetPosition() const { return _position; }
	const Vector3& GetRotation() const { return _rotation; }
	const Vector3& GetScale()	 const { return _scale; }

	Vector3& GetPosition() { return _position; }
	Vector3& GetRotation() { return _rotation; }
	Vector3& GetScale()	   { return _scale; }

protected:
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