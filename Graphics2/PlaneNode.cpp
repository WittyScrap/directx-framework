#include "PlaneNode.h"

bool PlaneNode::Initialise()
{
	shared_ptr<Mesh> planeMesh = RESOURCES->GetMesh(L"Plane/Bonanza.3ds");
	shared_ptr<MeshNode> plane = SceneGraph::Create<MeshNode>(L"Plane Geometry");

	plane->Build(planeMesh);
	Add(plane);

	return SceneGraph::Initialise();
}

void PlaneNode::Update(FXMMATRIX& m)
{
	SceneGraph::Update(m);
}
