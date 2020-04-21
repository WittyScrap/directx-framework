#include "RobotNode.h"
#include "SceneGraph.h"
#include "MeshNode.h"
#include "CubeNode.h"

bool RobotNode::Initialise()
{
	SceneGraph::Initialise();

	// Construct graph skeleton...
	SceneGraphPointer bodyRoot = SceneGraph::Create<SceneGraph>(L"Body");
	SceneGraphPointer headRoot = SceneGraph::Create<SceneGraph>(L"Head");
	SceneGraphPointer leftArmRoot = SceneGraph::Create<SceneGraph>(L"Left Arm");
	SceneGraphPointer rightArmRoot = SceneGraph::Create<SceneGraph>(L"Right Arm");
	SceneGraphPointer leftLegRoot = SceneGraph::Create<SceneGraph>(L"Left Leg");
	SceneGraphPointer rightLegRoot = SceneGraph::Create<SceneGraph>(L"Right Leg");

	bodyRoot->SetScale({ 5, 8, 2.5f });
	leftLegRoot->SetScale({ 1, 7.5f, 1 });
	rightLegRoot->SetScale({ 1, 7.5f, 1 });
	leftArmRoot->SetScale({ 1, 8.5f, 1 });
	rightArmRoot->SetScale({ 1, 8.5f, 1 });
	headRoot->SetScale({ 3, 3, 3 });

	bodyRoot->SetPosition({ 0, 23, 0 });
	leftLegRoot->SetPosition({ -4, 7.5f, 0 });
	rightLegRoot->SetPosition({ 4, 7.5f, 0 });
	leftArmRoot->SetPosition({ -6, 22, 0 });
	rightArmRoot->SetPosition({ 6, 22, 0 });
	headRoot->SetPosition({ 0, 34, 0 });

	Add(bodyRoot);
	Add(leftArmRoot);
	Add(rightArmRoot);
	Add(leftLegRoot);
	Add(rightLegRoot);
	Add(headRoot);

	// Construct all shared resources...
	shared_ptr<Shader>		shader = make_shared<Shader>(L"shader.hlsl"); shader->CompileOnce();
	shared_ptr<Mesh>		cubeBox = make_shared<Mesh>(CubeNode::GetVertices(), CubeNode::GetIndices());
	shared_ptr<Material>	bricks = make_shared<Material>(shader); bricks->SetTexture(L"Bricks.png");
	shared_ptr<Material>	concrete = make_shared<Material>(shader); concrete->SetTexture(L"Concrete.png");
	shared_ptr<Material>	wood = make_shared<Material>(shader); wood->SetTexture(L"Wood.png");

	cubeBox->Apply();

	// Construct all mesh nodes...
	shared_ptr<MeshNode> body = SceneGraph::Create<MeshNode>(L"Body Geometry");
	shared_ptr<MeshNode> leftArm = SceneGraph::Create<MeshNode>(L"Left Arm Geometry");
	shared_ptr<MeshNode> rightArm = SceneGraph::Create<MeshNode>(L"Right Arm Geometry");
	shared_ptr<MeshNode> leftLeg = SceneGraph::Create<MeshNode>(L"Left Leg Geometry");
	shared_ptr<MeshNode> rightLeg = SceneGraph::Create<MeshNode>(L"Right Leg Geometry");
	shared_ptr<MeshNode> head = SceneGraph::Create<MeshNode>(L"Head Geometry");

	body->SetMaterial(wood);
	leftArm->SetMaterial(concrete);
	rightArm->SetMaterial(concrete);
	leftLeg->SetMaterial(concrete);
	rightLeg->SetMaterial(concrete);
	head->SetMaterial(bricks);

	body->SetMesh(cubeBox);
	leftArm->SetMesh(cubeBox);
	rightArm->SetMesh(cubeBox);
	leftLeg->SetMesh(cubeBox);
	rightLeg->SetMesh(cubeBox);
	head->SetMesh(cubeBox);

	// Attach all nodes...

	bodyRoot->Add(body);
	leftArmRoot->Add(leftArm);
	rightArmRoot->Add(rightArm);
	leftLegRoot->Add(leftLeg);
	rightLegRoot->Add(rightLeg);
	headRoot->Add(head);

	return true;
}

void RobotNode::Update(FXMMATRIX& m)
{
	// Rotate around!
	Vector3 rotation = GetRotation();
	rotation += Vector3::UpVector * RotationSpeed;
	SetRotation(rotation);

	SceneGraph::Update(m);
}
