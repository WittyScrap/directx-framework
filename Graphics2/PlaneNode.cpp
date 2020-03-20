#include "PlaneNode.h"

bool PlaneNode::Initialise()
{
	shared_ptr<Mesh> planeMesh = RESOURCES->GetMesh(L"Plane/Bonanza.3ds");
	Map(planeMesh);

	return MeshNode::Initialise();
}

void PlaneNode::Update(FXMMATRIX& m)
{
	MeshNode::Update(m);
}
