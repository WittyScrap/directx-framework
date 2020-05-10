#include "PhysicsNode.h"
#include "PlanetNode.h"

void PhysicsNode::AddForce(const Vector3& force)
{
	_linearVelocity += force;
}

void PhysicsNode::AddSpin(const Vector3& spin)
{
	_angularVelocity += spin;
}

const Vector3 PhysicsNode::GetLinearVelocity() const
{
	return _linearVelocity;
}

const Vector3 PhysicsNode::GetAngularVelocity() const
{
	return _angularVelocity;
}

void PhysicsNode::SetLinearVelocity(const Vector3& velocity)
{
	_linearVelocity = velocity;
}

void PhysicsNode::SetAngularVelocity(const Vector3& angularVelocity)
{
	_angularVelocity = angularVelocity;
}

void PhysicsNode::Update(FXMMATRIX& m)
{
	if (b_simulateGravity)
	{
		_linearVelocity += PlanetNode::CalculateTotalGravity(GetWorldPosition(), 1) * FRAMEWORK->GetDeltaTime();
	}

	Vector3 position = GetPosition();
	position += _linearVelocity * FRAMEWORK->GetDeltaTime();
	SetPosition(position);

	RotateAround(GetForwardVector(), _angularVelocity.Z * FRAMEWORK->GetDeltaTime());
	RotateAround(GetRightVector(), _angularVelocity.X * FRAMEWORK->GetDeltaTime());
	RotateAround(GetUpVector(), _angularVelocity.Y * FRAMEWORK->GetDeltaTime());

	SceneGraph::Update(m);
}
