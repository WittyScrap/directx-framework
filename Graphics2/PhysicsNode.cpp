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

	for (PlanetNode* planet : PlanetNode::GetAllPlanets())
	{
		if (planet->PointInSOI(GetWorldPosition()))
		{
			_linearVelocity += GetCollisionForce(planet);
		}
	}

	Vector3 position = GetPosition();
	position += _linearVelocity * FRAMEWORK->GetDeltaTime();
	SetPosition(position);

	RotateAround(GetForwardVector(), _angularVelocity.Z * FRAMEWORK->GetDeltaTime());
	RotateAround(GetRightVector(), _angularVelocity.X * FRAMEWORK->GetDeltaTime());
	RotateAround(GetUpVector(), _angularVelocity.Y * FRAMEWORK->GetDeltaTime());

	SceneGraph::Update();
}

const Vector3 PhysicsNode::GetCollisionForce(const PlanetNode* planet)
{
	const Vector3& worldPosition = GetWorldPosition();
	const Vector3& planetWorldPosition = planet->GetWorldPosition();
	const Vector3& planetSurface = (worldPosition - planetWorldPosition).Normalized() * planet->GetRadius();

	if ((planetWorldPosition - worldPosition).SqrLength() < (planet->GetRadius() * planet->GetRadius()))
	{
		return -_linearVelocity;

		FLOAT collisionDistance = (planetSurface - worldPosition).Length();
		FLOAT velocitySquared = _linearVelocity.SqrLength();
		Vector3 impactNormal = (planetWorldPosition - worldPosition).Normalized();

		return -impactNormal * ((.5f * _mass * velocitySquared) / collisionDistance);
	}
	else
	{
		return Vector3::ZeroVector;
	}
}
