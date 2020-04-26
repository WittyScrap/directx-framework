#include "PlaneNode.h"

#define lerp(__from, __to, __alpha) (__from + __alpha * (__to - __from))

bool PlaneNode::Initialise()
{
	shared_ptr<Mesh> planeMesh = RESOURCES->GetMesh(L"Plane/Bonanza.3ds");
	shared_ptr<MeshNode> plane = SceneGraph::Create<MeshNode>(L"Plane Geometry");

	_planeController = SceneGraph::Create<SceneGraph>(L"Plane Controller");

	plane->Build(planeMesh);

	Add(_planeController);
	_planeController->Add(plane);

	plane->SetRotation({ 90.f, 0, 180.f });
	plane->SetPosition({ 0.f, -5.f, 0.f });

	return SceneGraph::Initialise();
}

void PlaneNode::Update(FXMMATRIX& m)
{
	Vector3 yawController(GetRotation());
	Vector3 rollController(_planeController->GetRotation());

	float rightInput = (float)GetKey('A');
	float leftInput = (float)GetKey('D');

	float downInput = (float)GetKey('W');
	float upInput = (float)GetKey('S');

	float yawingAngle = rightInput * -TurningSpeed + leftInput * TurningSpeed;
	float bankingAngle = rightInput * BankingAngle + leftInput * -BankingAngle;
	float pitchingAngle = downInput * PitchingAngle + upInput * -PitchingAngle;

	_turningAmount = lerp(_turningAmount, yawingAngle, .01f);

	float y = yawController.Y;
	float x = yawController.X;

	y += _turningAmount;
	x += downInput * TurningSpeed + upInput * -TurningSpeed;

	rollController.SetZ(lerp(rollController.Z, bankingAngle, .01f));
	rollController.SetX(lerp(rollController.X, pitchingAngle, .01f));

	yawController.SetY(y);
	yawController.SetX(x);

	SetRotation(yawController);
	_planeController->SetRotation(rollController);

	Vector3 location(GetPosition());
	Vector3 forward = GetForwardVector();

	forward *= MovementSpeed;
	location += forward;

	SetPosition(location);

	SceneGraph::Update(m);
}
