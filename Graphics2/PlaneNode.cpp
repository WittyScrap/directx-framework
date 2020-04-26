#include "PlaneNode.h"

bool PlaneNode::Initialise()
{
	shared_ptr<Mesh> planeMesh = RESOURCES->GetMesh(L"Plane/Bonanza.3ds");
	shared_ptr<MeshNode> plane = SceneGraph::Create<MeshNode>(L"Plane Geometry");

	plane->Build(planeMesh);
	Add(plane);

	plane->SetRotation({ 90.f, 0, 180.f });
	plane->SetPosition({ 0.f, -5.f, 0.f });

	return SceneGraph::Initialise();
}

void PlaneNode::Update(FXMMATRIX& m)
{
	Vector3 rotation(GetRotation());

	float rightMotion = GetKey('A') * -TurningSpeed;
	float leftMotion = GetKey('D') * TurningSpeed;

	float y = rotation.GetY();
	y += rightMotion + leftMotion;
	rotation.SetY(y);

	SetRotation(rotation);

	Vector3 location(GetPosition());
	Vector3 forward = GetForwardVector();

	forward *= MovementSpeed;
	location += forward;

	SetPosition(location);

	SceneGraph::Update(m);
}
