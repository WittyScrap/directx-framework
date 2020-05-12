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

void PhysicsNode::Update()
{
	if (b_simulateGravity)
	{
		_linearVelocity += PlanetNode::CalculateTotalGravity(GetWorldPosition(), _mass) * FRAMEWORK->GetDeltaTime();
	}

	Vector3 position = GetPosition();
	position += _linearVelocity * FRAMEWORK->GetDeltaTime();
	SetPosition(position);

	Vector3 depenetrationOffset;

	for (PlanetNode* planet : PlanetNode::GetAllPlanets())
	{
		if (planet->PointInSOI(GetWorldPosition()))
		{
			depenetrationOffset += GetCollisionOffset(planet);
		}
	}

	position += depenetrationOffset;
	SetPosition(position);

	if (depenetrationOffset.SqrLength() > 0)
	{
		_linearVelocity += depenetrationOffset / FRAMEWORK->GetDeltaTime();
	}

	RotateAround(GetForwardVector(), _angularVelocity.Z * FRAMEWORK->GetDeltaTime());
	RotateAround(GetRightVector(), _angularVelocity.X * FRAMEWORK->GetDeltaTime());
	RotateAround(GetUpVector(), _angularVelocity.Y * FRAMEWORK->GetDeltaTime());

	SceneGraph::Update();
}

const Vector3 PhysicsNode::GetCollisionOffset(const PlanetNode* planet)
{
	const Vector3& worldPosition = GetWorldPosition();
	const Vector3& planetWorldPosition = planet->GetWorldPosition();
	const FLOAT sqrRadius = planet->GetRadius() * planet->GetRadius();
	const Vector3& planetToObject = worldPosition - planetWorldPosition;

	if ((planetWorldPosition - worldPosition).SqrLength() < sqrRadius)
	{
		const Vector3  r = planetToObject.Normalized() * planet->GetRadius();
		const Vector3& p = planetToObject;

		return r - p;
	}
	else
	{
		return Vector3::ZeroVector;
	}
}